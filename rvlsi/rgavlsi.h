/*

	RVLSI Project Library

	RGAVLSI.h

	GA for VLSI placement - Header

	Copyright 1998-2010 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2003 by the Université Libre de Bruxelles (ULB).

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
#ifndef RGAVLSI_H
#define RGAVLSI_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rfitness.h>
#include <rga2d.h>


//-----------------------------------------------------------------------------
namespace RVLSI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Forward class declaration
class RInstVLSI;
class RChromoVLSI;
class RThreadDataVLSI;
class RFitnessVLSI;


//-----------------------------------------------------------------------------
/**
* This class represent the fitness for the VLSI GA.
* @author Pascal Francq
* @short VLSI Fitness.
*/
class RFitnessVLSI : public R::RFitness<double,false>
{
public:

	/**
	* Construct the fitness.
	*/
	RFitnessVLSI(void);

	/**
	* Assignment operator with a fitness f.
	*/
	RFitnessVLSI& operator=(const RFitnessVLSI &f);

	/**
	* Assignment operator with a double value.
	*/
	RFitnessVLSI& operator=(const double val);
};


}  //-------- End of namespace RVLSI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
