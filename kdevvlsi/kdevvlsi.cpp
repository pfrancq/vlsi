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
// include files for Qt
#include <qapplication.h>
#include <qdir.h>
#include <qprinter.h>
#include <qvbox.h>
#include <qwhatsthis.h>
#include <qtooltip.h>
#include <qtoolbutton.h>
#include <qstring.h>
#include <qvalidator.h>
#include <qlineedit.h>
#include <qsplitter.h>
#include <qcheckbox.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstdaction.h>
#include <kcombobox.h>
#include <kpopupmenu.h>
#include <kstatusbar.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kdevvlsi.h"
#include "kdevvlsiview.h"
#include "kvlsiprjview.h"
#include "kvlsigaview.h"
#include "kvlsiheuristicview.h"
#include "kvlsiviewpolygons.h"
#include "kdevvlsidoc.h"
#include "kappoptions.h"



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
	heuristicSelect->setEnabled(false);	
	GAInit->setEnabled(false);
	GAStart->setEnabled(false);	
	GAPause->setEnabled(false);	
	GAStop->setEnabled(false);	
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
	heuristicBL=new KAction(i18n("&Bottom-Left Heuristic"),KKey("Alt+B").keyCodeQt(),this,SLOT(slotHeuristicBL(void)),actionCollection(),"heuristic_bl");
	heuristicEdge=new KAction(i18n("&Edge Heuristic"),KKey("Alt+E").keyCodeQt(),this,SLOT(slotHeuristicEdge(void)),actionCollection(),"heuristic_edge");
	heuristicCenter=new KAction(i18n("&Center Heuristic"),KKey("Alt+C").keyCodeQt(),this,SLOT(slotHeuristicCenter(void)),actionCollection(),"heuristic_center");
	heuristicRun=new KAction(i18n("&Run Heuristic"),"run",KKey("Alt+R").keyCodeQt(),this,SLOT(slotHeuristicRun(void)),actionCollection(),"heuristic_run");
	heuristicNext=new KAction(i18n("&Next step for Heuristic"),"next",KKey("Alt+N").keyCodeQt(),this,SLOT(slotHeuristicNext(void)),actionCollection(),"heuristic_next");
	heuristicSelect=new KAction(i18n("&Select objects of Heuristic"),"queue",KKey("Alt+O").keyCodeQt(),this,SLOT(slotHeuristicSelect(void)),actionCollection(),"heuristic_select");

	// Menu "GA"
	GAInit=new KAction(i18n("&Initialize"),"reload",KKey("Alt+I").keyCodeQt(),this,SLOT(slotGAInit(void)),actionCollection(),"ga_init");
	GAStart=new KAction(i18n("&Start"),"exec",KKey("Alt+S").keyCodeQt(),this,SLOT(slotGAStart(void)),actionCollection(),"ga_start");
	GAPause=new KAction(i18n("&Pause"),"player_pause",KKey("Alt+P").keyCodeQt(),this,SLOT(slotGAPause(void)),actionCollection(),"ga_pause");
	GAStop=new KAction(i18n("&Stop"),"stop",KKey("Alt+T").keyCodeQt(),this,SLOT(slotGAStop(void)),actionCollection(),"ga_stop");

	// Menu "Tools"
	ToolsViewPolygons=new KAction(i18n("View &Polygons"),"viewmag",0,this,SLOT(slotViewPolygons(void)),actionCollection(),"tools_view_polygons");

	// Menu "Settings"
	viewToolBar = KStdAction::showToolbar(this, SLOT(slotViewToolBar(void)), actionCollection());
	viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar(void)), actionCollection());
	settingsOptions = new KAction(i18n("&Options"),"configure",0,this,SLOT(slotSettingsOptions(void)),actionCollection(),"settings_options");
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
	//QSplitter* view_back=new QSplitter(QSplitter::Vertical,this);
	view_back->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	pWorkspace = new QWorkspace( view_back );
	//Output=new QMultiLineEdit(view_back);
	connect(pWorkspace, SIGNAL(windowActivated(QWidget*)), this, SLOT(slotWindowActivated(QWidget*)));
	setCentralWidget(view_back);
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotWindowActivated(QWidget*)
{
	bool bPrj,bGA,bHeuristic;

	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m)
	{
		// Update caption
		setCaption(m->caption());

		// Update menu
		switch(m->getType())
		{
			case Project:
				bPrj=true;
				bGA=false;
				bHeuristic=false;
				break;

			case Heuristic:
				bPrj=false;
				bGA=false;
				bHeuristic=true;
				break;

			case GA:
				bPrj=false;
				bGA=true;
				bHeuristic=false;
				break;

			default:
				bPrj=false;
				bGA=false;
				bHeuristic=false;
				break;
		}
		GAInit->setEnabled(bPrj);
		heuristicBL->setEnabled(bPrj);
		heuristicEdge->setEnabled(bPrj);
		heuristicCenter->setEnabled(bPrj);
		GAStart->setEnabled(bGA);
		GAPause->setEnabled(bGA);
		GAStop->setEnabled(bGA);
		heuristicRun->setEnabled(bHeuristic);
		heuristicNext->setEnabled(bHeuristic);
		heuristicSelect->setEnabled(bHeuristic);
	}
	else
	{
		setCaption("");
		GAInit->setEnabled(false);
		heuristicBL->setEnabled(false);
		heuristicEdge->setEnabled(false);
		heuristicCenter->setEnabled(false);
		GAStart->setEnabled(false);
		GAPause->setEnabled(false);
		GAStop->setEnabled(false);
		heuristicRun->setEnabled(false);
		heuristicNext->setEnabled(false);
		heuristicSelect->setEnabled(false);
	}
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

	// Creates an untitled window if file is 0	
	if(url.isEmpty())
	{
		untitledCount+=1;
		QString fileName=QString(i18n("Untitled%1")).arg(untitledCount);
		KURL url;
		url.setFileName(fileName);
		doc->setURL(url);
	}
	else
	{
		// Open the file
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
	config->writeEntry("All Orientations",allOri);
	config->writeEntry("Free Polygons Calculated",calcFree);
	config->writeEntry("Free Polygons Used",useFree);
	config->writeEntry("Prométhée p for Area",HeurArea.P);
	config->writeEntry("Prométhée q for Area",HeurArea.Q);
	config->writeEntry("Prométhée weight for Area",HeurArea.Weight);
	config->writeEntry("Prométhée p for Dist",HeurDist.P);
	config->writeEntry("Prométhée q for Dist",HeurDist.Q);
	config->writeEntry("Prométhée weight for Dist",HeurDist.Weight);
	config->setGroup("Selection Options");
	config->writeEntry("Prométhée p for Weight",SelectWeight.P);
	config->writeEntry("Prométhée q for Weight",SelectWeight.Q);
	config->writeEntry("Prométhée weight for Weight",SelectWeight.Weight);
	config->writeEntry("Prométhée p for Dist",SelectDist.P);
	config->writeEntry("Prométhée q for Dist",SelectDist.Q);
	config->writeEntry("Prométhée weight for Dist",SelectDist.Weight);
	config->setGroup("GA Options");
	config->writeEntry("Heuristic Type",GAHeur);
	config->writeEntry("Maximum Generation",GAMaxGen);
	config->writeEntry("Step Generation",GAStepGen);
	config->writeEntry("Population Size",GAPopSize);
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
	allOri=config->readBoolEntry("All Orientations",false);	
	calcFree=config->readBoolEntry("Free Polygons Calculated",true);
	useFree=config->readBoolEntry("Free Polygons Used",true);
	HeurArea.P=config->readDoubleNumEntry("Prométhée p for Area",0.0);
	HeurArea.Q=config->readDoubleNumEntry("Prométhée q for Area",0.0);
	HeurArea.Weight=config->readDoubleNumEntry("Prométhée weight for Area",2.0);
	HeurDist.P=config->readDoubleNumEntry("Prométhée p for Dist",0.0);
	HeurDist.Q=config->readDoubleNumEntry("Prométhée q for Dist",0.0);
	HeurDist.Weight=config->readDoubleNumEntry("Prométhée weight for Dist",1.0);
	config->setGroup("Selection Options");
	SelectWeight.P=config->readDoubleNumEntry("Prométhée p for Weight",0.0);
	SelectWeight.Q=config->readDoubleNumEntry("Prométhée q for Weight",0.0);
	SelectWeight.Weight=config->readDoubleNumEntry("Prométhée weight for Weight",1.0);
	SelectDist.P=config->readDoubleNumEntry("Prométhée p for Dist",0.0);
	SelectDist.Q=config->readDoubleNumEntry("Prométhée q for Dist",0.0);
	SelectDist.Weight=config->readDoubleNumEntry("Prométhée weight for Dist",1.0);
	config->setGroup("GA Options");
	GAHeur=static_cast<HeuristicType>(config->readNumEntry("Heuristic Type",BottomLeft));
	GAMaxGen=config->readUnsignedLongNumEntry("Maximum Generation",100);
	GAStepGen=config->readUnsignedLongNumEntry("Step Generation",0);
	GAPopSize=config->readUnsignedLongNumEntry("Population Size",16);
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
		if(pDoc&&(pDoc->canCloseFrame(pView)))
		{
			pDoc->removeView(pView);
			if((!pDoc->firstView())||(pView->getType()==Project))
			{
				pDoc->closeDocument();
				pDocList->remove(pDoc);
			}
		}
		QWidgetList l=pWorkspace->windowList();
		l.remove(pView);
		if(l.count()<1)
			setPlainCaption(kapp->caption());
		else
			setCaption(pWorkspace->activeWindow()->caption());
		e->accept();
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
	KApplication::kApplication()->processEvents(1000);
	KURL url=KFileDialog::getOpenURL("~",i18n("*.pl2d|2D Placement files"), this, i18n("Open File..."));
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
	KApplication::kApplication()->processEvents(1000);
	openDocumentFile(url);	
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotFileSave(void)
{
	slotStatusMsg(i18n("Saving file..."));
	KApplication::kApplication()->processEvents(1000);
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
	KApplication::kApplication()->processEvents(1000);
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
// 	KMainWindow* w;

	slotStatusMsg(i18n("Exiting..."));
	saveOptions();
// 	// close the first window, the list makes the next one the first again.
// 	// This ensures that queryClose() is called on each window to ask for closing
// 	if(memberList)
// 	{
// 		for(w=memberList->first(); w!=0; w=memberList->first())
// 		{
// 			// only close the window if the closeEvent is accepted. If the user presses Cancel on the saveModified() dialog,
// 			// the window and the application stay open.
// 			if(!w->close())
// 				break;
// 		}
// 	}
	close();
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotEditUndo(void)
{
	slotStatusMsg(i18n("Reverting last action..."));
//	KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
//	if(m)
//	  m->undo();
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotEditCut(void)
{
	slotStatusMsg(i18n("Cutting selection..."));
//	KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
//	if(m)
//		m->cut();
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotEditCopy(void)
{
	slotStatusMsg(i18n("Copying selection to clipboard..."));
//	KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
//	if(m)
//		m->copy();
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotEditPaste(void)
{
	slotStatusMsg(i18n("Inserting clipboard contents..."));
//	KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
//	if(m)
//		m->paste();
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotHeuristicBL(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==Project))
	{
		KDevVLSIDoc* doc = m->getDocument();
		KVLSIHeuristicView* w = new KVLSIHeuristicView(doc,BottomLeft,pWorkspace,0,WDestructiveClose);
		w->installEventFilter(this);
		doc->addView(w);
		w->setIcon(kapp->miniIcon());
		w->resize(pWorkspace->sizeHint());
		w->show();
		w->setFocus();
		w->RunHeuristic();
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotHeuristicEdge(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==Project))
	{
		KDevVLSIDoc* doc = m->getDocument();
		KVLSIHeuristicView* w = new KVLSIHeuristicView(doc,Edge,pWorkspace,0,WDestructiveClose);
		w->installEventFilter(this);
		doc->addView(w);
		w->setIcon(kapp->miniIcon());
		w->resize(pWorkspace->sizeHint());
		w->show();
		w->setFocus();
		w->RunHeuristic();
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotHeuristicCenter(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==Project))
	{
		KDevVLSIDoc* doc = m->getDocument();
		KVLSIHeuristicView* w = new KVLSIHeuristicView(doc,Center,pWorkspace,0,WDestructiveClose);
		w->installEventFilter(this);
		doc->addView(w);
		w->setIcon(kapp->miniIcon());
		w->resize(pWorkspace->sizeHint());
		w->show();
		w->setFocus();	
		w->RunHeuristic();
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotEndHeuristic(void)
{
	bool bRun=false;
	KDevVLSIView* v;
	QWidgetList list;

	KApplication::kApplication()->processEvents(1000);

	// Scan all documents to see if all heuristics are end.
	list=pWorkspace->windowList();
	for(v=(KDevVLSIView*)list.first();v!=0;v=(KDevVLSIView*)list.next())
	{
		if(v->getType()==Heuristic)
		{
			if(((KVLSIHeuristicView*)v)->Running())
				bRun=true;
		}
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotHeuristicNext(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==Heuristic))
	{
		((KVLSIHeuristicView*)m)->NextStep();
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotHeuristicRun(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==Heuristic))
	{
		((KVLSIHeuristicView*)m)->RunToEnd();
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotHeuristicSelect(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==Heuristic))
	{
		((KVLSIHeuristicView*)m)->SelectObjects();
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotGAInit(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==Project))
	{
		KDevVLSIDoc* doc = m->getDocument();
		KVLSIGAView* w = new KVLSIGAView(doc,pWorkspace,0,WDestructiveClose);
		w->installEventFilter(this);
		doc->addView(w);
		w->setIcon(kapp->miniIcon());
		w->resize(pWorkspace->sizeHint());
		w->show();
		w->setFocus();
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotGAStart(void)
{
	KApplication::kApplication()->processEvents(1000);	
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==GA))
	{
		((KVLSIGAView*)m)->RunGA();
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotGAPause(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==GA))
	{
		((KVLSIGAView*)m)->PauseGA();
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotGAStop(void)
{
	KApplication::kApplication()->processEvents(1000);
	KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==GA))
	{
		((KVLSIGAView*)m)->StopGA();
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIApp::slotViewPolygons(void)
{
	slotStatusMsg(i18n("Opening file..."));
	KURL url=KFileDialog::getOpenURL(QString::null,QString::null, this, i18n("Open Polygon File..."));
	if(!url.isEmpty())
	{
		KVLSIViewPolygons* w = new KVLSIViewPolygons(url.path(),pWorkspace,0,WDestructiveClose);
		w->installEventFilter(this);
		w->setIcon(kapp->miniIcon());
		w->resize(pWorkspace->sizeHint());
		w->show();
		w->setFocus();
	}
	slotStatusMsg(i18n("Ready."));
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
	KAppOptions dlg(this,"Options",true);
	dlg.txtHeurAreaP->setValidator(new QDoubleValidator(this));
	dlg.txtHeurAreaQ->setValidator(new QDoubleValidator(this));
	dlg.txtHeurAreaW->setValidator(new QDoubleValidator(this));
	dlg.txtHeurDistP->setValidator(new QDoubleValidator(this));
	dlg.txtHeurDistQ->setValidator(new QDoubleValidator(this));
	dlg.txtHeurDistW->setValidator(new QDoubleValidator(this));
	dlg.txtSelectWeightP->setValidator(new QDoubleValidator(this));
	dlg.txtSelectWeightQ->setValidator(new QDoubleValidator(this));
	dlg.txtSelectWeightW->setValidator(new QDoubleValidator(this));
	dlg.txtSelectDistP->setValidator(new QDoubleValidator(this));
	dlg.txtSelectDistQ->setValidator(new QDoubleValidator(this));
	dlg.txtSelectDistW->setValidator(new QDoubleValidator(this));
	dlg.cbStep->setChecked(step);
	dlg.cbComputePoly->setChecked(calcFree);
	dlg.cbUsePoly->setChecked(useFree);
	dlg.cbUsePoly->setEnabled(calcFree);
	dlg.cbAllOris->setChecked(allOri);
	dlg.txtHeurAreaP->setText(QString::number(HeurArea.P));
	dlg.txtHeurAreaQ->setText(QString::number(HeurArea.Q));
	dlg.txtHeurAreaW->setText(QString::number(HeurArea.Weight));
	dlg.txtHeurDistP->setText(QString::number(HeurDist.P));
	dlg.txtHeurDistQ->setText(QString::number(HeurDist.Q));
	dlg.txtHeurDistW->setText(QString::number(HeurDist.Weight));
	dlg.txtSelectWeightP->setText(QString::number(SelectWeight.P));
	dlg.txtSelectWeightQ->setText(QString::number(SelectWeight.Q));
	dlg.txtSelectWeightW->setText(QString::number(SelectWeight.Weight));
	dlg.txtSelectDistP->setText(QString::number(SelectDist.P));
	dlg.txtSelectDistQ->setText(QString::number(SelectDist.Q));
	dlg.txtSelectDistW->setText(QString::number(SelectDist.Weight));
	dlg.txtMaxGen->setText(QString::number(GAMaxGen));
	dlg.txtStepGen->setText(QString::number(GAStepGen));
	dlg.cbGAHeuristicType->setCurrentItem(GAHeur);
	dlg.txtPopSize->setText(QString::number(GAPopSize));
	if(dlg.exec())
	{
		step=dlg.cbStep->isChecked();
		calcFree=dlg.cbComputePoly->isChecked();
		useFree=dlg.cbUsePoly->isChecked();
		allOri=dlg.cbAllOris->isChecked();
		HeurArea.P=dlg.txtHeurAreaP->text().toDouble();
		HeurArea.Q=dlg.txtHeurAreaQ->text().toDouble();
		HeurArea.Weight=dlg.txtHeurAreaW->text().toDouble();
		HeurDist.P=dlg.txtHeurDistP->text().toDouble();
		HeurDist.Q=dlg.txtHeurDistQ->text().toDouble();
		HeurDist.Weight=dlg.txtHeurDistW->text().toDouble();
		SelectWeight.P=dlg.txtSelectWeightP->text().toDouble();
		SelectWeight.Q=dlg.txtSelectWeightQ->text().toDouble();
		SelectWeight.Weight=dlg.txtSelectWeightW->text().toDouble();
		SelectDist.P=dlg.txtSelectDistP->text().toDouble();
		SelectDist.Q=dlg.txtSelectDistQ->text().toDouble();
		SelectDist.Weight=dlg.txtSelectDistW->text().toDouble();
		GAMaxGen=dlg.txtMaxGen->text().toULong();
		GAStepGen=dlg.txtStepGen->text().toULong();
		GAHeur=static_cast<HeuristicType>(dlg.cbGAHeuristicType->currentItem());
		GAPopSize=dlg.txtPopSize->text().toULong();
	}
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


//-----------------------------------------------------------------------------
KDevVLSIApp::~KDevVLSIApp(void)
{
	if(printer)
		delete printer;
}
