/*

  CmdMain

  Command-line tool for VLSI Project Analysing

  By (c) P. Francq, 1999.

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


//---------------------------------------------------------------------------
#include "cmdmain.h"


//---------------------------------------------------------------------------
int VlsiCmdTool(int argc, char* argv[])
{
  cVLSIProject *Project=NULL;
  static char Proj[255];

  cout<<"VLSI Project Analyser Version 1.0.0.0  Copyright (c) 1999 by Pascal Francq"<<endl;
  if((argc!=2)||(!strcmp(argv[1],"\?")))
  {
    cout<<"Syntax: vlsi2ga Projname"<<endl;
    return(0);
  }
  strcpy(Proj,argv[1]);
  if(!strchr(Proj,'.')) strcat(Proj,".vlsiprj");
  Project=new cVLSIProject(Proj);
  Project->LoadProject();
  Project->AnalyseProject();
  if(Project) delete Project;
  return(0);
}


//---------------------------------------------------------------------------

