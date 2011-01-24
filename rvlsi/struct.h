/*

	RVLSI Project Library

	Struct.h

	VLSI Structure - Header.

	Copyright 1998-2011 by Pascal Francq (pascal@francq.info).
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


//-----------------------------------------------------------------------------
#ifndef Struct_H
#define Struct_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rstring.h>
#include <rpolygons.h>
#include <rpolygon.h>


//-----------------------------------------------------------------------------
namespace RVLSI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward class definition
class RLibrary;                   // A Library
class RCell;                      // A Cell
class RPort;                      // A port of a Cell
class RInstance;                  // A Instance of a Cell
class RPortRef;                   // A reference to a Port and a Instance
class RNet;                       // A Net
class RStructure;                 // A VLSI Structure


//-----------------------------------------------------------------------------
/**
 * The RLibrary class provides a representation for a class of libraries of
 * VLSI cells.
 * @author Pascal Francq.
 * @short Library of VLSI Cells.
 */
class RLibrary
{
	/**
	 * Name of the library (in uppercase).
	 */
	R::RString Name;

	/**
	 * Real name of the library.
	 */
	R::RString RealName;

	/**
	 * Cells.
	 */
	R::RContainer<RCell,false,false> Cells;

public:

	/**
	 * Construct a library.
	 * @param name           Name of the library.
	 */
	RLibrary(const R::RString& name);

	/**
	 * Compare two libraries
	 * @param lib            Library to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const RLibrary& lib) const { return(Name.Compare(lib.Name)); }

	/**
	 * Compare a library with a given name.
	 * @param name           Name to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const R::RString& name) const { return(Name.Compare(name)); }

	/**
	 * Insert a cell.
	 * @param cell           Cell to insert.
	 */
	void InsertCell(RCell* cell);

	/**
	 * Destructor.
	 * @return
	 */
	~RLibrary(void);
};


//-----------------------------------------------------------------------------
/**
 * The RPort class provides a representation for a port of a given cell.
 * @author Pascal Francq.
 * @short Cell Port.
 */
class RPort
{
	/**
	 * Name of the port.
	 */
	R::RString Name;

	/**
	 * Cell owning the port.
	 */
	RCell* Owner;

	/**
	 * Direction of the port.
	 */
	char Dir;

public:

	/**
	 * Constructor of the cell.
	 * @param name           Name of the port.
	 * @param dir            Direction of the port.
	 */
	RPort(const R::RString& name,char dir);

	/**
	 * Compare two ports.
	 * @param port           Port to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const RPort& port) const { return(Name.Compare(port.Name)); }

	/**
	 * Compare the name of a port with a given name.
	 * @param name           Name to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const R::RString& name) const { return(Name.Compare(name)); }

	/**
	 * @return the name of the port.
	 */
	R::RString GetName(void) const {return(Name);}
};


//-----------------------------------------------------------------------------
/**
 * The RCell class provides a representation for a VLSI cell.
 * @author Pascal Francq
 * @short VLSI Cell.
 */
class RCell
{
	/**
	 * Name of the cell.
	 */
	R::RString Name;

	/**
	 * Library.
	 */
	RLibrary* Lib;

	/**
	 * Corresponding structure.
	 */
	RStructure* Struct;

	/**
	 * Port of the cell.
	 */
	R::RContainer<RPort,true,true> Ports;

	/**
	 * Instances of the cell.
	 */
	R::RContainer<RInstance,true,true> Instances;

	/**
	 * Nets of the cell.
	 */
	R::RContainer<RNet,true,true> Nets;

	/**
	 * Polygons representing the cell.
	 */
	R::RPolygons Polygons;

	/**
	 * Specify if the cell is an abstract one.
	 */
	bool Abstract;

public:

	/**
	 * Construct a cell.
	 * @param name           Name of the cell.
	 */
	RCell(const R::RString& name);

	/**
	 * Construct a cell.
	 * @param name           Name of the cell.
	 * @param s              Structure.
	 * @param lib            Library (may be null).
	 */
	RCell(const R::RString& name,RStructure* s,RLibrary* lib);

	/**
	 * Compare two cells.
	 * @param cell           Cell to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const RCell& cell) const { return(Name.Compare(cell.Name)); }

	/**
	 * Compare the name of the cell with another one.
	 * @param name           Name to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const R::RString& name) const { return(Name.Compare(name)); }

	/**
	 * @return the name of the cell.
	 */
	R::RString GetName(void) const {return(Name);}

	/**
	 * Set if the cell is an abstract one.
	 * @param abstract       Abstract ?
	 */
	void SetAbstract(bool abstract);

	/**
	 * @return pointer to the port corresponding to a given name.
	 * @param name           Name of the port.
	 */
	RPort* GetPort(const R::RString& name) const { return(Ports.GetPtr(name)); }

	/**
	 * @return a cursor over the ports.
	 */
	R::RCursor<RPort> GetPorts(void) const {return(R::RCursor<RPort>(Ports));}

	/**
	 * Add a polygon to describe the cell.
	 * @param poly           Polygon.
	 */
	void InsertPolygon(R::RPolygon* poly);

	/**
	 * Insert a port with a given name.
	 * @param name
	 * @param dir
	 */
	void InsertPort(const R::RString& name,const char dir);

	/**
	 * Create a new with a given name.
	 * @param name           Name of the net.
	 * @return Pointer to the net created.
	 */
	RNet* InsertNet(const R::RString &name);

