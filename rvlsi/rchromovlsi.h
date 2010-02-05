/*

	R Project Library

	RChromoVLSI.h

	Chromosome for VLSI placement GA - Header

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
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
#ifndef RChromoVLSI_H
#define RChromoVLSI_H


//------------------------------------------------------------------------------
// include files for Rainbow
#include <rgavlsi.h>
#include <rchromo2d.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
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
	* @param id             The identifier of the chromosome.
	*/
	RChromoVLSI(RInstVLSI* inst,unsigned int id);

	/**
	* Evaluate the chromosome, by comparing the area of the boundary rectangle,
	* with the sum of the areas of all the polygons.
	*/
	virtual void Evaluate(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
