/*

  Files.cpp

  VLSI input and output Files - Implementation.

  (C) 1999-2000 by P. Francq.

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
#include "files.h"
using namespace RVLSI;


//---------------------------------------------------------------------------
//
// Class "RDataFile"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RDataFile::RDataFile(const RString &name)
{
  Name=name;
  Type=cstNothing;
}



//---------------------------------------------------------------------------
//
// Class "RProject"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RProject::RProject(const RString &name) throw(bad_alloc)
	: RContainer<RDataFile,unsigned int,true,true>(10,5),RStructure(), Name(name), InputName(name),OutputName(name)
{
}


//---------------------------------------------------------------------------
RProject::RProject(void) throw(bad_alloc)
	: RContainer<RDataFile,unsigned int,true,true>(10,5),RStructure(), Name(200), InputName(200),OutputName(200)
{
}


//---------------------------------------------------------------------------
bool RProject::LoadProject(void)
{
  FILE *f;
  static char Tmp[350];
  char *ptr;

  // Open the file
	#if __BORLANDC__
  	#pragma warn -pia
	#endif
  if(!(f=fopen(Name(),"r"))) return(false);
	#if __BORLANDC__
	  #pragma warn .pia
	#endif

  // Read in the File
  ReadLine(Tmp,350,f);    // Dta File
  ptr=strstr(Tmp,"=");
  (*ptr)=0;
  InputName=Tmp;
  ReadLine(Tmp,350,f);    // Res File
  ptr=strstr(Tmp,"=");
  (*ptr)=0;
  OutputName=Tmp;
  while(ReadLine(Tmp,350,f)) // Read GDSII and EDIF2 Files
  {
    ptr=strstr(Tmp,"=");
    (*(ptr++))=0;
    if(!strcmp(ptr,"GDSII"))
    {
      InsertFile(CreateFile(Tmp,"GDSII"));
    }
    if(!strcmp(ptr,"EDIF2"))
    {
      InsertFile(CreateFile(Tmp,"EDIF2"));
    }
  }
	return(true);
}


//---------------------------------------------------------------------------
bool RProject::Analyse(void)
{
  RDataFile **file;
  unsigned int i;

  cout<<"Analysing Files ..."<<endl;
  for(i=NbPtr+1,file=Tab;--i;file++)
  {
    cout<<"  "<<(*file)->Name()<<" ...";
    (*file)->Analyse();
    cout<<"  OK"<<endl;
  }
  return(true);
}


//---------------------------------------------------------------------------
RProject::~RProject(void)
{
}


//---------------------------------------------------------------------------
