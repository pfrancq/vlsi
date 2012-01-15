/*

	RVLSI Project Library

	Main.cpp

	Main program - Implementation.

	Copyright 2000-2012 by Pascal Francq (pascal@francq.info).

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
// include files for ANSI C/C++
#include <stdexcept>
#include <iostream>
using namespace std;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>


//-----------------------------------------------------------------------------
// include file for current application
#include <kdevvlsi.h>


//-----------------------------------------------------------------------------
// Description of the application
static const char *description =
	"Development Application for VLSI Placement.";


//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE,"");

    // Information about the application
	KAboutData aboutData("kdevvlsi",0,ki18n("KDevVLSI"),"1.89",ki18n(description),
			KAboutData::License_GPL,ki18n("(C) 2000-2012 by Pascal Francq\n"),
			KLocalizedString(),"http://www.otlet-institute.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Project Manager"),"pascal@francq.info");

	// Init
	KCmdLineArgs::init( argc, argv, &aboutData );
    KCmdLineOptions options;
	KCmdLineArgs::addCmdLineOptions(options);

	// Run
	try
	{
		KApplication app;
		KDevVLSI* Center=new KDevVLSI(argc,argv);
		if(app.isSessionRestored())
		{
//			RESTORE(Center);
		}
		else
		{
			Center->show();
		}
		return(app.exec());
	}
	catch(RException& e)
	{
		cout<<e.GetMsg()<<endl;
	}
	catch(exception& e)
	{
		cout<<e.what()<<endl;
	}
	catch(...)
	{
		cout<<"Unknown problem"<<endl;
	}
 	return(0);
}  
