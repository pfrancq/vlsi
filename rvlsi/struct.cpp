/*

	R Project Library

	Struct.cpp

	VLSI Structure - Implementation.

	Copyright 1999-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
#include <rvlsi/struct.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// Class RLibrary
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RLibrary::RLibrary(const RString& name) throw(bad_alloc)
	: Name(name),RealName(name)
{
	Name.StrUpr();
	Cells=new RContainer<RCell,unsigned int,false,false>(20,10);
}


//------------------------------------------------------------------------------
void RLibrary::InsertCell(RCell* cell)
{
	Cells->InsertPtr(cell);
}


//------------------------------------------------------------------------------
RLibrary::~RLibrary(void)
{
	delete Cells;
}



//------------------------------------------------------------------------------
//
// Class RLibraries
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RLibraries::RLibraries(void) throw(bad_alloc)
	: RContainer<RLibrary,unsigned int,true,true>(20,5)
{
}



//------------------------------------------------------------------------------
//
// Class RPort
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPort::RPort(const RString &name) throw(bad_alloc)
	: Name(name)
{
	Dir=0;
}



//------------------------------------------------------------------------------
//
// Class RCell
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RCell::RCell(const RString& name) throw(bad_alloc)
	: Name(name), Polygons()
{
 Lib=0;
	Ports=0;
	Instances=0;
	Nets=0;
	Ports=new RContainer<RPort,unsigned int,true,true>(10,5);
	Instances=new RInstances(10,5);
	Nets=new RNets(10,5);
	Abstract=true;
}


//------------------------------------------------------------------------------
void RCell::InsertInst(const RString& name,const RString& cellref)
{
	(Instances->GetInstance(name))->CellRef=Owner->Struct->Cells->GetCell(cellref);
}


//------------------------------------------------------------------------------
RNet* RCell::InsertNet(const RString& name)
{
	RNet *ptr;

	ptr=Nets->GetNet(name);
	ptr->Owner=this;
	return(ptr);
}


//------------------------------------------------------------------------------
RCell::~RCell(void)
{
	if(Ports) delete Ports;
	if(Instances) delete Instances;
	if(Nets) delete Nets;
}



//------------------------------------------------------------------------------
//
// Class RCells
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RCells::RCells(RStructure* s) throw(bad_alloc)
	: RContainer<RCell,unsigned int,true,true>(500,250)
{
	Struct=s;
}



//------------------------------------------------------------------------------
//
// Class RNet
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RNet::RNet(const RString& name)
	: Name(name)
{
	Refs=new RContainer<RPortRef,unsigned int,true,false>(10,5);
}


//------------------------------------------------------------------------------
void RNet::InsertRef(const RString& port,const RString& instance)
{
	RPortRef *Ref=new RPortRef;

	Ref->Port=Owner->GetPort(port);
	Ref->Inst=Owner->Instances->GetInstance(instance);
	if((!Ref->Port)&&Ref->Inst)
	{
		Ref->Port=Ref->Inst->CellRef->GetPort(port);
	}
	Refs->InsertPtr(Ref);
}


//------------------------------------------------------------------------------
RNet::~RNet(void)
{
	delete Refs;
}



//------------------------------------------------------------------------------
//
// Class RStructure
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RStructure::RStructure(void)
{
	Libraries=new RLibraries();
//  Instances=new RInstances();
	Cells=new RCells(this);
	Units=0;
}


//------------------------------------------------------------------------------
RStructure::~RStructure(void)
{
	if(Libraries)
	{
		delete Libraries;
		Libraries=0;
	}
	if(Cells)
	{
		delete Cells;
		Cells=0;
	}
/*	if(Instances)
	{
		delete Instances;
		Instances=0;
	}*/
}
