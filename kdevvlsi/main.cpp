/*

  main.cpp

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
// include files for KDE
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>


//-----------------------------------------------------------------------------
// include file for current application
#include "kdevvlsi.h"


//-----------------------------------------------------------------------------
static const char *description =
	I18N_NOOP("KDevVLSI\nDevelopment Application for VLSI Placement");


//-----------------------------------------------------------------------------
static KCmdLineOptions options[] =
{
	{ "+[File]", I18N_NOOP("file to open"), 0 },
	{ 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};


//-----------------------------------------------------------------------------
KDevVLSIApp* theApp;


//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	KAboutData aboutData( "kdevvlsi", I18N_NOOP("KDevVLSI"),
		VERSION, description, KAboutData::License_GPL,
		"(c) 1999-2000, Université Libre de Bruxelles","http://www.ulb.ac.be",0,"pfrancq@ulb.ac.be");
	aboutData.addAuthor("Pascal Francq",0, "pfrancq@ulb.ac.be");

	KCmdLineArgs::init( argc, argv, &aboutData );
	KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

	KApplication app;

	if (app.isRestored())
	{
		RESTORE(KDevVLSIApp);
	}
	else
	{
		theApp = new KDevVLSIApp();
		theApp->show();

		KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
		
		if(args->count())	// Must be files
		{
			for(int i=0;i<args->count();i++)
			{
				theApp->openDocumentFile(args->arg(i));
			}
		}
		args->clear();
	}
	return app.exec();
}  
