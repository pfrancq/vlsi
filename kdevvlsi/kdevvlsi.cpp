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

// include files for KDE
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstdaction.h>

// application specific includes
#include "kdevvlsi.h"
#include "kdevvlsiview.h"
#include "kdevvlsidoc.h"


KDevVLSIApp::KDevVLSIApp():KMainWindow(0,"KDevVLSI")
{
  config=kapp->config();
  printer = new QPrinter;
  untitledCount=0;
  pDocList = new QList<KDevVLSIDoc>();
  pDocList->setAutoDelete(true);

  ///////////////////////////////////////////////////////////////////
  // call inits to invoke all other construction parts
  initStatusBar();
  initView();
  initActions();
	
  readOptions();

  ///////////////////////////////////////////////////////////////////
  // disable actions at startup
  fileSave->setEnabled(false);
  fileSaveAs->setEnabled(false);
  filePrint->setEnabled(false);
  editCut->setEnabled(false);
  editCopy->setEnabled(false);
  editPaste->setEnabled(false);
}

KDevVLSIApp::~KDevVLSIApp()
{
  delete printer;
}

void KDevVLSIApp::initActions()
{
  fileNew = KStdAction::openNew(this, SLOT(slotFileNew()), actionCollection());
  fileOpen = KStdAction::open(this, SLOT(slotFileOpen()), actionCollection());
  fileOpenRecent = KStdAction::openRecent(this, SLOT(slotFileOpenRecent(const KURL&)), actionCollection());
  fileSave = KStdAction::save(this, SLOT(slotFileSave()), actionCollection());
  fileSaveAs = KStdAction::saveAs(this, SLOT(slotFileSaveAs()), actionCollection());
  fileClose = KStdAction::close(this, SLOT(slotFileClose()), actionCollection());
  filePrint = KStdAction::print(this, SLOT(slotFilePrint()), actionCollection());
  fileQuit = KStdAction::quit(this, SLOT(slotFileQuit()), actionCollection());
  editCut = KStdAction::cut(this, SLOT(slotEditCut()), actionCollection());
  editCopy = KStdAction::copy(this, SLOT(slotEditCopy()), actionCollection());
  editPaste = KStdAction::paste(this, SLOT(slotEditPaste()), actionCollection());
  viewToolBar = KStdAction::showToolbar(this, SLOT(slotViewToolBar()), actionCollection());
  viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection());

  windowNewWindow = new KAction(i18n("New &Window"), 0, this, SLOT(slotWindowNewWindow()), actionCollection(),"window_new_window");
  windowTile = new KAction(i18n("&Tile"), 0, this, SLOT(slotWindowTile()), actionCollection(),"window_tile");
  windowCascade = new KAction(i18n("&Cascade"), 0, this, SLOT(slotWindowCascade()), actionCollection(),"window_cascade");

  fileNew->setStatusText(i18n("Creates a new document"));
  fileOpen->setStatusText(i18n("Opens an existing document"));
  fileOpenRecent->setStatusText(i18n("Opens a recently used file"));
  fileSave->setStatusText(i18n("Saves the actual document"));
  fileSaveAs->setStatusText(i18n("Saves the actual document as..."));
  fileClose->setStatusText(i18n("Closes the actual document"));
  filePrint ->setStatusText(i18n("Prints out the actual document"));
  fileQuit->setStatusText(i18n("Quits the application"));

  editCut->setStatusText(i18n("Cuts the selected section and puts it to the clipboard"));
  editCopy->setStatusText(i18n("Copies the selected section to the clipboard"));
  editPaste->setStatusText(i18n("Pastes the clipboard contents to actual position"));

  viewToolBar->setStatusText(i18n("Enables/disables the toolbar"));
  viewStatusBar->setStatusText(i18n("Enables/disables the statusbar"));

  windowMenu = new KActionMenu(i18n("&Window"), actionCollection(), "window_menu");
  connect(windowMenu->popupMenu(), SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));

  createGUI();

}


void KDevVLSIApp::initStatusBar()
{
  ///////////////////////////////////////////////////////////////////
  // STATUSBAR
  // TODO: add your own items you need for displaying current application status.
  statusBar()->insertItem(i18n("Ready."),1);
}


