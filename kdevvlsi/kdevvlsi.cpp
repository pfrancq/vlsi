/*

  kdevvlsi.cpp

  Description - Implementation.

  (c) 2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
// include files for QT
#include <qdir.h>
#include <qprinter.h>
#include <qvbox.h>
#include <qwhatsthis.h>
#include <qtooltip.h>
#include <qtoolbutton.h>
#include <qstring.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstdaction.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kdevvlsi.h"
#include "kdevvlsiview.h"
#include "kvlsiprjview.h"
#include "kvlsiheuristicview.h"
#include "kdevvlsidoc.h"
#include "qdlgoptions.h"



//-----------------------------------------------------------------------------
//
// class KDevVLSIApp
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KDevVLSIApp::KDevVLSIApp(void)
	: KMainWindow(0,"KDevVLSI")
{
	config=kapp->config();
	printer = new QPrinter;
	untitledCount=0;
	pDocList = new QList<KDevVLSIDoc>();
	pDocList->setAutoDelete(true);
	step=false;
	setCaption("");

	// call inits to invoke all other construction parts
	initStatusBar();
	initView();
	initActions();
	
	readOptions();

	// disable actions at startup
	fileNew->setEnabled(false);
	fileSave->setEnabled(false);
	fileSaveAs->setEnabled(false);
	filePrint->setEnabled(false);
	editCut->setEnabled(false);
	editCopy->setEnabled(false);
	editPaste->setEnabled(false);
	windowNewWindow->setEnabled(false);
	heuristicBL->setEnabled(false);
	heuristicEdge->setEnabled(false);
	heuristicCenter->setEnabled(false);
	heuristicRun->setEnabled(false);
	heuristicNext->setEnabled(false);
}


//-----------------------------------------------------------------------------
KDevVLSIApp::~KDevVLSIApp(void)
{
	delete printer;
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::initActions(void)
{
	// Menu "file"
	fileNew = KStdAction::openNew(this, SLOT(slotFileNew(void)), actionCollection());
	fileOpen = KStdAction::open(this, SLOT(slotFileOpen(void)), actionCollection());
	fileOpenRecent = KStdAction::openRecent(this, SLOT(slotFileOpenRecent(const KURL&)), actionCollection());
	fileSave = KStdAction::save(this, SLOT(slotFileSave(void)), actionCollection());
	fileSaveAs = KStdAction::saveAs(this, SLOT(slotFileSaveAs(void)), actionCollection());
	fileClose = KStdAction::close(this, SLOT(slotFileClose(void)), actionCollection());
	filePrint = KStdAction::print(this, SLOT(slotFilePrint(void)), actionCollection());
	fileQuit = KStdAction::quit(this, SLOT(slotFileQuit(void)), actionCollection());
	fileNew->setStatusText(i18n("Creates a new document"));
	fileOpen->setStatusText(i18n("Opens an existing document"));
	fileOpenRecent->setStatusText(i18n("Opens a recently used file"));
	fileSave->setStatusText(i18n("Saves the actual document"));
	fileSaveAs->setStatusText(i18n("Saves the actual document as..."));
	fileClose->setStatusText(i18n("Closes the actual document"));
	filePrint ->setStatusText(i18n("Prints out the actual document"));
	fileQuit->setStatusText(i18n("Quits the application"));

	// Menu "Edit"
	editCut = KStdAction::cut(this, SLOT(slotEditCut(void)), actionCollection());
	editCopy = KStdAction::copy(this, SLOT(slotEditCopy(void)), actionCollection());
	editPaste = KStdAction::paste(this, SLOT(slotEditPaste(void)), actionCollection());
	editCut->setStatusText(i18n("Cuts the selected section and puts it to the clipboard"));
	editCopy->setStatusText(i18n("Copies the selected section to the clipboard"));
	editPaste->setStatusText(i18n("Pastes the clipboard contents to actual position"));

	// Menu "Window"
	windowNewWindow = new KAction(i18n("New &Window"), 0, this, SLOT(slotWindowNewWindow(void)), actionCollection(),"window_new_window");
	windowTile = new KAction(i18n("&Tile"), 0, this, SLOT(slotWindowTile(void)), actionCollection(),"window_tile");
	windowCascade = new KAction(i18n("&Cascade"), 0, this, SLOT(slotWindowCascade(void)), actionCollection(),"window_cascade");
	windowMenu = new KActionMenu(i18n("&Window"), actionCollection(), "window_menu");
	connect(windowMenu->popupMenu(), SIGNAL(aboutToShow(void)), this, SLOT(windowMenuAboutToShow(void)));

	// Menu "Heuristic"
	heuristicBL=new KAction(i18n("&Bottom-Left Heuristic"),KAccel::stringToKey("Alt+B"),this,SLOT(slotHeuristicBL(void)),actionCollection(),"heuristic_bl");
	heuristicEdge=new KAction(i18n("&Edge Heuristic"),KAccel::stringToKey("Alt+E"),this,SLOT(slotHeuristicEdge(void)),actionCollection(),"heuristic_edge");
	heuristicCenter=new KAction(i18n("&Center Heuristic"),KAccel::stringToKey("Alt+C"),this,SLOT(slotHeuristicCenter(void)),actionCollection(),"heuristic_center");
	heuristicRun=new KAction(i18n("&Run Heuristic"),"run",KAccel::stringToKey("Alt+R"),this,SLOT(slotHeuristicRun(void)),actionCollection(),"heuristic_run");
	heuristicNext=new KAction(i18n("&Next step for Heuristic"),"next",KAccel::stringToKey("Alt+N"),this,SLOT(slotHeuristicNext(void)),actionCollection(),"heuristic_next");

	// Menu "Settings"
	viewToolBar = KStdAction::showToolbar(this, SLOT(slotViewToolBar(void)), actionCollection());
	viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar(void)), actionCollection());
	settingsOptions = new KAction(i18n("&Options"),0,this,SLOT(slotSettingsOptions(void)),actionCollection(),"settings_options");
	viewToolBar->setStatusText(i18n("Enables/disables the toolbar"));
	viewStatusBar->setStatusText(i18n("Enables/disables the statusbar"));
	settingsOptions->setStatusText(i18n("Set the options"));

	createGUI();
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::initStatusBar(void)
{
	// TODO: add your own items you need for displaying current application status.
	statusBar()->insertItem(i18n("Ready."),1);
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::initView(void)
{ 
	// here the main view of the KMainWindow is created by a background box and
	// the QWorkspace instance for MDI view.
	QVBox* view_back = new QVBox( this );
	view_back->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	pWorkspace = new QWorkspace( view_back );
	connect(pWorkspace, SIGNAL(windowActivated(QWidget*)), this, SLOT(setWndTitle(QWidget*)));
	setCentralWidget(view_back);
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::setWndTitle(QWidget*)
{
	setCaption(pWorkspace->activeWindow()->caption());
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::createClient(KDevVLSIDoc* doc)
{
	KDevVLSIView* w = new KVLSIPrjView(doc, pWorkspace,0,WDestructiveClose);
	w->installEventFilter(this);
	doc->addView(w);
	w->setIcon(kapp->miniIcon());
	w->show();
	w->resize(pWorkspace->sizeHint());	
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::openDocumentFile(const KURL& url)
{
	KDevVLSIDoc* doc;

	slotStatusMsg(i18n("Opening file..."));

	// check, if document already open. If yes, set the focus to the first view
	for(doc=pDocList->first(); doc > 0; doc=pDocList->next())
	{
		if(doc->URL()==url)
		{
			KDevVLSIView* view=doc->firstView();	
			view->setFocus();
			return;
		}
	}
	
	// Create the document
	doc = new KDevVLSIDoc();
	pDocList->append(doc);
	doc->newDocument();
	connect(doc,SIGNAL(endRun(void)),this,SLOT(slotEndHeuristic(void)));

	// Creates an untitled window if file is 0	
	if(url.isEmpty())
	{
		untitledCount+=1;
		QString fileName=QString(i18n("Untitled%1")).arg(untitledCount);
		KURL url;
		url.setFileName(fileName);
		doc->setURL(url);
	}
	// Open the file
	else
	{
		if(!doc->openDocument(url))
		{
			KMessageBox::error (this,i18n("Could not open document !"), i18n("Error !"));
			delete doc;
			return;	
		}
		fileOpenRecent->addURL(url);
	}

	// create the window
	createClient(doc);
	heuristicBL->setEnabled(true);
	heuristicEdge->setEnabled(true);
	heuristicCenter->setEnabled(true);

	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::saveOptions(void)
{	
	config->setGroup("General Options");
	config->writeEntry("Geometry", size());
	config->writeEntry("Show Toolbar", toolBar()->isVisible());
	config->writeEntry("Show Statusbar",statusBar()->isVisible());
	config->writeEntry("ToolBarPos", (int) toolBar("mainToolBar")->barPos());
	fileOpenRecent->saveEntries(config,"Recent Files");
	config->setGroup("Heuristic Options");
	config->writeEntry("Step Mode",step);
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::readOptions(void)
{
	config->setGroup("General Options");

	// bar status settings
	bool bViewToolbar = config->readBoolEntry("Show Toolbar", true);
	viewToolBar->setChecked(bViewToolbar);
	slotViewToolBar();

	bool bViewStatusbar = config->readBoolEntry("Show Statusbar", true);
	viewStatusBar->setChecked(bViewStatusbar);
	slotViewStatusBar();

	// bar position settings
	KToolBar::BarPosition toolBarPos;
	toolBarPos=(KToolBar::BarPosition) config->readNumEntry("ToolBarPos", KToolBar::Top);
	toolBar("mainToolBar")->setBarPos(toolBarPos);

	// initialize the recent file list
	fileOpenRecent->loadEntries(config,"Recent Files");

	// Set size
	QSize size=config->readSizeEntry("Geometry");
	if(!size.isEmpty())
	{
		resize(size);
	}

	// Heuristics settings
	config->setGroup("Heuristic Options");
	step=config->readBoolEntry("Step Mode",false);
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::saveProperties(KConfig* /*_cfg*/)
{

}


