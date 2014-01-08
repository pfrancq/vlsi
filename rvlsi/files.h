/*

	RVLSI Project Library

	Files.h

	VLSI input and output Files - Header

	Copyright 1998-2014 by Pascal Francq (pascal@francq.info).
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
#ifndef Files_H
#define Files_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rstring.h>
#include <ruri.h>


//-----------------------------------------------------------------------------
// include files for RVLSI Project
#include <struct.h>


//-----------------------------------------------------------------------------
namespace RVLSI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Declaration
class RDataFile;
class RProject;


//-----------------------------------------------------------------------------
/**
 * The RDataFile provides a generic data file related to a VLSI project.
 * @author Pascal Francq
 * @short Generic VLSI Data File.
 */
class RDataFile
{
public:

	/**
	* Represent the different type of files of a VLSI project.
	*/
	enum tDataType
	{
		vdtNothing,              /** No specified type. */
		vdtEDIF2,                /** EDIF2 type. */
		vdtGDSII                 /** GDS II type. */
	};

protected:

	/**
	 * The project.
	 */
	RProject* Project;

	/**
	 * URI of the file.
	 */
	R::RURI URI;

	/**
	 * Type of the type.
	 */
	tDataType Type;

public:

	/**
	 * Construct a data file.
	 * @param project        Project.
	 * @param uri            URI of the file.
	 */
	RDataFile(RProject* project,const R::RURI& uri);

	/**
	 * Compare two data file.
	 * @param file           File to compare with.
	 * @return a value compatible with RContainer.
	 */
	int Compare(const RDataFile& file) const;

	/**
	 * Compare a file with a given uri.
	 * @param uri            URI to compare with.
	 * @return a value compatible with RContainer.
	 */
	int Compare(const R::RURI& uri) const { return(URI.Compare(uri)); }

	/**
	 * @return the URI of the file.
	 */
	inline R::RURI GetURI(void) const {return(URI);}

	/**
	 * Analyze the file.
	 * @param log            Log file (may be null).
	 */
	virtual void Analyse(R::RTextFile* log)=0;

	/**
	 * Destructor.
	 */
	virtual ~RDataFile(void);
};



//-----------------------------------------------------------------------------
/**
 * The RProject provides a representation of a VLSI project, i.e. a VLSI
 * structure and a set of data files.
 * @author Pascal Francq.
 * @short VLSI Project.
 */
class RProject : private R::RContainer<RDataFile,true,true>, public RStructure
{
	/**
	* Name of the project.
	*/
	R::RURI URI;

	/**
	 * URI of the pl2d file to generate.
	 */
	R::RURI PL2D;

	/**
	 * URI of the log file.
	 */
	R::RURI LogName;

public:

	/**
	 * Create a project.
	 * @param uri            URI of the project.
	 */
	RProject(const R::RURI& uri);

	/**
	 * Analyze the project.
	 */
	void Analyse(void);

	/**
	 * Destruct the project.
	 * @return
	 */
	virtual ~RProject(void);
};


}  //-------- End of namespace RVLSI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
