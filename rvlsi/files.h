/*

	R Project Library

	Files.h

	VLSI input and output Files - Header

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
#ifndef Files_H
#define Files_H


//------------------------------------------------------------------------------
// Includes for ANSI C/C++
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#ifdef _BSD_SOURCE
#else
	#include <io.h>
#endif
#include <stdio.h>
#include <fcntl.h>
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
#include <rstd/rstd.h>
#include <rvlsi/struct.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Declaration
class RDataFile;
class RProject;


//------------------------------------------------------------------------------
// Constants
const char cstNothing=0;
const char cstEDIF2=1;
const char cstGDSII=2;


//------------------------------------------------------------------------------
// A typical data file
class RDataFile
{
public:
	RProject *Proj;
	RString Name;
	char Type;

	RDataFile(const RString &);
	int Compare(const RDataFile* file) const
	{
		if(Type==file->Type)
			return(Name.Compare(file->Name));
		else
		 return(Type-file->Type);
	}
	int Compare(const RString &name) { return(Name.Compare(name)); }
	virtual const char* StringType(void) {return(0);}
	virtual const char* TreeType(void) {return(0);}
	virtual bool Analyse(void) {return(true);}
	virtual ~RDataFile(void) {}
};



//------------------------------------------------------------------------------
// A VLSI project
class RProject : public RContainer<RDataFile,true,true>, public RStructure
{
public:
	/**
	* Name of the project.
	*/
	RString Name;
	RString InputName,OutputName;

	RProject(const RString &) throw(std::bad_alloc);
	RProject(void) throw(std::bad_alloc);
	inline void InsertFile(RDataFile *file)
	{
		InsertPtr(file);
		file->Proj=this;
	}

	/**
	* \remarks{Only for Text interface !!!!}
	*/
	bool LoadProject(void);

	/**
	* \remarks{Only for Text interface !!!!}
	*/
	bool Analyse(void);
	virtual RDataFile* CreateFile(const RString&,const RString&) {return(0);};
	virtual ~RProject(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
