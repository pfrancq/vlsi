/*

  RInstVLSI.h

  Instance for VLSI placement GA - Header

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

*/


//---------------------------------------------------------------------------
#ifndef RInstVLSIH
#define RInstVLSIH


//---------------------------------------------------------------------------
// Includes
#include "rchromovlsi.h"
#include "rinst2d.h"
#include "robj2d.h"
using namespace RGA;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
class RChromoVLSI; // Declare forward


//---------------------------------------------------------------------------
class RInstVLSI : public RInst2D<RInstVLSI,RChromoVLSI,RFitness<long int,false> >
{
public:
  RInstVLSI(unsigned popsize,RObj2D **objs) throw(bad_alloc);
};


}  //-------- End of namespace RGA ------------------------------------------


//---------------------------------------------------------------------------
#endif
