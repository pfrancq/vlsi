/*

	RVLSI Project Library

	RInstVLSI.h

	Instance for VLSI placement GA - Header

	Copyright 1998-2012 by Pascal Francq (pascal@francq.info).
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
#ifndef RInstVLSI_H
#define RInstVLSI_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rinst2d.h>


//-----------------------------------------------------------------------------
// include files for RVLSI Project
#include <rgavlsi.h>


//-----------------------------------------------------------------------------
namespace RVLSI{
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent "thread-dependent" data for the VLSI GA.
* @author Pascal Francq
* @short VSLI GA "thread-dependent" Data.
*/
class RThreadDataVLSI : public R::RThreadData2D<RInstVLSI,RChromoVLSI>
{
public:

	/**
	* Construct the data.
	* @param owner          The instance of the problem.
	*/
	RThreadDataVLSI(RInstVLSI* owner);
};


//------------------------------------------------------------------------------
/**
* The instance of the VLSI problem.
* @author Pascal Francq
* @short VLSI GA Instance.
*/
class RInstVLSI : public R::RInst2D<RInstVLSI,RChromoVLSI,RFitnessVLSI,RThreadDataVLSI,R::RGeoInfo>
{
public:

	/**
	* The maximum number of generations.
	*/
	size_t MaxGen;

	/**
	* Construct the instance.
	* @param max            Maximal number of generations.
	* @param popsize        The size of the population.
	* @param prob           The problem.
	* @param h              The type of heuristic to be used.
	* @param heurdist       Distance criteria parameters for the heuristic.
	* @param heurarea       Area criteria parameters for the heuristic.
	* @param selectdist     Distance criteria parameters for the objects
	*                       selection.
	* @param selectweight   Weight criteria parameters for the objects
	*                       selection.
	* @param debug          Debugger.
	*/
	RInstVLSI(size_t max,size_t popsize,R::RProblem2D* prob,const R::RString& h,
			R::RParamStruct* heurdist,R::RParamStruct* heurarea,R::RParamStruct* selectdist,R::RParamStruct* selectweight,
			R::RDebug* debug=0);

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