//-----------------------------------------------------------------------------
void KDevVLSIApp::readProperties(KConfig* /*_cfg*/)
{

}


//-----------------------------------------------------------------------------
bool KDevVLSIApp::queryClose(void)
{
	QStringList saveFiles;
	KDevVLSIDoc* doc;

	if(pDocList->isEmpty())
		return true;

	for(doc=pDocList->first(); doc!=0;doc=pDocList->next())
	{
		if(doc->isModified())
			saveFiles.append(doc->URL().fileName());
	}
	if(saveFiles.isEmpty())
		return true;

	switch(KMessageBox::questionYesNoList(this,i18n("One or more documents have been modified.\nSave changes before exiting?"),saveFiles))
	{
		case KMessageBox::Yes:
			for(doc=pDocList->first(); doc!=0;doc=pDocList->next())
			{
				if(doc->URL().fileName().contains(i18n("Untitled")))
					slotFileSaveAs();
				else
				{
					if(!doc->saveDocument(doc->URL()))
					{
						KMessageBox::error(this,i18n("Could not save the current document !"), i18n("I/O Error !"));
						return false;
					}
				}
			}
			return true;

		case KMessageBox::No:
		default:
			return true;
	}
}


//-----------------------------------------------------------------------------
bool KDevVLSIApp::queryExit(void)
{
	saveOptions();
	return true;
}


