/*

	R Project Library

	RChromoVLSI.h

	Chromosome for VLSI placement GA - Header

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
#ifndef RChromoVLSIH
#define RChromoVLSIH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rvlsi/rgavlsi.h>
using namespace RVLSI;
#include <rga2d/rchromo2d.h>
using namespace RGA2D;


//------------------------------------------------------------------------
namespace RVLSI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The chromosome for the VLSI GA.
* @author Pascal Francq
* @short VLSI GA Chromosome.
*/
class RChromoVLSI : public RChromo2D<RInstVLSI,RChromoVLSI,RFitnessVLSI,RThreadDataVLSI,RGeoInfo>
{
public:

	/**
	* Construct the chromosome.
	* @param inst           The instance of the problem.
	* @param id             The identificator of the chromosome.
	*/
	RChromoVLSI(RInstVLSI *inst,unsigned int id) throw(bad_alloc);

	/**
	* Evaluate the chromosome, by comparing the area of the boundary rectangle,
	* with the sum of the areas of all the polygons.
	*/
	virtual void Evaluate(void);
};


}  //-------- End of namespace RVLSI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
