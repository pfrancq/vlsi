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
#ifndef FilesH
#define FilesH


//---------------------------------------------------------------------------
// Includes
#include <iostream.h>
#include <string.h>
#include <sys/stat.h>
#ifdef unix

#else
	#include <io.h>
#endif
#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include "rstd/rstring.h"
#include "rstd/rfunc.h"
using namespace RStd;
#include "struct.h"
using namespace RVLSI;


//---------------------------------------------------------------------------
namespace RVLSI{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Declaration
class RDataFile;
class RProject;


//---------------------------------------------------------------------------
// Constants
const char cstNothing=0;
const char cstEDIF2=1;
const char cstGDSII=2;


//---------------------------------------------------------------------------
// A typical data file
class RDataFile
{
public:
  RProject *Proj;
  RString Name;
  char Type;

  RDataFile(const RString &);
  int Compare(RDataFile* file) { return(Type-file->Type); }
  int Compare(const RString &name) { return(Name.Compare(name)); }
  virtual char* StringType(void) {return("");}
  virtual char* TreeType(void) {return("");}
  virtual bool Analyse(void) {return(true);}
};



//---------------------------------------------------------------------------
// A VLSI project
class RProject : public RContainer<RDataFile,unsigned,true,true>, public RStructure
{
public:
  RString Name;                                     // Name
  RString InputName,OutputName;

  RProject(const RString &) throw(bad_alloc);
  inline void InsertFile(RDataFile *file)
  {
    InsertPtr(file);
    file->Proj=this;
  }
  void LoadProject(void);      											// Only for Text interface !!!!
  bool Analyse(void);         											// Only for Text interface !!!!
  virtual RDataFile* CreateFile(const RString &name,const RString &type) {return(NULL);};
  ~RProject(void);
};


}  //-------- End of namespace RVLSI ---------------------------------------

//---------------------------------------------------------------------------
#endif
