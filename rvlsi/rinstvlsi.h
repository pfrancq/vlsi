/*

	R Project Library

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



//-----------------------------------------------------------------------------
#ifndef RInstVLSIH
#define RInstVLSIH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rvlsi/rgavlsi.h>
#include <rga2d/rinst2d.h>
using namespace RGA2D;


//-----------------------------------------------------------------------------
namespace RVLSI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
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
	* @param owner          The instance of the problem.
	*/
	RThreadDataVLSI(RInstVLSI *owner);
};


//-----------------------------------------------------------------------------
/**
* The instance of the VLSI problem.
* @author Pascal Francq
* @short VLSI GA Instance.
*/
class RInstVLSI : public RInst2D<RInstVLSI,RChromoVLSI,RFitnessVLSI,RThreadDataVLSI,RGeoInfo>
{
public:

	/**
	* The maximum number of generations.
	*/
	unsigned long MaxGen;

	/**
	* Construct the instance.
	* @param max            Maximal number of generations.
	* @param popsize        The size of the population.	
	* @param prob           The problem.
	* @param h              The type of heuristic to be used.
	* @param debug          Debugger.
	*/
	RInstVLSI(unsigned int max,unsigned int popsize,RProblem2D* prob,HeuristicType h,RDebug *debug=0) throw(bad_alloc);

	/**
	* This function determines if the GA must be stopped. Actually, it is the case
	* when the maximal number of generation is calculated.
	* @return	The function returns true if the GA must stop.
	*/
	virtual bool StopCondition(void);
};


}  //-------- End of namespace RVLSI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
