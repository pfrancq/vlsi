/*

	RVLSI Project Library

	KDevVLSI.cpp

	Main Window - Implementation.

	Copyright 2000-2014 by Pascal Francq (pascal@francq.info).

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
// include files for R/RVLSI Project
#include <rqt.h>
#include <files.h>


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <kstatusbar.h>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <KDE/KLocale>
#include <kapplication.h>
#include <KDE/KConfigGroup>
#include <kmessagebox.h>
#include <kfiledialog.h>


//-----------------------------------------------------------------------------
// application specific includes
#include <kdevvlsi.h>
#include <kappoptions.h>
#include <kproject.h>
#include <kheuristic.h>
#include <kgaview.h>
#include <kpolygonsview.h>



//-----------------------------------------------------------------------------
//
// class KDevVLSI
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KDevVLSI::KDevVLSI(int argc, char *argv[])
	: KXmlGuiWindow(0), RVLSIApp("KDevVLSI",argc,argv),
	  Desktop(new QMdiArea(this)), Status(new QLabel(statusBar())), Doc(0)
{
   setAcceptDrops(true);
   setCentralWidget(Desktop);
   initActions();
	Status->setPixmap(KIconLoader::global()->loadIcon("project-development-close",KIconLoader::Small));
	statusBar()->insertWidget(0,Status);
	statusBar()->insertItem(i18n("Ready."),1);
   statusBar()->show();
   setupGUI();
   Init();
   readOptions();
	fileOpened(false);
}


//-----------------------------------------------------------------------------
KAction* KDevVLSI::addAction(const char* title,const char* name,const char* slot,const char* icon,const char* key)
{
	KAction* ptr(new KAction(i18n(title),this));
	if(icon)
		ptr->setIcon(KIcon(icon));
	if(key)
		ptr->setShortcut(QKeySequence(tr(key)));
	actionCollection()->addAction(QLatin1String(name),ptr);
	connect(ptr,SIGNAL(triggered(bool)),this,slot);
	return(ptr);
}


//-----------------------------------------------------------------------------
void KDevVLSI::initActions(void)
{
	// Menu "file"
	aFileOpen= KStandardAction::open(this, SLOT(openFile()), actionCollection());
	aFileOpenRecent = KStandardAction::openRecent(this, SLOT(openRecentFile(const KUrl&)), actionCollection());
	Actions.insert(Actions.size(),KStandardAction::close(this, SLOT(closeFile()), actionCollection()));
	KStandardAction::quit(this, SLOT(applicationQuit()), actionCollection());
	aImport=addAction("&Import VLSI Project...","importProject",SLOT(importProject()),0,0);

	// Menu "Window"
	KAction* windowCloseAll(new KAction(i18n("&Close All"),this));
	actionCollection()->addAction(QLatin1String("window_closeall"),windowCloseAll);
	connect(windowCloseAll,SIGNAL(triggered(bool)),Desktop,SLOT(closeAllSubWindows()));
	KAction* windowTile(new KAction(i18n("&Tile"),this));
	actionCollection()->addAction(QLatin1String("window_tile"),windowTile);
	connect(windowTile,SIGNAL(triggered(bool)),Desktop,SLOT(tileSubWindows()));
	KAction* windowCascade(new KAction(i18n("&Cascade"),this));
	actionCollection()->addAction(QLatin1String("window_cascade"),windowCascade);
	connect(windowCascade,SIGNAL(triggered(bool)),Desktop,SLOT(cascadeSubWindows()));

	// Menu "Heuristic"
	Actions.insert(Actions.size(),addAction("&Bottom-Left Heuristic","heuristic_bl",SLOT(heuristicBL()),0,"ALT+B"));
	Actions.insert(Actions.size(),addAction("&Edge Heuristic","heuristic_edge",SLOT(heuristicEdge()),0,"ALT+E"));
	Actions.insert(Actions.size(),addAction("&Center Heuristic","heuristic_center",SLOT(heuristicCenter()),0,"ALT+C"));
	Actions.insert(Actions.size(),addAction("&Run Heuristic","heuristic_run",SLOT(heuristicRun()),"run","ALT+R"));
	Actions.insert(Actions.size(),addAction("&Next step for Heuristic","heuristic_next",SLOT(heuristicNext()),"next","ALT+N"));
	Actions.insert(Actions.size(),addAction("&Select objects of Heuristic","heuristic_select",SLOT(heuristicSelect()),"queue","ALT+O"));

	// Menu "GA"
	Actions.insert(Actions.size(),addAction("&Initialize","ga_init",SLOT(GAInit()),"reload","ALT+I"));
	Actions.insert(Actions.size(),addAction("&Start","ga_start",SLOT(GAStart()),"exec","ALT+S"));
	Actions.insert(Actions.size(),addAction("&Pause","ga_pause",SLOT(GAPause()),"player_pause","ALT+P"));
	Actions.insert(Actions.size(),addAction("&Stop","ga_stop",SLOT(GAStop()),"stop","ALT+T"));

	// Menu "Tools"
	addAction("View &Polygons","tools_view_polygons",SLOT(viewPolygons()),"viewmag",0);

	// Menu Settings
	KStandardAction::preferences(this,SLOT(optionsPreferences()),actionCollection());
}


//-----------------------------------------------------------------------------
void KDevVLSI::fileOpened(bool opened)
{
	aFileOpen->setEnabled(!opened);
	aFileOpenRecent->setEnabled(!opened);
	aImport->setEnabled(!opened);
	for(int i=0;i<Actions.count();i++)
		Actions.at(i)->setEnabled(opened);
}


//-----------------------------------------------------------------------------
void KDevVLSI::saveOptions(void)
{
	KConfig Config;
	KConfigGroup General(&Config,"KDevVLSI");
	KAppOptions::saveOptions();
	aFileOpenRecent->saveEntries(General);
}


//-----------------------------------------------------------------------------
void KDevVLSI::readOptions(void)
{
	KConfig Config;
	KConfigGroup General(&Config,"KDevVLSI");
	KAppOptions::readOptions();
	aFileOpenRecent->loadEntries(General);
}


//-----------------------------------------------------------------------------
void KDevVLSI::statusMsg(const QString& text)
{
	statusBar()->changeItem(text,1);
}


//-----------------------------------------------------------------------------
void KDevVLSI::openDocumentFile(const KUrl& url)
{
	bool DestroyDoc(false);

	statusMsg(i18n("Opening file..."));

	// check, if document already open.
	if(Doc)
		mThrowRException("Big Problem : No VLSI session should exist");

	// Create the document
	try
	{
		DestroyDoc=true;
		Doc=CreateSession(FromQString(url.path()));
		KProject* ptr(new KProject(Doc,url.path()));
		DestroyDoc=false;
		Desktop->addSubWindow(ptr);
		ptr->adjustSize();
		ptr->show();
		aFileOpenRecent->addUrl(url);
		fileOpened(true);
		Status->setPixmap(QPixmap(KIconLoader::global()->loadIcon("project-open",KIconLoader::Small)));
		statusMsg(i18n("Connected"));
	}
	catch(RException& e)
	{
		KMessageBox::error(this,ToQString(e.GetMsg()),"R Exception");
	}
	catch(std::exception& e)
	{
		KMessageBox::error(this,e.what(),"std::exception");
	}
	catch(...)
	{
		KMessageBox::error(this,"Undefined Error");
	}
	if(DestroyDoc)
	{
		delete Doc;
		Doc=0;
		DeleteSession();
	}
	statusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSI::applicationQuit(void)
{
	statusMsg(i18n("Exiting..."));
	saveOptions();
	closeFile();
	statusMsg(i18n("Ready."));
	close();
}


//-----------------------------------------------------------------------------
void KDevVLSI::openFile(void)
{
	statusMsg(i18n("Opening file..."));
	KUrl url(KFileDialog::getOpenFileName(KUrl("~"),"*.pl2d|2D Placement files",Desktop,"Open File..."));
	if(url.isEmpty())
		QMessageBox::critical(this,"KDevVLSI","File could not be found");
	else
	{
		openDocumentFile(url);
		aFileOpenRecent->addUrl(url);
	}
	statusMsg(i18n("Ready."));
}


////-----------------------------------------------------------------------------
void KDevVLSI::openRecentFile(const KUrl& url)
{
	statusMsg(i18n("Opening file..."));
	openDocumentFile(url);
	statusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSI::closeFile(void)
{
	if(Doc)
	{
		Desktop->closeAllSubWindows();
		DeleteSession();
		Doc=0;
		fileOpened(false);
		statusMsg(i18n("No VLSI !"));
		Status->setPixmap(KIconLoader::global()->loadIcon("project-development-close",KIconLoader::Small));
	}
}


//-----------------------------------------------------------------------------
void KDevVLSI::heuristicBL(void)
{
	KHeuristic* ptr(new KHeuristic(Doc,"Bottom-Left"));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
	ptr->RunHeuristic();
}


//-----------------------------------------------------------------------------
void KDevVLSI::heuristicEdge(void)
{
	KHeuristic* ptr(new KHeuristic(Doc,"Edge"));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
	ptr->RunHeuristic();
}


//-----------------------------------------------------------------------------
void KDevVLSI::heuristicCenter(void)
{
	KHeuristic* ptr(new KHeuristic(Doc,"Center"));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
	ptr->RunHeuristic();
}


//-----------------------------------------------------------------------------
void KDevVLSI::heuristicNext(void)
{
	KHeuristic* Win(dynamic_cast<KHeuristic*>(Desktop->activeSubWindow()));
	if(Win)
		Win->NextStep();
}


//-----------------------------------------------------------------------------
void KDevVLSI::heuristicRun(void)
{
	KHeuristic* Win(dynamic_cast<KHeuristic*>(Desktop->activeSubWindow()));
	if(Win)
		Win->RunToEnd();
}


//-----------------------------------------------------------------------------
void KDevVLSI::heuristicSelect(void)
{
	KHeuristic* Win(dynamic_cast<KHeuristic*>(Desktop->activeSubWindow()));
	if(Win)
		Win->SelectObjects();
}


//-----------------------------------------------------------------------------
void KDevVLSI::GAInit(void)
{
	KGAView* ptr(new KGAView());
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void KDevVLSI::GAStart(void)
{
	KGAView* Win(dynamic_cast<KGAView*>(Desktop->activeSubWindow()));
	if(Win)
		Win->RunGA();
}


//-----------------------------------------------------------------------------
void KDevVLSI::GAPause(void)
{
	KGAView* Win(dynamic_cast<KGAView*>(Desktop->activeSubWindow()));
	if(Win)
		Win->PauseGA();
}


//-----------------------------------------------------------------------------
void KDevVLSI::GAStop(void)
{
	KGAView* Win(dynamic_cast<KGAView*>(Desktop->activeSubWindow()));
	if(Win)
		Win->StopGA();
}


//-----------------------------------------------------------------------------
void KDevVLSI::viewPolygons(void)
{
	KUrl Url(KFileDialog::getOpenFileName(KUrl("~"),QString::null,Desktop,"Open Polygon File..."));
	if(Url.isEmpty())
		return;
	try
	{
		KPolygonsView* ptr(new KPolygonsView());
		Desktop->addSubWindow(ptr);
		ptr->adjustSize();
		ptr->show();
		ptr->Load(Url);
	}
	catch(RException& e)
	{
		KMessageBox::error(this,ToQString(e.GetMsg()),"R Exception");
	}
	catch(std::exception& e)
	{
		KMessageBox::error(this,e.what(),"std::exception");
	}
	catch(...)
	{
		KMessageBox::error(this,"Undefined Error");
	}
}


//-----------------------------------------------------------------------------
void KDevVLSI::optionsPreferences(void)
{
	statusMsg(i18n("Set the options..."));

	KAppOptions Dlg(this);
	Dlg.exec(this);
	statusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSI::importProject(void)
{
	statusMsg(i18n("Opening file..."));
	KUrl url(KFileDialog::getOpenFileName(KUrl("~"),"*.vlsiprj|VLSI Project",Desktop,"Open File..."));
	//KUrl url("/home/pfrancq/Documents/data/vlsi/example.vlsiprj");
	if(url.isEmpty())
		QMessageBox::critical(this,"KDevVLSI","File could not be found");
	else
	{
		try
		{
			RProject Project(FromQString(url.path()));
			Project.Analyse();
		}
		catch(RException& e)
		{
			KMessageBox::error(this,ToQString(e.GetMsg()),"R Exception");
		}
		catch(std::exception& e)
		{
			KMessageBox::error(this,e.what(),"std::exception");
		}
		catch(...)
		{
			KMessageBox::error(this,"Undefined Error");
		}
	}
	statusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
KDevVLSI::~KDevVLSI(void)
{
	delete Status;
}
