/*

	R Project Library

	Files.cpp

	VLSI input and output Files - Implementation.

	Copyright 1999-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rtextfile.h>
#include <rvlsi/files.h>
using namespace R;



//------------------------------------------------------------------------------
//
// Class RDataFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDataFile::RDataFile(const RString& name)
{
	Name=name;
	Type=cstNothing;
}



//------------------------------------------------------------------------------
//
// Class RProject
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RProject::RProject(const RString &name) throw(bad_alloc)
	: RContainer<RDataFile,unsigned int,true,true>(10,5),RStructure(), Name(name), InputName(name),OutputName(name)
{
}


//------------------------------------------------------------------------------
RProject::RProject(void) throw(bad_alloc)
	: RContainer<RDataFile,unsigned int,true,true>(10,5),RStructure(), Name(200), InputName(200),OutputName(200)
{
}


//------------------------------------------------------------------------------
bool RProject::LoadProject(void)
{
	RTextFile f(Name(),Read);
	char *Tmp;
	char* ptr;

	// Read in the File
	Tmp=f.GetLine();    // Dta File
	ptr=strstr(Tmp,"=");
	(*ptr)=0;
	InputName=Tmp;
	Tmp=f.GetLine();    // Res File
	ptr=strstr(Tmp,"=");
	(*ptr)=0;
	OutputName=Tmp;
	while(!f.Eof()) // Read GDSII and EDIF2 Files
	{
		Tmp=f.GetLine();
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


//------------------------------------------------------------------------------
bool RProject::Analyse(void)
{
	RDataFile** file;
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


//------------------------------------------------------------------------------
RProject::~RProject(void)
{
}
