/*

  Edif.h

  A EDIF File

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
#ifndef EDIFH
#define EDIFH


//---------------------------------------------------------------------------
// Includes
#if unix
	#include <unistd.h>
#endif
#include "rstd/tree.h"
using namespace RStd;
#include "files.h"
using namespace RVLSI;


//---------------------------------------------------------------------------
namespace RVLSI{
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constances
const long TYPECELL=1;
const long TYPELIBRARY=2;
const long TYPEINSTANCE=3;
const long TYPEPORTIMP=4;
const long TYPENET=5;
const long TYPEPORT=6;


//---------------------------------------------------------------------------
// Forward declarations
class REDIFTag;
class REDIFFile;


//---------------------------------------------------------------------------
class RStringId : public RString
{
public:
	long Id;	
	
	RStringId(const RString &str) : RString(str) { Id=-1; }
};


//---------------------------------------------------------------------------
// A EDIF Tag
class REDIFTag : public RBaseNode<REDIFTag>
{
public:
  RString TagName;
  RString TypeName;
  RString Params;

  REDIFTag(unsigned long ,REDIFFile*,char *(&Buffer),unsigned &BufferLen);
  virtual int Compare(REDIFTag*) {return(0);}
  virtual int Compare(RBaseNode<REDIFTag>*) {return(0);}
  virtual int Compare(char* name) { return(TagName.Compare(name)); }
  void InsertInst(REDIFFile*);
  void InsertPortImp(REDIFFile*);
  void InsertNet(REDIFFile*);
  bool Analyse(REDIFFile*);
};



//---------------------------------------------------------------------------
// A EDIF2 data file
class REDIFFile : public RDataFile
{
	RCell *CurrCell;
	RLibrary *CurrLib;
public:
  RTree<REDIFTag> *Struct;
	RContainer<RStringId,long,true,true> *Types;
	
  REDIFFile(char* name);
  virtual char* StringType(void) {return("=EDIF2");}
  virtual char* TreeType(void) {return(" - EDIF2 File");}
  virtual bool Analyse(void);
  ~REDIFFile(void);

  friend REDIFTag;
};


}  //-------- End of namespace RVLSI ---------------------------------------

//---------------------------------------------------------------------------
#endif
