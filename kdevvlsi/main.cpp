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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>

#include "kdevvlsi.h"

static const char *description =
	I18N_NOOP("KDevVLSI");
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE
	
	
static KCmdLineOptions options[] =
{
  { "+[File]", I18N_NOOP("file to open"), 0 },
  { 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{

	KAboutData aboutData( "testmdi", I18N_NOOP("KDevVLSI"),
		VERSION, description, KAboutData::License_GPL,
		"(c) 2000, ");
	aboutData.addAuthor("",0, "");
	KCmdLineArgs::init( argc, argv, &aboutData );
	KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

  KApplication app;
 
  if (app.isRestored())
  {
    RESTORE(KDevVLSIApp);
  }
  else 
  {
    KDevVLSIApp *testmdi = new KDevVLSIApp();
    testmdi->show();

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
		
		if (args->count())
		{
			for(int i=0;i<args->count();i++)
			{
        testmdi->openDocumentFile(args->arg(i));
		  }
		}
		else
		{
		  testmdi->openDocumentFile();
		}
		args->clear();
  }

  return app.exec();
}  
