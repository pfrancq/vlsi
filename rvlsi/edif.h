/*

	R Project Library

	Edif.h

	EDIF File - Header.

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
#ifndef EDIF_H
#define EDIF_H


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#ifdef _BSD_SOURCE
	#include <unistd.h>
#endif


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
#include <rstd/rtree.h>
#include <rstd/rnode.h>


//------------------------------------------------------------------------------
// include files for VLSI
#include <rvlsi/files.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Constances
const long TYPECELL=1;
const long TYPELIBRARY=2;
const long TYPEINSTANCE=3;
const long TYPEPORTIMP=4;
const long TYPENET=5;
const long TYPEPORT=6;


//------------------------------------------------------------------------------
// Forward declarations
class REDIFTag;
class REDIFFile;


//------------------------------------------------------------------------------
class RStringId : public RString
{
public:
	long Id;

	RStringId(const RString &str) : RString(str) { Id=-1; }
};


//------------------------------------------------------------------------------
/**
* @short EDIF Tag
*/
class REDIFTag : public RNode<REDIFTag,false>
{
public:
	RString TagName;
	RString TypeName;
	RString Params;
	unsigned int Id;

	REDIFTag(unsigned int,REDIFFile*,char *(&Buffer),unsigned &BufferLen);
	virtual int Compare(const REDIFTag*) const {return(0);}
	virtual int Compare(const RNode<REDIFTag,false>*) const {return(0);}
	virtual int Compare(const char* name) const { return(TagName.Compare(name)); }
	void InsertInst(REDIFFile*);
	void InsertPortImp(REDIFFile*);
	void InsertNet(REDIFFile*);
	bool Analyse(REDIFFile*);
};



//------------------------------------------------------------------------------
/**
* @short EDIF2 data file
*/
class REDIFFile : public RDataFile
{
	RCell *CurrCell;
	RLibrary *CurrLib;
public:
	RTree<REDIFTag,true,false> *Struct;
	RContainer<RStringId,true,true> *Types;

	REDIFFile(const RString& name);
	virtual const char* StringType(void) {return("=EDIF2");}
	virtual const char* TreeType(void) {return(" - EDIF2 File");}
	virtual bool Analyse(void);
	~REDIFFile(void);

	friend class REDIFTag;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
