/*

	R Project Library

	RInstVLSI.cpp

	Instance for VLSI placement GA - Implementation

	Copyright 1999-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
// include files for R Project
#include <rvlsi/rinstvlsi.h>
#include <rvlsi/rchromovlsi.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RThreadDataVLSI
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RThreadDataVLSI::RThreadDataVLSI(RInstVLSI *owner)
	: RThreadData2D<RInstVLSI,RChromoVLSI>(owner)
{
}



//------------------------------------------------------------------------------
//
// class RInstVLSI
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RInstVLSI::RInstVLSI(unsigned int max,unsigned int popsize,RProblem2D* prob,HeuristicType h,RDebug* debug) throw(bad_alloc)
	: RInst2D<RInstVLSI,RChromoVLSI,RFitnessVLSI,RThreadDataVLSI,RGeoInfo>(popsize,prob,h,debug),
		MaxGen(max)
{
}


//------------------------------------------------------------------------------
bool RInstVLSI::StopCondition(void)
{
	return(Gen==MaxGen);
}
