/*

	Rainbow Library Project

  RChromoVLSI.cpp

  Chromosome for VLSI placement GA - Implementation

  (C) 1999-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	As a special exception to the GNU General Public License, permission is
	granted for additional uses of the text contained in its release
	of the Rainbow Library.

	The exception is that, if you link the Rainbow with other files
	to produce an executable, this does not by itself cause the
	resulting executable to be covered by the GNU General Public License.
	Your use of that executable is in no way restricted on account of
	linking the Rainbow library code into it.

	This exception does not however invalidate any other reasons why
	the executable file might be covered by the GNU General Public License.

	This exception applies only to the code released under the
	name Rainbow.  If you copy code from other releases into a copy of
	RAinbow, as the General Public License permits, the exception does
	not apply to the code that you add in this way.  To avoid misleading
	anyone as to the status of such modified files, you must delete
	this exception notice from them.

	If you write modifications of your own for Rainbow, it is your choice
	whether to permit this exception to apply to your modifications.
	If you do not wish that, delete this exception notice.

*/



//---------------------------------------------------------------------------
// include files for Rainbow
#include "rchromovlsi.h"
using namespace RGA;



//---------------------------------------------------------------------------
//
// RChromoVLSI
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RChromoVLSI::RChromoVLSI(RInstVLSI *inst,unsigned int id) throw(bad_alloc)
  : RChromo2D<RInstVLSI,RChromoVLSI,RFitnessVLSI,RThreadDataVLSI,RGeoInfo>(inst,id),
		VLSIInfos(NULL)
{
}


//---------------------------------------------------------------------------
void RChromoVLSI::Init(RThreadDataVLSI *thData) throw(bad_alloc)
{
	RGeoInfo **infos;
	unsigned int i;

	RChromo2D<RInstVLSI,RChromoVLSI,RFitnessVLSI,RThreadDataVLSI,RGeoInfo>::Init(thData);
	VLSIInfos = new RGeoInfos(NbObjs);
	for(i=NbObjs+1,infos=Infos;--i;infos++)
		VLSIInfos->InsertPtr(*infos);
}


//---------------------------------------------------------------------------
void RChromoVLSI::Evaluate(void)
{
	RRect rect;
	double Num,Den;

	VLSIInfos->Boundary(rect);
	Num=double(rect.Pt2.X-rect.Pt1.X)*double(rect.Pt2.Y-rect.Pt1.Y);
	Den=double(Limits.X)*double(Limits.Y);
	(*Fitness)=Num/Den;
}


//---------------------------------------------------------------------------
RChromoVLSI::~RChromoVLSI(void)
{
}