void KDevVLSIApp::initView()
{ 
  ////////////////////////////////////////////////////////////////////
  // here the main view of the KMainWindow is created by a background box and
  // the QWorkspace instance for MDI view.
  QVBox* view_back = new QVBox( this );
  view_back->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
  pWorkspace = new QWorkspace( view_back );
  connect(pWorkspace, SIGNAL(windowActivated(QWidget*)), this, SLOT(setWndTitle(QWidget*)));
  setCentralWidget(view_back);
}

void KDevVLSIApp::setWndTitle(QWidget*){
 setCaption(pWorkspace->activeWindow()->caption());
}


void KDevVLSIApp::createClient(KDevVLSIDoc* doc)
{
  KDevVLSIView* w = new KDevVLSIView(doc, pWorkspace,0,WDestructiveClose);
  w->installEventFilter(this);
  doc->addView(w);
  w->setIcon(kapp->miniIcon());
  if ( pWorkspace->windowList().isEmpty() ) // show the very first window in maximized mode
    w->showMaximized();
  else
    w->show();
}

void KDevVLSIApp::openDocumentFile(const KURL& url)
{
  slotStatusMsg(i18n("Opening file..."));
  KDevVLSIDoc* doc;
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

  slotStatusMsg(i18n("Ready."));
}


void KDevVLSIApp::saveOptions()
{	
  config->setGroup("General Options");
  config->writeEntry("Geometry", size());
  config->writeEntry("Show Toolbar", toolBar()->isVisible());
  config->writeEntry("Show Statusbar",statusBar()->isVisible());
  config->writeEntry("ToolBarPos", (int) toolBar("mainToolBar")->barPos());
  fileOpenRecent->saveEntries(config,"Recent Files");
}


void KDevVLSIApp::readOptions()
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

  QSize size=config->readSizeEntry("Geometry");
  if(!size.isEmpty())
  {
    resize(size);
  }
}

void KDevVLSIApp::saveProperties(KConfig *_cfg)
{

}


void KDevVLSIApp::readProperties(KConfig* _cfg)
{
}

bool KDevVLSIApp::queryClose()
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

  switch (KMessageBox::questionYesNoList(this,
          i18n("One or more documents have been modified.\nSave changes before exiting?"),saveFiles))
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
          KMessageBox::error (this,i18n("Could not save the current document !"), i18n("I/O Error !"));
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

bool KDevVLSIApp::queryExit()
{
  saveOptions();
  return true;
}