	/**
	 * Insert a instance.
	 * @param name           Name of the instance.
	 * @param cell           Name of the corresponding cell.
	 */
	void InsertInstance(const R::RString& name,const R::RString& cell);

	/**
	 * Get a instance.
	 * @param name           Name of the instance.
	 * @return a pointer to the instance (or numl if not found).
	 */
	RInstance* GetInstance(const R::RString& name) const {return(Instances.GetPtr(name));}

	/**
	 * @return the number of instances.
	 */
	size_t GetNbInstances(void) const {return(Instances.GetNb());}

	/**
	 * @return a cursor over the instances.
	 */
	R::RCursor<RInstance> GetInstances(void) const {return(R::RCursor<RInstance>(Instances));}

	/**
	 * @return a cursor over the polygons.
	 */
	R::RCursor<R::RPolygon> GetPolygons(void) const {return(R::RCursor<R::RPolygon>(Polygons));}

	/**
	 * Destructor of the cell.
	 * @return
	 */
	~RCell(void);
};


//-----------------------------------------------------------------------------
/**
 * The RInstance class provides a representation for an instance of a cell.
 * @author Pascal Francq
 * @short Cell Instance
 */
class RInstance
{
	/**
	 * Corresponding cell.
	 */
	RCell* Cell;

	/**
	 * Reference to the ports.
	 */
	R::RContainer<RPortRef,true,true>* PortRefs;

	/**
	 * Name of the instance.
	 */
	R::RString Name;

public:

	/**
	 * Constructor of the instance.
	 * @param name           Name of the instance.
	 */
	RInstance(const R::RString& name);

	/**
	 * Constructor of the instance.
	 * @param name           Name of the instance.
	 * @param cell           Cell owning the instance.
	 */
	RInstance(const R::RString& name,RCell* cell);

	/**
	 * Compare two instances.
	 * @param inst           Instance to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const RInstance& inst) const { return(Name.Compare(inst.Name)); }

	/**
	 * Compare the name of an instance with another one.
	 * @param name           Name to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const R::RString& name) const { return(Name.Compare(name)); }

	/**
	 * @return the name of the instance.
	 */
	R::RString GetName(void) const {return(Name);}

	/**
	 * @return pointer of the cell.
	 */
	RCell* GetCell(void) const {return(Cell);}
};


//-----------------------------------------------------------------------------
/**
 * The RPortRef provides a representation for a port of a particular instance.
 * @author Pascal Francq
 * @short Instance Port.
 */
class RPortRef
{
	/**
	 * Port of the corresponding cell.
	 */
	RPort* Port;

	/**
	 * Instance of the port.
	 */
	RInstance* Instance;

public:

	/**
	 * Create a port of an instance.
	 * @param port           Generic port.
	 * @param inst           Instance.
	 */
	RPortRef(RPort* port,RInstance* inst);

	/**
	 * Compare a instance port with another one.
	 * @param ref            Port to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const RPortRef& ref) const {return(Port->Compare(*ref.Port));}
};


//-----------------------------------------------------------------------------
/**
 * the RNet class provides a representation of a net involving different
 * instance ports.
 * @author Pascal Francq
 * @short Net.
 */
class RNet
{
	/**
	 * Name of the net.
	 */
	R::RString Name;

	/**
	 * Cell owning the net.
	 */
	RCell* Owner;

	/**
	 * Ports involved in the net.
	 */
	R::RContainer<RPortRef,true,false> Refs;

public:

	/**
	 * Construct a net.
	 * @param name           Name of the net.
	 * @param owner          Owner of the net.
	 */
	RNet(const R::RString &name,RCell* owner);

	/**
	 * Compare two nets.
	 * @param net            Net to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const RNet& net) const { return(Name.Compare(net.Name)); }

	/**
	 * Compare the name of the net with another one.
	 * @param name           Name to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const R::RString &name) const { return(Name.Compare(name)); }

	/**
	 * Insert a particular port.
	 * @param port           Name of the port.
	 * @param instance       Name of the instance.
	 */
	void InsertRef(const R::RString& port,const R::RString& instance);

	/**
	 * Destructor of the net.
	 */
	~RNet(void);
};


//-----------------------------------------------------------------------------
/**
 * The RStructure class provides a representation for the structure of a VLSI
 * problem.
 * @author Pascal Francq
 * @short VLSI Structure.
 */
class RStructure
{
protected:

	/**
	 * The libraries.
	 */
	R::RContainer<RLibrary,true,true> Libraries;

	/**
	 * The cells.
	 */
	R::RContainer<RCell,true,true> Cells;

	/**
	 * Units used.
	 */
	R::tCoord Units;

public:

	/**
	 * Constructor of the structure.
	 */
	RStructure(void);

	/**
	 * Create a cell.
	 * @param name           Name of the cell.
	 * @param lib            Library of the cell.
	 * @return a pointer to the cell created.
	 */
	RCell* CreateCell(const R::RString& name,RLibrary* lib);

	/**
	 * Look if a cell exist or create one.
	 * @param name           Name of the cell.
	 * @return a pointer to the cell.
	 */
	RCell* GetCell(const R::RString& name);

	/**
	 * Look if a library exist or create one.
	 * @param name           Name of the library.
	 * @return a pointer to the library.
	 */
	RLibrary* GetLibrary(const R::RString& name);

	/**
	 * Destructor of the structure.
	 */
	~RStructure(void);
};


}  //-------- End of namespace RVLSI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
