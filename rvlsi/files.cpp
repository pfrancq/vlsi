/*

  Files

  Representing VLSI input and output Files

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
// Class "RVLSIProject"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RProject::RProject(const RString &name) throw(bad_alloc)
	: Name(name),RContainer<RDataFile,unsigned,true,true>(10,5), RStructure()
{
}


//---------------------------------------------------------------------------
void RProject::LoadProject(void)
{
  FILE *f;
  static char Tmp[350];
  char *ptr;

  cout<<"Open Project File "<<Name()<<" ...";
  // Open the file
  #pragma warn -pia
  if(!(f=fopen(Name(),"r")))
  {
    cout<<"Error"<<endl;
    return;
  }
  #pragma warn .pia
  cout<<endl;

  // Read in the File
  ReadLine(Tmp,350,f);    // Dta File
  ptr=strstr(Tmp,"=");
  (*ptr)=0;
  InputName=Tmp;
  cout<<"  Input File: "<<Tmp<<endl;
  ReadLine(Tmp,350,f);    // Res File
  ptr=strstr(Tmp,"=");
  (*ptr)=0;
  OutputName=Tmp;
  cout<<"  Output File: "<<Tmp<<endl;
  while(ReadLine(Tmp,350,f)) // Read GDSII and EDIF2 Files
  {
    ptr=strstr(Tmp,"=");
    (*(ptr++))=0;
    if(!strcmp(ptr,"GDSII"))
    {
      cout<<"  Load GDSII "<<Tmp<<" ...";
      InsertFile(CreateFile(Tmp,"GDSII"));
      cout<<"OK"<<endl;
    }
    if(!strcmp(ptr,"EDIF2"))
    {
      cout<<"  Load EDIF2  "<<Tmp<<" ...";
      InsertFile(CreateFile(Tmp,"EDIF2"));
      cout<<"OK"<<endl;
    }
  }
}


//---------------------------------------------------------------------------
bool RProject::Analyse(void)
{
  RDataFile **file;
  unsigned i;

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