bool KDevVLSIApp::eventFilter(QObject* object, QEvent* event)
{
  if(event->type() == QEvent::Close)
  {
    QCloseEvent* e=(QCloseEvent*)event;

    KDevVLSIView* pView=(KDevVLSIView*)object;
    KDevVLSIDoc* pDoc=pView->getDocument();
    if(pDoc->canCloseFrame(pView))
    {
      pDoc->removeView(pView);
      if(!pDoc->firstView())
        pDocList->remove(pDoc);
      e->accept();
      //////////////	
      if(pWorkspace->windowList().count()<1)
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

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////


void KDevVLSIApp::slotFileNew()
{
  slotStatusMsg(i18n("Creating new document..."));

  openDocumentFile();

  slotStatusMsg(i18n("Ready."));
}

void KDevVLSIApp::slotFileOpen()
{
  slotStatusMsg(i18n("Opening file..."));
	
  KURL url=KFileDialog::getOpenURL(QString::null,
      i18n("*|All files"), this, i18n("Open File..."));
  if(!url.isEmpty())
  {
    openDocumentFile(url);
    fileOpenRecent->addURL( url );
  }

  slotStatusMsg(i18n("Ready."));
}

void KDevVLSIApp::slotFileOpenRecent(const KURL& url)
{
  slotStatusMsg(i18n("Opening file..."));
  	
  openDocumentFile(url);
	
  slotStatusMsg(i18n("Ready."));
}

void KDevVLSIApp::slotFileSave()
{
  slotStatusMsg(i18n("Saving file..."));

  KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
  if( m )
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

void KDevVLSIApp::slotFileSaveAs()
{
  slotStatusMsg(i18n("Saving file with a new filename..."));

  KURL url=KFileDialog::getSaveURL(QDir::currentDirPath(),
        i18n("*|All files"), this, i18n("Save as..."));
  if(!url.isEmpty())
  {
    KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
    if( m )
    {
      KDevVLSIDoc* doc =	m->getDocument();
      if(!doc->saveDocument(url))
      {
        KMessageBox::error (this,i18n("Could not save the current document !"), i18n("I/O Error !"));
        return;
      }
      doc->changedViewList();
//      setWndTitle(m);
	    fileOpenRecent->addURL(url);
    }	
  }

  slotStatusMsg(i18n("Ready."));
}

void KDevVLSIApp::slotFileClose()
{
  slotStatusMsg(i18n("Closing file..."));
  KDevVLSIView* m = (KDevVLSIView*)pWorkspace->activeWindow();
  if( m )
  {
    KDevVLSIDoc* doc=m->getDocument();
    doc->closeDocument();
  }
	
  slotStatusMsg(i18n("Ready."));
}

void KDevVLSIApp::slotFilePrint()
{
  slotStatusMsg(i18n("Printing..."));
	
  KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
  if ( m )
    m->print( printer );

  slotStatusMsg(i18n("Ready."));
}

void KDevVLSIApp::slotFileQuit()
{
  slotStatusMsg(i18n("Exiting..."));
  saveOptions();
  // close the first window, the list makes the next one the first again.
  // This ensures that queryClose() is called on each window to ask for closing
  KMainWindow* w;
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

void KDevVLSIApp::slotEditUndo()
{
  slotStatusMsg(i18n("Reverting last action..."));
	
  KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
  if ( m )
//  m->undo();

  slotStatusMsg(i18n("Ready."));
}

void KDevVLSIApp::slotEditCut()
{
  slotStatusMsg(i18n("Cutting selection..."));
	
  KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
  if ( m )
//    m->cut();	

  slotStatusMsg(i18n("Ready."));
}

void KDevVLSIApp::slotEditCopy()
{
  slotStatusMsg(i18n("Copying selection to clipboard..."));
	
  KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
  if ( m )
//    m->copy();
		
  slotStatusMsg(i18n("Ready."));
}

void KDevVLSIApp::slotEditPaste()
{
  slotStatusMsg(i18n("Inserting clipboard contents..."));
	
  KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
  if ( m )
//    m->paste();
		
  slotStatusMsg(i18n("Ready."));
}

void KDevVLSIApp::slotViewToolBar()
{
  slotStatusMsg(i18n("Toggle the toolbar..."));
  ///////////////////////////////////////////////////////////////////
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

void KDevVLSIApp::slotViewStatusBar()
{
  slotStatusMsg(i18n("Toggle the statusbar..."));
  ///////////////////////////////////////////////////////////////////
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

void KDevVLSIApp::slotWindowNewWindow()
{
  slotStatusMsg(i18n("Opening a new application window..."));

  KDevVLSIView* m = (KDevVLSIView*) pWorkspace->activeWindow();
  if ( m )
  {
    KDevVLSIDoc* doc = m->getDocument();
    createClient(doc);
  }

  slotStatusMsg(i18n("Ready."));
}

void KDevVLSIApp::slotWindowTile(){
  pWorkspace->tile();
}

void KDevVLSIApp::slotWindowCascade(){
  pWorkspace->cascade();
}

void KDevVLSIApp::slotStatusMsg(const QString &text)
{
  ///////////////////////////////////////////////////////////////////
  // change status message permanently
  statusBar()->clear();
  statusBar()->changeItem(text,1);
}


void KDevVLSIApp::windowMenuAboutToShow()
{
  windowMenu->popupMenu()->clear();
  windowMenu->insert(windowNewWindow);
  windowMenu->insert(windowCascade);
  windowMenu->insert(windowTile);

  if ( pWorkspace->windowList().isEmpty() ){
    windowNewWindow->setEnabled(false);
    windowCascade->setEnabled(false);
    windowTile->setEnabled(false);
  }
  else{
    windowNewWindow->setEnabled(true);
    windowCascade->setEnabled(true);
    windowTile->setEnabled(true);
  }
  windowMenu->popupMenu()->insertSeparator();

  QWidgetList windows = pWorkspace->windowList();
  for ( int i = 0; i < int(windows.count()); ++i )
  {
    int id = windowMenu->popupMenu()->insertItem(QString("&%1 ").arg(i+1)+windows.at(i)->caption(), this, SLOT( windowMenuActivated( int ) ) );
    windowMenu->popupMenu()->setItemParameter( id, i );
    windowMenu->popupMenu()->setItemChecked( id, pWorkspace->activeWindow() == windows.at(i) );
  }
}

void KDevVLSIApp::windowMenuActivated( int id )
{
  QWidget* w = pWorkspace->windowList().at( id );
  if ( w )
    w->setFocus();
}

