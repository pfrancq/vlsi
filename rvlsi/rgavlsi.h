/*

	Rainbow Library Project

	RGAVLSI.h

	GA for VLSI placement - Header

	(C) 1999-2000 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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



//---------------------------------------------------------------------------
#ifndef RGAVLSIH
#define RGAVLSIH


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rga.h"
#include "rga2d.h"
using namespace RGA;


//---------------------------------------------------------------------------
// Forward class declaration
namespace RGA
{
	class RInstVLSI;
	class RChromoVLSI;
	class RThreadDataVLSI;
	class RFitnessVLSI;
}


//---------------------------------------------------------------------------
// include files for GA
#include "rinstvlsi.h"
#include "rchromovlsi.h"
using namespace RGA;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
* This class represent "thread-dependent" data for the VLSI GA.
* @author Pascal Francq
* @short VSLI GA "thread-dependent" Data.
*/
class RThreadDataVLSI : public RThreadData2D<RInstVLSI,RChromoVLSI>
{
public:

	/**
	* Construct the data.
	* @param owner	The instance of the problem.
	*/
	RThreadDataVLSI(RInstVLSI *owner) : RThreadData2D<RInstVLSI,RChromoVLSI>(owner) {}
};


//---------------------------------------------------------------------------
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


}  //-------- End of namespace RGA ------------------------------------------


//---------------------------------------------------------------------------
#endif
