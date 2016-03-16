/*

	RVLSI Project Library

	KDevVLSI.cpp

	Main Window - Implementation.

	Copyright 2000-2016 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rtextfile.h>


//-----------------------------------------------------------------------------
// include files for R
#include <qraboutdialog.h>
#include <rchromovlsi.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <QMessageBox>
#include <QFileDialog>


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
KDevVLSI::KDevVLSI(QVLSIApp* app)
	: QMainWindow(0), App(app), Doc(0)
{
	setupUi(this);
	connectMenus();
	fileOpened();
	setWindowTitle("KDevVLSI");
}


//-----------------------------------------------------------------------------
void KDevVLSI::connectMenus(void)
{

	// Menu 'File'
	connect(aOpen,SIGNAL(triggered()),this,SLOT(openFile()));
	connect(aImport,SIGNAL(triggered()),this,SLOT(importProject()));
	connect(aExport,SIGNAL(triggered()),this,SLOT(exportResults()));
	connect(aOptions,SIGNAL(triggered()),this,SLOT(optionsPreferences()));
	connect(aExit,SIGNAL(triggered()),this,SLOT(applicationQuit()));

	// Recent files
	for(int i=0;i<MaxRecentFiles;++i)
	{
		recentFiles[i] = new QAction(this);
		if(App->LastOpenedFiles.GetNb()>i)
		{
			recentFiles[i]->setText(ToQString(*App->LastOpenedFiles[i]));
			recentFiles[i]->setVisible(true);
			QVariant variant;
			variant.setValue(App->LastOpenedFiles[i]);
			recentFiles[i]->setData(variant);
		}
		else
			recentFiles[i]->setVisible(false);
      connect(recentFiles[i], SIGNAL(triggered()),this, SLOT(openRecentFile()));
   }
	//menuFile->addSeparator();
   for (int i = 0; i < MaxRecentFiles; ++i)
        menuOpenRecent->addAction(recentFiles[i]);

	// Menu "Heuristic"
	connect(aBottomLeft,SIGNAL(triggered()),this,SLOT(heuristicBL()));
	connect(aEdge,SIGNAL(triggered()),this,SLOT(heuristicEdge()));
	connect(aCenter,SIGNAL(triggered()),this,SLOT(heuristicCenter()));
	connect(aRunHeuristic,SIGNAL(triggered()),this,SLOT(heuristicRun()));
	connect(aNextStep,SIGNAL(triggered()),this,SLOT(heuristicNext()));
	connect(aSelectObjects,SIGNAL(triggered()),this,SLOT(heuristicSelect()));

	// Menu "GA"
	connect(aInitialise,SIGNAL(triggered()),this,SLOT(GAInit()));
	connect(aStart,SIGNAL(triggered()),this,SLOT(GAStart()));
	connect(aPause,SIGNAL(triggered()),this,SLOT(GAPause()));
	connect(aStop,SIGNAL(triggered()),this,SLOT(GAStop()));

	// Menu 'Window'
	connect(aCloseAll,SIGNAL(triggered()),Desktop,SLOT(closeAllSubWindows()));
	connect(aTile,SIGNAL(triggered()),Desktop,SLOT(tileSubWindows()));
	connect(aCascade,SIGNAL(triggered()),Desktop,SLOT(cascadeSubWindows()));

	// Menu "Help"
	//connect(aViewPolygons,SIGNAL(triggered()),this,SLOT(viewPolygons()));
	connect(aAbout,SIGNAL(triggered()),this,SLOT(about()));
}


//-----------------------------------------------------------------------------
void KDevVLSI::fileOpened(void)
{
	bool opened(Doc!=0);
	aImport->setEnabled(!opened);
	aOptions->setEnabled(opened);
	aBottomLeft->setEnabled(opened);
	aEdge->setEnabled(opened);
	aCenter->setEnabled(opened);
	aRunHeuristic->setEnabled(opened);
	aNextStep->setEnabled(opened);
	aSelectObjects->setEnabled(opened);
	aInitialise->setEnabled(opened);
	aStart->setEnabled(opened);
	aPause->setEnabled(opened);
	aStop->setEnabled(opened);
	aCloseAll->setEnabled(opened);
	aTile->setEnabled(opened);
	aCascade->setEnabled(opened);
	aViewPolygons->setEnabled(opened);
	aExport->setEnabled(opened);
}


//-----------------------------------------------------------------------------
void KDevVLSI::statusMsg(const QString& text)
{
	statusBar()->showMessage(text);
}


//-----------------------------------------------------------------------------
void KDevVLSI::openDocumentFile(const RString& url)
{
	bool DestroyDoc(false);

	statusMsg(tr("Opening file..."));

	// check, if document already open.
	closeFile();

	// Create the document
	QApplication::setOverrideCursor(Qt::WaitCursor);
	try
	{
		DestroyDoc=true;
		Doc=App->CreateSession(url);
		KProject* ptr(new KProject(Doc,url));
		DestroyDoc=false;
		Desktop->addSubWindow(ptr);
		ptr->adjustSize();
		ptr->show();
		updateRecentFileActions(url);
		fileOpened();
		statusMsg(tr("Connected"));
		QApplication::setOverrideCursor(Qt::ArrowCursor);
	}
	catch(RException& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("R exception"),ToQString(e.GetMsg()),QMessageBox::Ok);
	}
	catch(std::exception& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("std::exception"),QWidget::trUtf8(e.what()),QMessageBox::Ok);
	}
	catch(...)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("Unknown exception"),QWidget::trUtf8("Unknown problem"),QMessageBox::Ok);
	}
	if(DestroyDoc)
	{
		delete Doc;
		Doc=0;
		App->DeleteSession();
	}
	statusMsg(tr("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSI::updateRecentFileActions(const RString& url)
{
	if(App->LastOpenedFiles.IsIn(url))
		return;

	// Update the list
	if(App->LastOpenedFiles.GetNb()==MaxRecentFiles)
		App->LastOpenedFiles.DeletePtrAt(MaxRecentFiles-1);
	App->LastOpenedFiles.InsertPtrAt(new RString(url),0,false);

	// Update the menu
	RCursor<RString> Cur(App->LastOpenedFiles);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		recentFiles[Cur.GetPos()]->setText(ToQString(*Cur()));
		QVariant variant;
		variant.setValue(Cur());
      recentFiles[Cur.GetPos()]->setData(variant);
      recentFiles[Cur.GetPos()]->setVisible(true);
	}
}

//-----------------------------------------------------------------------------
void KDevVLSI::applicationQuit(void)
{
	statusMsg(tr("Exiting..."));
	closeFile();
	statusMsg(tr("Ready."));
	close();
}


//-----------------------------------------------------------------------------
void KDevVLSI::openFile(void)
{
	statusMsg(tr("Opening file..."));

	QString url(QFileDialog::getOpenFileName(this,tr("Open Project"),QDir::homePath(), tr("2D Problem files (*.pl2d)")));
	if(url.isEmpty())
	{
		statusMsg(tr("Ready."));
		return;
	}
	openDocumentFile(FromQString(url));
	statusMsg(tr("Ready."));
}


////-----------------------------------------------------------------------------
void KDevVLSI::openRecentFile(void)
{
	statusMsg(tr("Opening file..."));
   QAction *action = qobject_cast<QAction *>(sender());
   if(action)
       openDocumentFile(*action->data().value<RString*>());
	statusMsg(tr("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSI::closeFile(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	if(Doc)
	{
		Desktop->closeAllSubWindows();
		App->DeleteSession();
		Doc=0;
		fileOpened();
		statusMsg(tr("No VLSI !"));
	}
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void KDevVLSI::heuristicBL(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	KHeuristic* ptr(new KHeuristic(Doc,"Bottom-Left"));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
	ptr->RunHeuristic();
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void KDevVLSI::heuristicEdge(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	KHeuristic* ptr(new KHeuristic(Doc,"Edge"));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
	ptr->RunHeuristic();
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void KDevVLSI::heuristicCenter(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	KHeuristic* ptr(new KHeuristic(Doc,"Center"));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
	ptr->RunHeuristic();
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void KDevVLSI::heuristicNext(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	KHeuristic* Win(dynamic_cast<KHeuristic*>(Desktop->activeSubWindow()));
	if(Win)
		Win->NextStep();
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void KDevVLSI::heuristicRun(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	KHeuristic* Win(dynamic_cast<KHeuristic*>(Desktop->activeSubWindow()));
	if(Win)
		Win->RunToEnd();
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void KDevVLSI::heuristicSelect(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	KHeuristic* Win(dynamic_cast<KHeuristic*>(Desktop->activeSubWindow()));
	if(Win)
		Win->SelectObjects();
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void KDevVLSI::GAInit(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	KGAView* ptr(new KGAView());
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void KDevVLSI::GAStart(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	KGAView* Win(dynamic_cast<KGAView*>(Desktop->activeSubWindow()));
	if(Win)
		Win->RunGA();
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void KDevVLSI::GAPause(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	KGAView* Win(dynamic_cast<KGAView*>(Desktop->activeSubWindow()));
	if(Win)
		Win->PauseGA();
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void KDevVLSI::GAStop(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	KGAView* Win(dynamic_cast<KGAView*>(Desktop->activeSubWindow()));
	if(Win)
		Win->StopGA();
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void KDevVLSI::viewPolygons(void)
{
	QString Url(QFileDialog::getOpenFileName(this,tr("Open Polygon File"),QDir::homePath(), tr("*.*")));
	if(Url.isEmpty())
		return;
	try
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		KPolygonsView* ptr(new KPolygonsView());
		Desktop->addSubWindow(ptr);
		ptr->adjustSize();
		ptr->show();
		ptr->Load(Url);
		QApplication::setOverrideCursor(Qt::ArrowCursor);
	}
	catch(RException& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("R exception"),ToQString(e.GetMsg()),QMessageBox::Ok);
	}
	catch(std::exception& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("std::exception"),QWidget::trUtf8(e.what()),QMessageBox::Ok);
	}
	catch(...)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("Unknown exception"),QWidget::trUtf8("Unknown problem"),QMessageBox::Ok);
	}
}


//-----------------------------------------------------------------------------
void KDevVLSI::optionsPreferences(void)
{
	statusMsg(tr("Set the options..."));

	KAppOptions Dlg(this);
	Dlg.exec(App);
	statusMsg(tr("Ready."));
}


//-----------------------------------------------------------------------------
void KDevVLSI::importProject(void)
{
	statusMsg(tr("Opening file..."));
	QString url(QFileDialog::getOpenFileName(this,tr("Import"),QDir::homePath(), tr("*.vlsiprj|VLSI Project")));
	if(url.isEmpty())
	{
		statusMsg(tr("Ready."));
		return;
	}
	try
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		RProject Project(FromQString(url));
		Project.Analyse();
		QApplication::setOverrideCursor(Qt::ArrowCursor);
	}
	catch(RException& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("R exception"),ToQString(e.GetMsg()),QMessageBox::Ok);
	}
	catch(std::exception& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("std::exception"),QWidget::trUtf8(e.what()),QMessageBox::Ok);
	}
	catch(...)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("Unknown exception"),QWidget::trUtf8("Unknown problem"),QMessageBox::Ok);
	}
	statusMsg(tr("Ready."));
}


//-----------------------------------------------------------------------------
RString Cast(double d)
{
	return(RString::Number(static_cast<long>(d)));
}


//-----------------------------------------------------------------------------
void KDevVLSI::exportResults(void)
{
	// Look if the window is a result one

	QMdiSubWindow* Cur(Desktop->currentSubWindow());
	if(!Cur)
		return;

	// Find the layout to export
	RLayout* Layout(0);
	if(dynamic_cast<KHeuristic*>(Cur))
		Layout=dynamic_cast<KHeuristic*>(Cur)->PlacementHeuristic->GetLayout();
	else if(dynamic_cast<KGAView*>(Cur))
		Layout=dynamic_cast<KGAView*>(Cur)->Instance->GetBestChromosome();
	if(!Layout)
		return;

	// Export the file
	statusMsg(tr("Export current results..."));
	QString url(QFileDialog::getSaveFileName(this, tr("Export Results"),QDir::homePath()));
	if(!url.isEmpty())
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		RTextFile Out(FromQString(url),"utf8");
		Out.Open(RIO::Create);
		Out<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"<<endl;
		Out<<"<!DOCTYPE vlsiResults>"<<endl;
		Out<<"<vlsiResults xmlns:svg=\"http://www.w3.org/2000/svg\">"<<endl;
		Out<<"\t<spaceOccupied>"<<endl;
		RRect Bound(Layout->ComputeBoundary());
		Out<<"\t\t<svg:polygon svg:points=\""+Cast(Bound.GetWidth())+","+Cast(Bound.GetWidth())+"\"/>"<<endl;
		Out<<"\t</spaceOccupied>"<<endl;
		Out<<"\t<instances>"<<endl;
		RCursor<RGeoInfo> Info(Layout->GetInfos());
		for(Info.Start();!Info.End();Info.Next())
			Out<<"\t\t<instance id=\""+RString::Number(Info()->GetObj()->GetId())+"\" svg:points=\""+
					Cast(Info()->GetPos().X)+","+
				   Cast(Info()->GetPos().Y)+"\"/>"<<endl;
		Out<<"\t<instances>"<<endl;
		Out<<"</vlsiResults>"<<endl;
	}
	statusMsg(tr("Ready."));
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void KDevVLSI::about(void)
{
	QRAboutDialog dlg("KDevVLSI","2.0");
	dlg.setDescription("Qt-based 2D-GA frontend.");
	dlg.setCopyright(QWidget::trUtf8("(C) 2001-2008 by the Université Libre de Bruxelles (ULB)<br/>(C) 2010-2016 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.exec();
}


//-----------------------------------------------------------------------------
KDevVLSI::~KDevVLSI(void)
{
}
