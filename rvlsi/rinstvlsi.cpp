/*

	R Project Library

	RInstVLSI.cpp

	Instance for VLSI placement GA - Implementation

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



//------------------------------------------------------------------------------
// include files for RVLSI Project
#include <rinstvlsi.h>
#include <rchromovlsi.h>
using namespace R;
using namespace RVLSI;
using namespace std;



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
RInstVLSI::RInstVLSI(size_t max,size_t popsize,RProblem2D* prob,const RString& h,
		RParamStruct* heurdist,RParamStruct* heurarea,RParamStruct* selectdist,RParamStruct* selectweight,
		RDebug* debug)
	: RInst2D<RInstVLSI,RChromoVLSI,RFitnessVLSI,RThreadDataVLSI,RGeoInfo>(popsize,prob,h,"VLSI",
			heurdist,heurarea,selectdist,selectweight,debug),
		MaxGen(max)
{
}


//------------------------------------------------------------------------------
bool RInstVLSI::StopCondition(void)
{
	return(GetGen()==MaxGen);
}
