/*

	RVLSI Project Library

	Edif.h

	EDIF File - Header.

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
#ifndef EDIF_H
#define EDIF_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rtree.h>
#include <rnode.h>


//-----------------------------------------------------------------------------
// include files for VLSI Project
#include <files.h>


//-----------------------------------------------------------------------------
namespace RVLSI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The REDIFFile class represents a EDIF2 data file.
* @short EDIF2 data file
*/
class REDIFFile : public RDataFile
{
	class EDIFTree;
	class StringId;
	class EDIFNode;

	/**
	 * Current cell treated.
	 */
	RCell* CurrCell;

	/**
	 * Current library treated.
	 */
	RLibrary* CurrLib;

	/**
	 * Types of the elements.
	 */
	R::RContainer<StringId,true,true> Types;

public:

	/**
	 * Construct a EDIF file.
	 * @param project        Project.
	 * @param uri            URI of the file.
	 */
	REDIFFile(RProject* project,const R::RURI& uri);

	/**
	 * Analyze the file.
	 * @param log            Log file (may be null).
	 */
	virtual void Analyse(R::RTextFile* log);

	/**
	 * Destructor of the EDIF file.
	 * @return
	 */
	~REDIFFile(void);

	friend class EDIFNode;
	friend class EDIFTree;
};


}  //-------- End of namespace RVLSI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
