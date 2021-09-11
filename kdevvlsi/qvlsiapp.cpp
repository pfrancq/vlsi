/*

	RVLSI Project Library

	QVLSIApp.cpp

	Main Application - Implementation.

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
// include files for Qt
#include <QTranslator>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QLocale>


//-----------------------------------------------------------------------------
// include files
#include <qvlsiapp.h>
#include <kdevvlsi.h>


//-----------------------------------------------------------------------------
//
// class QVLSIApp
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QVLSIApp::QVLSIApp(int argc, char *argv[])
	: RVLSIApp("KDevVLSI",argc,argv), Main(0), LastOpenedFiles(KDevVLSI::MaxRecentFiles)
{
	// Qt translation for default dialogs (QFileDialog) and so on
	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(),
	QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	QApplication::instance()->installTranslator(&qtTranslator);
}


//-----------------------------------------------------------------------------
void QVLSIApp::CreateConfig(void)
{
	RVLSIApp::CreateConfig();
	Config.InsertParam(new RParamValue("MainWindowWidth",300));
	Config.InsertParam(new RParamValue("MainWindowHeight",300));
	Config.InsertParam(new RParamList("LastOpenedFiles"));
}


//-----------------------------------------------------------------------------
void QVLSIApp::Init(void)
{
	RVLSIApp::Init();
	RParamList* List(Config.FindParam<RParamList>("LastOpenedFiles"));
	RCursor<RString> Cur(List->GetList());
	for(Cur.Start();!Cur.End();Cur.Next())
		LastOpenedFiles.InsertPtr(new RString(*Cur()));
}


//-----------------------------------------------------------------------------
void QVLSIApp::Run(void)
{
 	Main=new KDevVLSI(this);
	Main->resize(Config.GetInt("MainWindowWidth"),Config.GetInt("MainWindowHeight"));
	Main->show();
	QApplication::instance()->exec();
}


//-----------------------------------------------------------------------------
QVLSIApp::~QVLSIApp(void)
{
	RParamList* List(Config.FindParam<RParamList>("LastOpenedFiles"));
	List->Reset();
	RCursor<RString> Cur(LastOpenedFiles);
	for(Cur.Start();!Cur.End();Cur.Next())
		List->Insert(*Cur());

	if(Main)
	{
		Config.SetInt("MainWindowWidth",Main->width());
		Config.SetInt("MainWindowHeight",Main->height());
		delete Main;
		Main=0;
	}
}