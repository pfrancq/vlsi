/*

	R Project Library

	RChromoVLSI.cpp

	Chromosome for VLSI placement GA - Implementation

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
#include <rvlsi/rchromovlsi.h>
#include <rvlsi/rinstvlsi.h>


//-----------------------------------------------------------------------------
//
// RChromoVLSI
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RVLSI::RChromoVLSI::RChromoVLSI(RInstVLSI *inst,unsigned int id) throw(bad_alloc)
  : RChromo2D<RInstVLSI,RChromoVLSI,RFitnessVLSI,RThreadDataVLSI,RGeoInfo>(inst,id)
{
}


//-----------------------------------------------------------------------------
void RVLSI::RChromoVLSI::Evaluate(void)
{
	double Num,Den;

	Num=(static_cast<double>(ActLimits.X)*static_cast<double>(ActLimits.Y));
	Den=(static_cast<double>(Limits.X)*static_cast<double>(Limits.Y));
	(*Fitness)=Num/Den;
}