//-----------------------------------------------------------------------------
bool KDevVLSIApp::eventFilter(QObject* object, QEvent* event)
{
	if((event->type()==QEvent::Close)&&((KDevVLSIApp*)object!=this))
	{
		QCloseEvent* e=(QCloseEvent*)event;
		KDevVLSIView* pView=(KDevVLSIView*)object;
		KDevVLSIDoc* pDoc=pView->getDocument();
		if(pDoc->canCloseFrame(pView))
		{
			pDoc->removeView(pView);
			if(!pDoc->firstView())
			{
				pDoc->closeDocument();
				pDocList->remove(pDoc);
      		if(pDocList->isEmpty())
      		{
      			heuristicBL->setEnabled(false);
      			heuristicEdge->setEnabled(false);
      			heuristicCenter->setEnabled(false);
      			heuristicRun->setEnabled(false);
            }
			}
			e->accept();
      	//////////////	
			QWidgetList l=pWorkspace->windowList();
			l.remove(pView);
			if(l.count()<1)
				setPlainCaption(kapp->caption());
			else
				setCaption(pWorkspace->activeWindow()->caption());
			//////////////
		}
		else
			e->ignore();
	}
	return QWidget::eventFilter( object, event );    // standard event processing
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotFileNew(void)
{
	slotStatusMsg(i18n("Creating new document..."));
	openDocumentFile();
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotFileOpen(void)
{
	slotStatusMsg(i18n("Opening file..."));	
	KURL url=KFileDialog::getOpenURL(QString::null,i18n("*|All files"), this, i18n("Open File..."));
	if(!url.isEmpty())
	{
		openDocumentFile(url);
		fileOpenRecent->addURL(url);
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotFileOpenRecent(const KURL& url)
{
	slotStatusMsg(i18n("Opening file..."));  	
	openDocumentFile(url);	
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotFileSave(void)
{
	slotStatusMsg(i18n("Saving file..."));
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m)
	{
		KDevVLSIDoc* doc = m->getDocument();
		if(doc->URL().fileName().contains(i18n("Untitled")))
			slotFileSaveAs();
		else
			if(!doc->saveDocument(doc->URL()))
		KMessageBox::error (this,i18n("Could not save the current document !"), i18n("I/O Error !"));
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotFileSaveAs(void)
{
	slotStatusMsg(i18n("Saving file with a new filename..."));
	KURL url=KFileDialog::getSaveURL(QDir::currentDirPath(),i18n("*|All files"), this, i18n("Save as..."));
	if(!url.isEmpty())
	{
		KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
		if(m)
		{
			KDevVLSIDoc* doc =	m->getDocument();
			if(!doc->saveDocument(url))
			{
        		KMessageBox::error (this,i18n("Could not save the current document !"), i18n("I/O Error !"));
        		return;
      	}
			doc->changedViewList();
//			setWndTitle(m);
			fileOpenRecent->addURL(url);
		}	
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotFileClose(void)
{
	slotStatusMsg(i18n("Closing file..."));
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m)
	{
		KDevVLSIDoc* doc=m->getDocument();
		doc->closeDocument();
		pDocList->remove(doc);
		if(pDocList->isEmpty())
		{
			heuristicBL->setEnabled(false);
			heuristicEdge->setEnabled(false);
			heuristicCenter->setEnabled(false);
      }
	}	
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotFilePrint(void)
{
	slotStatusMsg(i18n("Printing..."));	
	KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
	if(m)
		m->print( printer );
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotFileQuit(void)
{
	KMainWindow* w;

	slotStatusMsg(i18n("Exiting..."));
	saveOptions();
	// close the first window, the list makes the next one the first again.
	// This ensures that queryClose() is called on each window to ask for closing
	if(memberList)
	{
		for(w=memberList->first(); w!=0; w=memberList->first())
		{
			// only close the window if the closeEvent is accepted. If the user presses Cancel on the saveModified() dialog,
			// the window and the application stay open.
			if(!w->close())
				break;
		}
	}	
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotEditUndo(void)
{
	slotStatusMsg(i18n("Reverting last action..."));	
/*	KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
	if(m)
	  m->undo();*/
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotEditCut(void)
{
	slotStatusMsg(i18n("Cutting selection..."));	
/*	KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
	if(m)
		m->cut();*/
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotEditCopy(void)
{
	slotStatusMsg(i18n("Copying selection to clipboard..."));	
/*	KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
	if(m)
		m->copy();*/		
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotEditPaste(void)
{
	slotStatusMsg(i18n("Inserting clipboard contents..."));	
/*	KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
	if(m)
		m->paste();*/		
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotHeuristicBL(void)
{
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m)
	{
		heuristicRun->setEnabled(true);
		heuristicNext->setEnabled(true);
		KDevVLSIDoc* doc = m->getDocument();
		KVLSIHeuristicView* w = new KVLSIHeuristicView(doc,BottomLeft,pWorkspace,0,WDestructiveClose);
		w->installEventFilter(this);
		doc->addView(w);
		w->setIcon(kapp->miniIcon());
		w->show();
		w->resize(pWorkspace->sizeHint());	
		doc->slotHeuristic(BottomLeft,step,w);
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotHeuristicEdge(void)
{
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m)
	{
		heuristicRun->setEnabled(true);
		heuristicNext->setEnabled(true);
		KDevVLSIDoc* doc = m->getDocument();
		KVLSIHeuristicView* w = new KVLSIHeuristicView(doc,Edge,pWorkspace,0,WDestructiveClose);
		w->installEventFilter(this);
		doc->addView(w);
		w->setIcon(kapp->miniIcon());
		w->show();
		w->resize(pWorkspace->sizeHint());	
		doc->slotHeuristic(Edge,step,w);
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotHeuristicCenter(void)
{
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m)
	{
		heuristicRun->setEnabled(true);
		heuristicNext->setEnabled(true);
		KDevVLSIDoc* doc = m->getDocument();
		KVLSIHeuristicView* w = new KVLSIHeuristicView(doc,Center,pWorkspace,0,WDestructiveClose);
		w->installEventFilter(this);
		doc->addView(w);
		w->setIcon(kapp->miniIcon());
		w->show();
		w->resize(pWorkspace->sizeHint());	
		doc->slotHeuristic(Center,step,w);
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotEndHeuristic(void)
{
	bool bRun=false;
	KDevVLSIDoc *doc;

	// Scan all documents to see if all heuristics are end.
   for(doc=pDocList->first();doc!=0;doc=pDocList->next())
		if(doc->Run)
			bRun=true;

	if(!bRun)
	{
		heuristicNext->setEnabled(false);
		heuristicRun->setEnabled(false);
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotHeuristicNext(void)
{
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m)
	{
		KDevVLSIDoc* doc = m->getDocument();
		if((doc->Run)&&(doc->curView==m))
			doc->slotNextStep(step);
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotHeuristicRun(void)
{
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m)
	{
		KDevVLSIDoc* doc = m->getDocument();
		doc->slotRun();
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotViewToolBar(void)
{
	slotStatusMsg(i18n("Toggle the toolbar..."));
	// turn Toolbar on or off
	if(!viewToolBar->isChecked())
	{
		toolBar("mainToolBar")->hide();
	}
	else
	{
		toolBar("mainToolBar")->show();
	}		
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotViewStatusBar(void)
{
	slotStatusMsg(i18n("Toggle the statusbar..."));
	//turn Statusbar on or off
	if(!viewStatusBar->isChecked())
	{
		statusBar()->hide();
	}
	else
	{
		statusBar()->show();
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotSettingsOptions(void)
{
	slotStatusMsg(i18n("Set the options..."));
	QDlgOptions dlg(this);
	dlg.show();
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotWindowNewWindow(void)
{
	slotStatusMsg(i18n("Opening a new application window..."));
	KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
	if(m)
	{
		KDevVLSIDoc* doc = m->getDocument();
		createClient(doc);
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotWindowTile(void)
{
	pWorkspace->tile();
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotWindowCascade(void)
{
	pWorkspace->cascade();
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotStatusMsg(const QString &text)
{
	// change status message permanently
	statusBar()->clear();
	statusBar()->changeItem(text,1);
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::windowMenuAboutToShow(void)
{
	windowMenu->popupMenu()->clear();
	windowMenu->insert(windowNewWindow);
	windowMenu->insert(windowCascade);
	windowMenu->insert(windowTile);
	if(pWorkspace->windowList().isEmpty())
	{
		windowNewWindow->setEnabled(false);
		windowCascade->setEnabled(false);
		windowTile->setEnabled(false);
	}
	else
	{
		windowNewWindow->setEnabled(true);
		windowCascade->setEnabled(true);
		windowTile->setEnabled(true);
	}
	windowMenu->popupMenu()->insertSeparator();
	QWidgetList windows = pWorkspace->windowList();
	for(int i=0;i<int(windows.count());++i)
	{
		int id=windowMenu->popupMenu()->insertItem(QString("&%1 ").arg(i+1)+windows.at(i)->caption(),this,SLOT(windowMenuActivated(int)));
		windowMenu->popupMenu()->setItemParameter(id,i);
		windowMenu->popupMenu()->setItemChecked(id,pWorkspace->activeWindow()==windows.at(i));
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::windowMenuActivated(int id)
{
	QWidget* w = pWorkspace->windowList().at(id);
	if(w)
		w->setFocus();
}
