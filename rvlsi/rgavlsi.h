/*

	R Project Library

	RGAVLSI.h

	GA for VLSI placement - Header

	Copyright 1999-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef RGAVLSI_H
#define RGAVLSI_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rga/rfitness.h>
#include <rga/rga2d.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward class declaration
class RInstVLSI;
class RChromoVLSI;
class RThreadDataVLSI;
class RFitnessVLSI;


//------------------------------------------------------------------------------
/**
* This class represent the fitness for the VLSI GA.
* @author Pascal Francq
* @short VLSI Fitness.
*/
class RFitnessVLSI : public RFitness<double,false>
{
public:

	/**
	* Construct the fitness.
	*/
	RFitnessVLSI(void) : RFitness<double,false>() {}

	/**
	* Assignment operator with a fitness f.
	*/
	RFitnessVLSI& operator=(const RFitnessVLSI &f)
	{
		RFitness<double,false>::operator=(f);
		return(*this);
	}

	/**
	* Assignment operator with a double value.
	*/
	RFitnessVLSI& operator=(const double val)
	{
		RFitness<double,false>::operator=(val);
		return(*this);
	}
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
