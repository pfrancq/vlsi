//---------------------------------------------------------------------------
#include "CMain.h"

int GACmdTool(int argc, char* argv[])
{
  static char Proj[255];

  cout<<"GA for VLSI Version 1.0.0.0  Copyright (c) 1999 by Pascal Francq"<<endl;
  if((argc!=2)||(!strcmp(argv[1],"\?")))
  {
    cout<<"Syntax: cga datafile"<<endl;
    return(0);
  }
  strcpy(Proj,argv[1]);
  if(!strchr(Proj,'.')) strcat(Proj,".dat");

  return(0);
}

//---------------------------------------------------------------------------

