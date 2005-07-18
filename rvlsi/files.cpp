/*

	R Project Library

	Files.cpp

	VLSI input and output Files - Implementation.

	Copyright 1999-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <rtextfile.h>
#include <files.h>
using namespace R;
using namespace std;



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
	: RContainer<RDataFile,true,true>(10,5),RStructure(), Name(name), InputName(name),OutputName(name)
{
}


//------------------------------------------------------------------------------
RProject::RProject(void) throw(bad_alloc)
	: RContainer<RDataFile,true,true>(10,5),RStructure(), Name(200), InputName(200),OutputName(200)
{
}


//------------------------------------------------------------------------------
bool RProject::LoadProject(void)
{
	RTextFile f(Name);
	RString Tmp;
	unsigned int pos;

	// Read in the File
	f.Open(RIO::Read);
	Tmp=f.GetLine();    // Dta File
	pos=Tmp.Find('=');
	InputName=Tmp.Mid(0,pos);
	Tmp=f.GetLine();    // Res File
	pos=Tmp.Find('=');
	OutputName=Tmp.Mid(0,pos);
	while(!f.Eof()) // Read GDSII and EDIF2 Files
	{
		Tmp=f.GetLine();
		pos=Tmp.Find('=');
		if(Tmp=="GDSII")
			InsertFile(CreateFile(Tmp,"GDSII"));
		if(Tmp=="EDIF2")
			InsertFile(CreateFile(Tmp,"EDIF2"));
	}
	return(true);
}


//------------------------------------------------------------------------------
bool RProject::Analyse(void)
{
	cout<<"Analysing Files ..."<<endl;
	RCursor<RDataFile> file(*this);
	for(file.Start();!file.End();file.Next())
	{
		cout<<"  "<<file()->Name()<<" ...";
		file()->Analyse();
		cout<<"  OK"<<endl;
	}
	return(true);
}


//------------------------------------------------------------------------------
RProject::~RProject(void)
{
}
