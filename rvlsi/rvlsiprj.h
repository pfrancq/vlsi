/*

  VlsiPrj.h

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
#ifndef RVLSIPRJH
#define RVLSIFRJH


//---------------------------------------------------------------------------
// Includes
#include "files.h"
#include "edif.h"
#include "gds.h"
using namespace RVLSI;


//---------------------------------------------------------------------------
namespace RVLSI{
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// A VLSI project
class RVLSIProject : public RProject
{
public:

  RVLSIProject(const RString &name) throw(bad_alloc) : RProject(name) {}
  virtual RDataFile* CreateFile(const RString &name,const RString &type)
  {
  	if(type=="GDSII") return(new RGDSFile(name));
		if(type=="EDIF2") return(new REDIFFile(name));
		return(NULL°;		
  };
};


}  //-------- End of namespace RVLSI ---------------------------------------

//---------------------------------------------------------------------------
#endif
