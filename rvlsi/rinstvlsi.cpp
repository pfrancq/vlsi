/*

	Rainbow Library Project

	RInstVLSI.cpp

	Instance for VLSI placement GA - Implementation

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
// include files for Rainbow
#include "rinstvlsi.h"
using namespace RGA;



//---------------------------------------------------------------------------
//
// RInstVLSI
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RInstVLSI::RInstVLSI(unsigned int max,unsigned int popsize,RObj2D **objs,unsigned int nbobjs,RPoint &limits,HeuristicType h) throw(bad_alloc)
	: RInst2D<RInstVLSI,RChromoVLSI,RFitnessVLSI,RThreadDataVLSI,RGeoInfo>(popsize,objs,nbobjs,limits,h),
		MaxGen(max)
{
}


//---------------------------------------------------------------------------
bool RInstVLSI::StopCondition(void)
{
	return(Gen==MaxGen);
}