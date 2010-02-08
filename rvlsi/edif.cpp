/*

	RVLSI Project Library

	Edif.cpp

	EDIF file - Implementation.

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
// include files for ANSI C/C++
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#if defined(_BSD_SOURCE) || defined(__GNUC__) || defined(__APPLE_)
	#include <unistd.h>
#else
	#include <io.h>
#endif
#include <fcntl.h>


//------------------------------------------------------------------------------
// include files for RVLSI Project
#include <edif.h>
using namespace R;
using namespace RVLSI;
using namespace std;



//------------------------------------------------------------------------------
//
// General Functions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
inline bool MustIncBuffer(char c)
{
	if(c==' ') return(true);         // Space
	if(c=='\n') return(true);        // Line Feed
	if(c=='\t') return(true);        // Tab
	if(c=='\r') return(true);        // Carriage Return
	return(false);
}



//------------------------------------------------------------------------------
//
// Declarations
//
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class REDIFFile::StringId : public RString
{
public:
	/**
	 * Type of the EDIF element treated.
	 */
	enum tType
	{
		tUnknown,            /** Unknown.*/
		tCell,               /** Cell. */
		tLibrary,            /** Library.*/
		tInstance,           /** Instance.*/
		tPortImp,            /** Port implementation.*/
		tNet,                /** Net.*/
		tPort                /** Port.*/
	};

	tType Type;

	StringId(const RString& str,tType type) : RString(str), Type(type) {}
};


//-----------------------------------------------------------------------------
class REDIFFile::EDIFNode : public RNode<REDIFFile::EDIFTree,EDIFNode,true>
{
public:
	RString TagName;
	StringId::tType Type;
	RString Params;

	EDIFNode(REDIFFile::EDIFTree& owner,EDIFNode* parent,RTextFile& in);
	virtual int Compare(const EDIFNode&) const {return(-1);}
	virtual int Compare(const RNode<REDIFFile::EDIFTree,EDIFNode,true>&) const {return(0);}
	virtual int Compare(const RString& name) const { return(TagName.Compare(name)); }
	void InsertInst(REDIFFile*);
	void InsertPortImp(REDIFFile*);
	void InsertNet(REDIFFile*);
	void Analyse(REDIFFile*);
};


//------------------------------------------------------------------------------
class REDIFFile::EDIFTree : public RTree<REDIFFile::EDIFTree,REDIFFile::EDIFNode,true>
{
public:
	RContainer<StringId,true,true>& Types;

	EDIFTree(REDIFFile* owner) : RTree<EDIFTree,REDIFFile::EDIFNode,true>(50), Types(owner->Types) {}
};



//------------------------------------------------------------------------------
//
// Class REDIFTag
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
REDIFFile::EDIFNode::EDIFNode(REDIFFile::EDIFTree& owner,EDIFNode* parent,RTextFile& in)
	: RNode<REDIFFile::EDIFTree,EDIFNode,true>(&owner)
{
	// Insert it
	owner.InsertNode(parent,this);

	// First character must be a (
	if(in.GetChar()!='(')
		ThrowRIOException(&in,"'(' expected");
	in.SkipSpaces();

	// Read the Tag name
	TagName=in.GetToken("()");
	StringId* Ptr(owner.Types.GetPtr(TagName.ToUpper()));
	if(Ptr)
		Type=Ptr->Type;
	else
		Type=StringId::tUnknown;
	in.SkipSpaces();

	// Look if the next character is ( or )
	bool LookParent(true);
	while((!in.End())&&((!LookParent)||(LookParent&&(in.GetNextChar()!='(')&&(in.GetNextChar()!=')'))))
	{
		if(in.GetNextChar()=='"')
			LookParent=!LookParent;

		// Some parameters to read
		Params+=in.GetChar();
	}
	Params=Params.Trim();

	// There must be something
	if(in.End())
		ThrowRIOException(&in,"')' or '(' expected");

	RChar Car(in.GetNextChar());

	// if ')' -> no child tags
	if(Car==')')
	{
		in.GetChar(); // Read ')'
		in.SkipSpaces();
		return;
	}
	if(Car!='(')
		ThrowRIOException(&in,"')' or '(' expected");

	// Read the child nodes
	while((!in.End())&&(in.GetNextChar()!=')'))
		new EDIFNode(owner,this,in);

	if(in.End()||(in.GetChar()!=')'))
		ThrowRIOException(&in,"')' expected");

	in.SkipSpaces();
}


