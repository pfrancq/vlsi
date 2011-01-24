/*

	R Project Library

	RVLSIDoc.h

	R VLSI Documentation - Header.

	Copyright 2009-2011 by Pascal Francq (pascal@francq.info).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
#ifndef RVLSIDOCH
#define RVLSIDOCH


//-----------------------------------------------------------------------------
namespace RVLSI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * \mainpage RVLSI Library
 *
 * \section rvlsi_intro_sec Introduction
 *
 * The RVLSI provides a set of classes to manage VLSI project.
 *
 * The RVLSIApp is an implementation of a R::RApplication adapted for a VLSI
 * session. In practice, it manages the creation of a VLSI session and the
 * parameters (which are stored in configuration files).
 *
 * Besides that, the RVLSI library provides several tools to manage GDS II and
 * EDIF2 files. The idea is that the user defines a project file (with an
 * ".vlsiprj" extension) containing all the files needed by a project. The
 * RProject class manages such a project, build the structure from the
 * different files, and stores the result in a xml file that can be read by
 * the KDevVLSI application. The project file is defined by one file per line:
 * @code
 *    file=type
 * @endcode
 * where the type can be "EDIF2", "GDSII", "LOG" (one log file can be created to
 * store the errors) or "PL2D" (one output xml file).
 */


}  //-------- End of namespace RVLSI ------------------------------------------

//-----------------------------------------------------------------------------
#endif
