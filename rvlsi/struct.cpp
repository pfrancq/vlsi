/*

	RVLSI Project Library

	Struct.cpp

	VLSI Structure - Implementation.

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2003 by the Université Libre de Bruxelles (ULB).

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
// include files for RVLSI Project
#include <struct.h>
using namespace R;
using namespace RVLSI;
using namespace std;



//------------------------------------------------------------------------------
//
// Class RLibrary
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RLibrary::RLibrary(const RString& name)
	: Name(name.ToUpper()), RealName(name), Cells(20,10)
{
}


//------------------------------------------------------------------------------
void RLibrary::InsertCell(RCell* cell)
{
	Cells.InsertPtr(cell);
}


//------------------------------------------------------------------------------
RLibrary::~RLibrary(void)
{
}



//------------------------------------------------------------------------------
//
// Class RPort
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPort::RPort(const RString& name,char dir)
	: Name(name), Owner(0), Dir(dir)
{
}



//------------------------------------------------------------------------------
//
// Class RCell
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RCell::RCell(const RString& name)
	: Name(name), Lib(0), Struct(0), Ports(10,5), Instances(10,5), Nets(10,5),
	  Polygons(), Abstract(true)
{
}


//------------------------------------------------------------------------------
RCell::RCell(const RString& name,RStructure* s,RLibrary* lib)
	: Name(name), Lib(lib), Struct(s), Ports(10,5), Instances(10,5), Nets(10,5),
	  Polygons(), Abstract(true)
{
	if(Lib)
		Lib->InsertCell(this);
}


//------------------------------------------------------------------------------
void RCell::SetAbstract(bool abstract)
{
	Abstract=abstract;
}


//------------------------------------------------------------------------------
void RCell::InsertPolygon(R::RPolygon* poly)
{
	Polygons.InsertPtr(poly);
}


//------------------------------------------------------------------------------
void RCell::InsertInstance(const RString& name,const RString& cell)
{
	Instances.InsertPtr(new RInstance(name,Struct->GetCell(cell)));
}


//------------------------------------------------------------------------------
void RCell::InsertPort(const R::RString& name,const char dir)
{
	Ports.InsertPtr(new RPort(name,dir));
}


//------------------------------------------------------------------------------
RNet* RCell::InsertNet(const RString& name)
{
	RNet *ptr;
	Nets.InsertPtr(ptr=new RNet(name,this));
	return(ptr);
}


//------------------------------------------------------------------------------
RCell::~RCell(void)
{
}



//------------------------------------------------------------------------------
//
// Class RInstance
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RInstance::RInstance(const RString& name)
	: Name(name), Cell(0), PortRefs(0)
{
}


//------------------------------------------------------------------------------
RInstance::RInstance(const R::RString& name,RCell* cell)
	: Name(name), Cell(cell), PortRefs(0)
{
}



//------------------------------------------------------------------------------
//
// Class RPortRef
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPortRef::RPortRef(RPort* port,RInstance* inst)
	: Port(port), Instance(inst)
{
}



//------------------------------------------------------------------------------
//
// Class RNet
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RNet::RNet(const RString& name,RCell* owner)
	: Name(name), Owner(owner), Refs(10,5)
{
}


//------------------------------------------------------------------------------
void RNet::InsertRef(const RString& port,const RString& instance)
{
	RPort* Port(Owner->GetPort(port));
	RInstance* Inst(Owner->GetInstance(instance));
	if((!Port)&&Inst)
	{
		Port=Inst->GetCell()->GetPort(port);
	}
	Refs.InsertPtr(new RPortRef(Port,Inst));
}


//------------------------------------------------------------------------------
RNet::~RNet(void)
{
}



//------------------------------------------------------------------------------
//
// Class RStructure
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RStructure::RStructure(void)
	: Libraries(10), Cells(20), Units(0.0)
{
}


//------------------------------------------------------------------------------
RCell* RStructure::CreateCell(const RString& name,RLibrary* lib)
{
	RCell* ptr(new RCell(name,this,lib));
	Cells.InsertPtr(ptr);
	return(ptr);
}


//------------------------------------------------------------------------------
RCell* RStructure::GetCell(const R::RString& name)
{
	return(Cells.GetInsertPtr(name));
}


//------------------------------------------------------------------------------
RLibrary* RStructure::GetLibrary(const R::RString& name)
{
	return(Libraries.GetInsertPtr(name));
}


//------------------------------------------------------------------------------
RStructure::~RStructure(void)
{
}
