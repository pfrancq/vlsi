/*

  Struct.h

  The structure of the VLSI

  By (c) P. Francq, 1999.

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
#ifndef StructH
#define StructH


//---------------------------------------------------------------------------
// Includes
#include "rstd/rcontainer.h"
#include "rstd/rstring.h"
#include "geometry/polygons.h"
using namespace RStd;


//---------------------------------------------------------------------------
namespace RVLSI{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Forward class definition
class RLibrary;                   // A Library
class RLibraries;                 // All the Libraries
class RCell;                      // A Cell
class RCells;                     // All the Cells
class RPort;                      // A port of a Cell
class RInstance;                  // A Instance of a Cell
class RInstances;                 // Instances
class RPortRef;                  	// A reference to a Port and a Instance
class RNet;                       // A Net
class RNets;                      // Nets
class RStructure;                 // A VLSI Structure


//---------------------------------------------------------------------------
class RLibrary
{
public:
  RString Name;
  RString RealName;
  RContainer<RCell,long,false,false> *Cells;

  RLibrary(const RString &name) throw(bad_alloc);
  int Compare(const RString &name) { return(Name.Compare(name)); }
  int Compare(const RLibrary *lib) { return(Name.Compare(lib->Name)); }
  void InsertCell(RCell *);
  ~RLibrary(void);
};


//---------------------------------------------------------------------------
class RLibraries : public RContainer<RLibrary,long,true,true>
{
public:
  RLibraries(void) throw(bad_alloc);
  RLibrary* InsertLib(const RString &name) throw(bad_alloc)
  {
    return(GetInsertPtr<RString>(name));
  }
};


//---------------------------------------------------------------------------
class RPort
{
public:
  RString Name;
  long Id;
  RCell *Owner;
  char Dir;

  RPort(const RString &name) throw(bad_alloc);
  int Compare(const RString &name) { return(Name.Compare(name)); }
  int Compare(RPort *port) { return(Name.Compare(port->Name)); }
};


//---------------------------------------------------------------------------
class RCell
{
public:
  RString Name;
  RLibrary *Lib;
  RCells *Owner;
  RContainer<RPort,long,true,true> *Ports;
  RInstances *Instances;
  RNets *Nets;
  RPolygons Polygons;
  bool Abstract;

  RCell(const RString &name) throw(bad_alloc);
  int Compare(const RString &name) { return(Name.Compare(name)); }
  int Compare(RCell *cell) { return(Name.Compare(cell->Name)); }
	RPort* GetPort(const RString &name)  { return(Ports->GetInsertPtr<RString>(name)); }
  void InsertPort(const RString &name,const char dir)
  {
    RPort *port=GetPort(name);
    port->Dir=dir;
  }
  RNet* InsertNet(const RString &name);
  void InsertInst(const RString &name,const RString &cellref);
  ~RCell(void);
};


//---------------------------------------------------------------------------
class RCells : public RContainer<RCell,long,true,true>
{
public:
  RStructure *Struct;

  RCells(RStructure *s) throw(bad_alloc);
  RCell* GetCell(const RString &name) { return(GetPtr<RString>(name)); }
  RCell* InsertCell(const RString &name,RLibrary* lib)
  {
    RCell *ptr=GetInsertPtr<RString>(name);
    ptr->Owner=this;
    if(lib)
    {
      ptr->Lib=lib;
      lib->InsertCell(ptr);
    }
    return(ptr);
  }
};


//---------------------------------------------------------------------------
class RInstance
{
public:
  RCell *CellRef;
  RContainer<RPortRef,long,true,true> *PortRefs;
  RString Name;

  RInstance(const RString &name) throw(bad_alloc) { Name=name; }
  int Compare(const RString &name) { return(Name.Compare(name)); }
  int Compare(RInstance *inst) { return(Name.Compare(inst->Name)); }
};


//---------------------------------------------------------------------------
class RInstances : public RContainer<RInstance,long,true,true>
{
public:
	RInstances(long max,long inc) : RContainer<RInstance,long,true,true>(max,inc) {}
	RInstance* GetInstance(const RString &name)  { return(GetInsertPtr<RString>(name)); }
};


//---------------------------------------------------------------------------
class RPortRef
{
public:
  RPort *Port;
  RInstance *Inst;

  int Compare(RPortRef *ref) {return(Port->Compare(ref->Port));}
};


//---------------------------------------------------------------------------
class RNet
{
public:
  RString Name;
  RCell *Owner;
  RContainer<RPortRef,long,true,false> *Refs;

  RNet(const RString &name);
  int Compare(const RString &name) { return(Name.Compare(name)); }
  int Compare(RNet *net) { return(Name.Compare(net->Name)); }
  void InsertRef(const RString &port,const RString &instance);
  ~RNet(void);
};


//---------------------------------------------------------------------------
class RNets : public RContainer<RNet,long,true,true>
{
public:
	RNets(long max,long inc) : RContainer<RNet,long,true,true>(max,inc) {}
	RNet* GetNet(const RString &name)  { return(GetInsertPtr<RString>(name)); }
};


//---------------------------------------------------------------------------
class RStructure
{
public:
  RLibraries *Libraries;                // Libraries
  RCells *Cells;                        // Cells
  RInstances *Instances;								// Instances	
  RCoord Units;                         // the units

  RStructure(void);
  ~RStructure(void);
};


}  //-------- End of namespace RVLSI ---------------------------------------

//---------------------------------------------------------------------------
#endif
