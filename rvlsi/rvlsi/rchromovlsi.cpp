/*

	RVLSI Project Library

	RChromoVLSI.cpp

	Chromosome for VLSI placement GA - Implementation

	Copyright 1998-2014 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2003 by the Universit√© Libre de Bruxelles (ULB).

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
#include <rchromovlsi.h>
#include <rinstvlsi.h>
using namespace R;
using namespace RVLSI;
using namespace std;



//------------------------------------------------------------------------------
//
// class RChromoVLSI
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RChromoVLSI::RChromoVLSI(RInstVLSI *inst,size_t id)
  : RChromo2D<RInstVLSI,RChromoVLSI,RFitnessVLSI,RThreadDataVLSI,RGeoInfo>(inst,id)
{
}


//------------------------------------------------------------------------------
void RChromoVLSI::Evaluate(void)
{
	double Num,Den;

	Num=(static_cast<double>(ActLimits.GetWidth())*static_cast<double>(ActLimits.GetHeight()));
	Den=(static_cast<double>(Problem->GetLimits().GetWidth())*static_cast<double>(Problem->GetLimits().GetHeight()));
	(*Fitness)=Num/Den;
}
