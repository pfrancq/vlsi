/*

	R Project Library

	VlsiPrj.h

	VLSI Project File - Header.

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2003 by the Université Libre de Bruxelles (ULB).

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
#ifndef RVLSIPRJ_H
#define RVLSIPRJ_H


//------------------------------------------------------------------------------
// Include files for R Project
#include <files.h>
#include <edif.h>
#include <gds.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* @short A VLSI project.
*/
class RVLSIProject : public RProject
{
public:

	RVLSIProject(const RString& name) throw(std::bad_alloc) : RProject(name) {}
	RVLSIProject(void) throw(std::bad_alloc) : RProject() {}
	virtual RDataFile* CreateFile(const RString& name,const RString& type)
	{
		if(type=="GDSII") return(new RGDSFile(name));
		if(type=="EDIF2") return(new REDIFFile(name));
		return(NULL);
	}
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
