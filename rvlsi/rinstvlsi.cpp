/*

	R Project Library

	RInstVLSI.cpp

	Instance for VLSI placement GA - Implementation

	(C) 1999-2001 by P. Francq.

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
// include files for R Project
#include <rvlsi/rinstvlsi.h>
#include <rvlsi/rchromovlsi.h>

//-----------------------------------------------------------------------------
//
// RThreadDataVLSI
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RVLSI::RThreadDataVLSI::RThreadDataVLSI(RInstVLSI *owner)
 : RThreadData2D<RInstVLSI,RChromoVLSI>(owner)
{
}



//-----------------------------------------------------------------------------
//
// RInstVLSI
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RVLSI::RInstVLSI::RInstVLSI(unsigned int max,unsigned int popsize,RProblem2D* prob,HeuristicType h,RDebug *debug) throw(bad_alloc)
	: RInst2D<RInstVLSI,RChromoVLSI,RFitnessVLSI,RThreadDataVLSI,RGeoInfo>(popsize,prob,h,debug),
		MaxGen(max)
{
}


//-----------------------------------------------------------------------------
bool RVLSI::RInstVLSI::StopCondition(void)
{
	return(Gen==MaxGen);
}
