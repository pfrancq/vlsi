/*

	Struct.h

	VLSI Structure - Header.

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
#ifndef Struct_H
#define Struct_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rstring.h>
#include <rpolygons.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward class definition
class RLibrary;                   // A Library
class RLibraries;                 // All the Libraries
class RCell;                      // A Cell
class RCells;                     // All the Cells
class RPort;                      // A port of a Cell
class RInstance;                  // A Instance of a Cell
class RInstances;                 // Instances
class RPortRef;                   // A reference to a Port and a Instance
class RNet;                       // A Net
class RNets;                      // Nets
class RStructure;                 // A VLSI Structure


//------------------------------------------------------------------------------
class RLibrary
{
public:
	RString Name;
	RString RealName;
	RContainer<RCell,false,false>* Cells;

	RLibrary(const RString& name) throw(std::bad_alloc);
	int Compare(const RString& name) const { return(Name.Compare(name)); }
	int Compare(const RLibrary& lib) const { return(Name.Compare(lib.Name)); }
	void InsertCell(RCell*);
	~RLibrary(void);
};


//------------------------------------------------------------------------------
class RLibraries : public RContainer<RLibrary,true,true>
{
public:
	RLibraries(void) throw(std::bad_alloc);
	RLibrary* InsertLib(const RString &name) throw(std::bad_alloc)
	{
		return(GetInsertPtr<RString>(name));
	}
};


//------------------------------------------------------------------------------
class RPort
{
public:
	RString Name;
	unsigned int Id;
	RCell* Owner;
	char Dir;

	RPort(const RString& name) throw(std::bad_alloc);
	int Compare(const RString& name) const { return(Name.Compare(name)); }
	int Compare(const RPort& port) const { return(Name.Compare(port.Name)); }
};


//------------------------------------------------------------------------------
class RCell
{
public:
	RString Name;
	RLibrary* Lib;
	RCells* Owner;
	RContainer<RPort,true,true> *Ports;
	RInstances* Instances;
	RNets* Nets;
	RPolygons Polygons;
	bool Abstract;

	RCell(const RString &name) throw(std::bad_alloc);
	int Compare(const RString& name) const { return(Name.Compare(name)); }
	int Compare(const RCell& cell) const { return(Name.Compare(cell.Name)); }
	RPort* GetPort(const RString& name) const { return(Ports->GetInsertPtr<RString>(name)); }
	void InsertPort(const RString& name,const char dir)
	{
		RPort* port=GetPort(name);
		port->Dir=dir;
	}
	RNet* InsertNet(const RString &name);
	void InsertInst(const RString &name,const RString &cellref);
	~RCell(void);
};


//------------------------------------------------------------------------------
class RCells : public RContainer<RCell,true,true>
{
public:
	RStructure* Struct;

	RCells(RStructure* s) throw(std::bad_alloc);
	RCell* GetCell(const RString& name) const { return(GetPtr<RString>(name)); }
	RCell* InsertCell(const RString& name,RLibrary* lib)
	{
		RCell* ptr=GetInsertPtr<const RString>(name);
		ptr->Owner=this;
		if(lib)
		{
			ptr->Lib=lib;
			lib->InsertCell(ptr);
		}
		return(ptr);
	}
};


//------------------------------------------------------------------------------
class RInstance
{
public:
	RCell* CellRef;
	RContainer<RPortRef,true,true>* PortRefs;
	RString Name;

	RInstance(const RString& name) throw(std::bad_alloc) { Name=name; }
	int Compare(const RString& name) const { return(Name.Compare(name)); }
	int Compare(const RInstance& inst) const { return(Name.Compare(inst.Name)); }
};


//------------------------------------------------------------------------------
class RInstances : public RContainer<RInstance,true,true>
{
public:
	RInstances(long max,long inc) : RContainer<RInstance,true,true>(max,inc) {}
	RInstance* GetInstance(const RString& name) { return(GetInsertPtr<RString>(name)); }
};


//------------------------------------------------------------------------------
class RPortRef
{
public:
	RPort* Port;
	RInstance* Inst;

	int Compare(const RPortRef& ref) const {return(Port->Compare(*ref.Port));}
};


//------------------------------------------------------------------------------
class RNet
{
public:
	RString Name;
	RCell* Owner;
	RContainer<RPortRef,true,false>* Refs;

	RNet(const RString &name);
	int Compare(const RString &name) const { return(Name.Compare(name)); }
	int Compare(const RNet& net) const { return(Name.Compare(net.Name)); }
	void InsertRef(const RString &port,const RString &instance);
	~RNet(void);
};


//------------------------------------------------------------------------------
class RNets : public RContainer<RNet,true,true>
{
public:
	RNets(long max,long inc) : RContainer<RNet,true,true>(max,inc) {}
	RNet* GetNet(const RString &name) { return(GetInsertPtr<RString>(name)); }
};


//------------------------------------------------------------------------------
class RStructure
{
public:
	RLibraries *Libraries;                // Libraries
	RCells *Cells;                        // Cells
	RInstances *Instances;                // Instances
	tCoord Units;                         // the units

	RStructure(void);
	~RStructure(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
