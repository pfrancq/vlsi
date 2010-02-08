/*

	RVLSI Project Library

	GDS.h

	GDS file - Header.

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



//-----------------------------------------------------------------------------
#ifndef GDS_H
#define GDS_H


//-----------------------------------------------------------------------------
// include files for RVLSI Project
#include <files.h>


//-----------------------------------------------------------------------------
namespace RVLSI{
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RGDSFile class represents a GDS data file.
 * @author Pascal Francq
 * @short GDS File.
 */
class RGDSFile : public RDataFile
{
	class Rec;

	/**
	 * Current cell treated.
	 */
	RCell* CurrCell;

	/**
	 * Current library treated.
	 */
	RLibrary *CurrLib;

	/**
	 * Name of the library.
	 */
	R::RString LibName;

	/**
	 * Units of the library
	 */
	R::tCoord Units;

	/**
	 * Records.
	 */
	R::RContainer<Rec,true,false> Recs;

public:

	/**
	 * Construct a GDS data file.
	 * @param project        Project.
	 * @param uri            URI of the file.
	 */
	RGDSFile(RProject* project,const R::RURI& uri);

	/**
	 * Analyze the file.
	 * @param log            Log file (may be null).
	 */
	virtual void Analyse(R::RTextFile* log);
};


}  //-------- End of namespace RVLSI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
