/*

	Rainbow Library Project

	RInstVLSI.h

	Instance for VLSI placement GA - Header

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
#ifndef RInstVLSIH
#define RInstVLSIH


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rgavlsi.h"
using namespace RGA;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
* The instance of the VLSI problem.
* @author Pascal Francq
* @short VLSI GA Instance.
*/
class RInstVLSI :	public RInst2D<RInstVLSI,RChromoVLSI,RFitnessVLSI,RThreadDataVLSI,RGeoInfo>
{
public:

	/**
	* The maximum number of generations.
	*/
	unsigned long MaxGen;

	/**
	* Construct the instance.
	* @param popsize		The size of the population.
	* @param objs				The objects to place.
	* @param nbobjs			Number of objects to place.
	* @param max				Maximal number of generations.
	* @param limits			The limits for the placement.
	* @param h					The type of heuristic to be used.
	*/
	RInstVLSI(unsigned int max,unsigned int popsize,RObj2D **objs,unsigned int nbobjs,RPoint &limits,HeuristicType h) throw(bad_alloc);

	/**
	* This function determines if the GA must be stopped. Actually, it is the case
	* when the maximal number of generation is calculated.
	* @return	The function returns true if the GA must stop.
	*/
	virtual bool StopCondition(void);
};


}  //-------- End of namespace RGA ------------------------------------------


//---------------------------------------------------------------------------
#endif