//------------------------------------------------------------------------------
void REDIFFile::EDIFNode::InsertInst(REDIFFile* owner)
{
	EDIFNode* ptr(GetNode("name"));
	if(!ptr) return;
	RString Name(ptr->Params);
	ptr=GetNode("viewRef");
	if(!ptr) return;
	ptr=ptr->GetNode("cellRef");
	if(!ptr) return;
	RString Ref(ptr->Params);
	owner->CurrCell->InsertInstance(Name,Ref);
}


//------------------------------------------------------------------------------
void REDIFFile::EDIFNode::InsertPortImp(REDIFFile*)
{
}


//------------------------------------------------------------------------------
void REDIFFile::EDIFNode::InsertNet(REDIFFile* owner)
{
	if(!(owner->CurrCell)) return;
	RNet* net(owner->CurrCell->InsertNet(Params));
	EDIFNode* join(GetNode("joined"));
	if(!join) return;
	RCursor<EDIFNode> ptr(join->GetNodes());
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		if(ptr()->TagName=="portRef") continue;
		RString PortName(ptr()->Params);
		RCursor<EDIFNode> Cur(ptr()->GetNodes());
		Cur.Start();
		RString InstName;
		if(!Cur.End())
		{
			EDIFNode* sub(Cur());
			if(sub->TagName=="instanceRef") continue;
			InstName=sub->Params;
		}
		else
			InstName=owner->CurrCell->GetName();
		net->InsertRef(PortName,InstName);
	}
}


//------------------------------------------------------------------------------
void REDIFFile::EDIFNode::Analyse(REDIFFile* owner)
{
	bool bSub=true;
	char dir;

	switch(Type)
	{
		case StringId::tCell:                // This is a cell
			owner->CurrCell=owner->Project->CreateCell(Params,owner->CurrLib);
			break;
		case StringId::tLibrary:             // This is a library
			owner->CurrLib=owner->Project->GetLibrary(Params);
			break;
		case StringId::tInstance:                // This is a instance of the current Cell
			InsertInst(owner);
			owner->CurrCell->SetAbstract(false);
			bSub=false;
			break;
		case StringId::tPortImp:
			InsertPortImp(owner);
			bSub=false;
			break;
		case StringId::tNet:
			InsertNet(owner);
			bSub=false;
			break;
		case StringId::tPort:                // This is a port of the current Cell
			dir=2;
			RCursor<EDIFNode> Cur(GetNodes());
			Cur.Start();
			if(!Cur.End())
			{
				if(Cur()->Params=="INPUT") dir=-1;
				if(Cur()->Params=="OUTPUT") dir=+1;
				if(Cur()->Params=="INOUT") dir=0;
				owner->CurrCell->InsertPort(Params,dir);
				bSub=false;
			}
			break;
	}

	if(!bSub) return;
	RCursor<EDIFNode> tag(GetNodes());
	for(tag.Start();!tag.End();tag.Next())
		tag()->Analyse(owner);

	switch(Type)
	{
		case StringId::tCell:                // This is a cell
			owner->CurrCell=0;
			break;
		case StringId::tLibrary:             // This is a library
			owner->CurrLib=0;
			break;
	}
}



//------------------------------------------------------------------------------
//
// Class REDIFFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
REDIFFile::REDIFFile(RProject* project,const RURI& uri)
	: RDataFile(project,uri), CurrCell(0), CurrLib(0),
	  Types(10)
{
	Type=vdtEDIF2;
	Types.InsertPtr(new StringId("CELL",StringId::tCell));
	Types.InsertPtr(new StringId("LIBRARY",StringId::tLibrary));
	Types.InsertPtr(new StringId("INSTANCE",StringId::tInstance));
	Types.InsertPtr(new StringId("PORTIMPLEMENTATION",StringId::tPortImp));
	Types.InsertPtr(new StringId("NET",StringId::tNet));
	Types.InsertPtr(new StringId("PORT",StringId::tPort));
}


//------------------------------------------------------------------------------
void REDIFFile::Analyse(R::RTextFile*)
{
	// Read the file
	RTextFile In(URI);
	In.Open(RIO::Read);
	EDIFTree Tree(this);
	new EDIFNode(Tree,0,In);

	// Analyze the nodes
	RCursor<EDIFNode> Nodes(Tree.GetTop()->GetNodes());
	for(Nodes.Start();!Nodes.End();Nodes.Next())
		Nodes()->Analyse(this);
}


//------------------------------------------------------------------------------
REDIFFile::~REDIFFile(void)
{
}
