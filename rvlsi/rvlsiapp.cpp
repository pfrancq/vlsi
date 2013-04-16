/*

	RVLSI Project Library

	Application handling a VLSI Session - Implementing.

	Copyright 2000-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2003 by the Université Libre de Bruxelles (ULB).

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
// include files for R Project
#include <rxmlparser.h>


//------------------------------------------------------------------------------
// include files for RVLSI Project
#include <rvlsiapp.h>
#include <rxmlparser.h>
using namespace R;
using namespace RVLSI;
using namespace std;


//------------------------------------------------------------------------------
// Global variables
RVLSIApp* RVLSI::VLSIApp=0;



//------------------------------------------------------------------------------
//
// class VLSIParser
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class VLSIParser : public RXMLParser
{
	enum tSection {Undefined,Circuit,Masters,Instances,Connections};
	enum tTag {unknown,main,circuit,masters,master,polygon,terminal,pin,instances,instance,connections,net,connect};
	enum tAttr {noattr,points,id,layer,masterattr,instanceattr,terminalattr};
	tSection Section;
	tTag Tag;
	tAttr Attr;
	RProblem2D* Problem;
	RPolygon* Polygon;
	RObj2D* Obj;
	RStack<RString,true,true,true> Ids;
	RPoint Tr;
	bool ReadPolygon;
	RString AttrValue;
	RString AttrValue2;
	bool Pin;
	RVectorBool Read;
	RConnection* Connection;
	RObj2DConnector* Connector;
	RObj2DConfigConnector* ConnectorConfig;
	RContainer<RObj2D,true,true> Templates;

public:
	VLSIParser(RProblem2D* problem,const RURI& uri);
	tTag GetTagType(const RString& ns,const RString& name);
	tAttr GetAttrType(const RString& ns,const RString& name);
	virtual void BeginTag(const RString &namespaceURI, const RString &lName, const RString &name);
	virtual void BeginTagParsed(const RString &namespaceURI, const RString &lName, const RString &name);
	virtual void EndTag(const RString &namespaceURI, const RString &lName, const RString &name);
	virtual void AddAttribute(const RString &namespaceURI, const RString &lName, const RString &name);
	virtual void Value(const RString &value);
};


//------------------------------------------------------------------------------
VLSIParser::VLSIParser(RProblem2D* problem,const RURI& uri)
	: RXMLParser(uri), Problem(problem), Obj(0), Ids(20), Pin(false), Read(10), Templates(50)
{
	Section=Undefined;
	Read.Init(10,false);
}


//------------------------------------------------------------------------------
VLSIParser::tTag VLSIParser::GetTagType(const RString& ns,const RString& name)
{
	if(name=="vlsi")
		return(main);
	if(ns=="http://www.w3.org/2000/svg")
	{
		if(name=="polygon")
			return(polygon);
	}
	else if(ns=="http://www.otlet-institute.org/vlsi")
	{
		if(name=="circuit")
			return(circuit);
		if(name=="terminal")
			return(terminal);
		if(name=="pin")
			return(pin);
		if(name=="masters")
			return(masters);
		if(name=="master")
			return(master);
		if(name=="instances")
			return(instances);
		if(name=="instance")
			return(instance);
		if(name=="connections")
			return(connections);
		if(name=="net")
			return(net);
		if(name=="connect")
			return(connect);
	}
	return(unknown);
}


//------------------------------------------------------------------------------
VLSIParser::tAttr VLSIParser::GetAttrType(const RString& ns,const RString& name)
{
	if((name=="xmlns")||(name=="svg"))
		return(noattr);
	else if(ns=="http://www.w3.org/2000/svg")
	{
		if(name=="points")
			return(points);
	}
	else if(ns=="http://www.otlet-institute.org/vlsi")
	{
		if(name=="id")
			return(id);
		if(name=="layer")
			return(layer);
		if(name=="master")
			return(masterattr);
		if(name=="instance")
			return(instanceattr);
		if(name=="terminal")
			return(terminalattr);
	}
	throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute '"+name+"'");
}


//------------------------------------------------------------------------------
void VLSIParser::BeginTag(const RString &namespaceURI, const RString &lName, const RString& name)
{
	Tag=GetTagType(namespaceURI,lName);
	if(Tag==unknown)
		throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid tag '"+name+"'");

	switch(Section)
	{
		case Undefined:
		{
			switch(Tag)
			{
				case main:
					return;

				case circuit:
					Section=Circuit;
					ReadPolygon=false;
					Obj=0;
					break;

				case masters:
					Section=Masters;
					break;

				case instances:
					if(!Read[Masters])
						throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): 'instances' cannot precede 'masters'");
					Section=Instances;
					break;

				case connections:
					if(!Read[Instances])
						throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): 'connections' cannot precede 'instances'");
					Section=Connections;
					break;

				default:
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid tag '"+name+"' as root child");
			}
			break;
		}

		case Circuit:
		{
			switch(Tag)
			{
				case polygon:
					AttrValue.Clear();
					break;

				case pin:
					Pin=true;
					Polygon=new RPolygon(2);
					break;

				case terminal:
					Connector=0;
					if(!ReadPolygon)
						throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): 'polygon' tag must precede '"+name+"'");
					break;

				default:
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid tag '"+name+"' in 'circuit'");
			}
			break;
		}

		case Masters:
		{
			switch(Tag)
			{
				case master:
					ReadPolygon=false;
					Obj=0;
					break;

				case terminal:
					Connector=0;
					if(!ReadPolygon)
						throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): 'polygon' tag must precede '"+name+"'");
					break;

				case polygon:
					AttrValue.Clear();
					break;

				case pin:
					Pin=true;
					Polygon=new RPolygon(2);
					break;

				default:
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid tag '"+name+"' in 'masters'");
			}
			break;
		}

		case Instances:
		{
			switch(Tag)
			{
				case instance:
					Obj=0;
					AttrValue.Clear();
					break;

				default:
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid tag '"+name+"' in 'instances'");
					break;
			}
			break;
		}

		case Connections:
		{
			switch(Tag)
			{
				case net:
					Connection=0;
					break;

				case connect:
					AttrValue.Clear();
					AttrValue2.Clear();
					break;

				default:
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid tag '"+name+"' in 'connections'");
					break;
			}
			break;
		}

		default:
			throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Problem with '"+name+"'");
	}

//	cout<<"Begin: "<<namespaceURI<<" "<<name<<endl;
}


//------------------------------------------------------------------------------
void VLSIParser::BeginTagParsed(const RString &namespaceURI, const RString &lName, const RString&)
{
	Tag=GetTagType(namespaceURI,lName);
	switch(Tag)
	{
		case circuit:
			Polygon=new RPolygon(4);
			break;

		case master:
			Obj=new RObj2D(Templates.GetNb(),*Ids());
			Ids.Pop();
			Templates.InsertPtr(Obj);
			Polygon=new RPolygon(8);
			break;

		case terminal:
		{
			if(Obj)
			{
				// Normal object
				Connector=new RObj2DConnector(Obj,Obj->GetNbConnectors(),*Ids());
				Ids.Pop();
				ConnectorConfig=new RObj2DConfigConnector(Connector);
				Obj->GetDefaultConfig()->Add(ConnectorConfig);
			}
			else
			{
				// Circuit
				Connector=Problem->InsertConnector(Problem->GetNbConnectors(),*Ids());
				ConnectorConfig=Problem->GetDefaultConfig()->GetConnector(*Ids());
				Ids.Pop();
			}
			break;
		}

		case instance:
		{
			// Find the template
			RObj2D* Template(Templates.GetPtr(AttrValue,false));
			if(!Template)
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Master '"+AttrValue+"' does not exist");

			// Create the object
			Obj=new RObj2D(Problem->GetNbObjs(),*Ids());
			Ids.Pop();
			Problem->Insert(Obj);
			Obj->Copy(*Template);

			// Initialize the object
			Obj->CreateOri(oNormal);
			Obj->CreateOri(oNormalX);
			Obj->CreateOri(oNormalY);
			Obj->CreateOri(oNormalYX);
			Obj->CreateOri(oRota90);
			Obj->CreateOri(oRota90X);
			Obj->CreateOri(oRota90Y);
			Obj->CreateOri(oRota90YX);

			break;
		}

		case net:
		{
			Connection=new RConnection(*Ids());
			Problem->Insert(Connection);
		}
			break;

		default:
			break;
	}
}


//------------------------------------------------------------------------------
void VLSIParser::EndTag(const RString &namespaceURI, const RString &lName, const RString &)
{
	Tag=GetTagType(namespaceURI,lName);
	switch(Tag)
	{
		case circuit:
			Obj=0;
			Read[Circuit]=true;
			Section=Undefined;
			break;

		case polygon:
		{
			// Read the points
			bool Ok;
			RChar* Car(AttrValue());
			while(!Car->IsNull())
			{
				// Skip Spaces
				while((!Car->IsNull())&&(Car->IsSpace()))
					Car++;

				// Read X
				RString ReadX;
				while((!Car->IsNull())&&(!Car->IsSpace())&&((*Car)!=','))
					ReadX+=(*(Car++));
				tCoord X=ReadX.ToDouble(Ok);
				if(!Ok)
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute for 'points'");

				// Skip Spaces, the comma and spaces
				while((!Car->IsNull())&&(Car->IsSpace()))
					Car++;
				if((Car->IsNull())||((*Car)!=','))
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute for 'points'");
				Car++; // Skip ','
				while((!Car->IsNull())&&(Car->IsSpace()))
					Car++;

				// Read Y
				RString ReadY;
				while((!Car->IsNull())&&(!Car->IsSpace())&&((*Car)!=','))
					ReadY+=(*(Car++));
				tCoord Y=ReadY.ToDouble(Ok);
				if(!Ok)
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute for 'points'");

				// Add the point
				Polygon->InsertVertex(new RPoint(X,Y));
			}

			// Make computations
			if(!Pin)
			{
				if(Obj)
				{
					// Normal polygon
					Tr=Polygon->Calibrate();
					Polygon->ReOrder();
					Obj->GetDefaultConfig()->Set(*Polygon);
				}
				else
				{
					// It is the circuit <- get only the size
					if(Polygon->GetNbVertices()!=1)
						throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Circuit must defined only the size");
					Problem->SetLimits(RSize((*Polygon)[0]->X,(*Polygon)[0]->Y));
				}
				delete Polygon;
				Polygon=0;
				ReadPolygon=true;
			}

			break;
		}

		case pin:
		{
			if(Polygon->GetNbVertices()!=2)
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): 'pin' defines "+RString::Number(Polygon->GetNbVertices())+" points rather than two");
			RObj2DPin* pin(new RObj2DPin(Connector,Connector->GetNb(),*Ids()));
			//Ids.Pop();
			RRect rect(*(*Polygon)[0],*(*Polygon)[1]);
			ConnectorConfig->InsertPtr(new RObj2DConfigPin(pin,rect));
			delete Polygon;
			Polygon=0;
			Pin=false;
			break;
		}

		case terminal:
		{
			Connector=0;
			ConnectorConfig=0;
			break;
		}

		case master:
			Obj=0;
			break;

		case masters:
			Read[Masters]=true;
			Section=Undefined;
			break;

		case instances:
			Read[Instances]=true;
			Section=Undefined;
			break;

		case connections:
			Read[Connections]=true;
			Section=Undefined;
			break;

		case connect:
		{
			RObj2DConnector* con;
			if(AttrValue.IsEmpty())
			{
				// Connection of a connector of the board
				con=Problem->GetDefaultConfig()->GetConnector(AttrValue2)->GetConnector();
				if(!con)
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Wrong terminal identifier '"+AttrValue2+"'");
			}
			else
			{
				// Connection of a connector of an object
				RObj2D* obj(Problem->GetObj(AttrValue));
				if(!obj)
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Wrong object identifier '"+AttrValue+"'");
				con=obj->GetConnector(AttrValue2,false);
				if(!con)
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Wrong terminal identifier '"+AttrValue2+"'");
			}
			Connection->Insert(con);
			break;
		}

		default:
			break;
	}
}


//------------------------------------------------------------------------------
void VLSIParser::AddAttribute(const RString& namespaceURI,const RString & lName, const RString &)
{
	Attr=GetAttrType(namespaceURI,lName);
	switch(Attr)
	{
		case points:
			if(Tag!=polygon)
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute '"+lName+"'");
			break;

		case id:
			if((Tag!=terminal)&&(Tag!=master)&&(Tag!=instance)&&(Tag!=pin)&&(Tag!=net))
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute '"+lName+"'");
			Ids.Push(new RString());
			break;

		case masterattr:
			if(Tag!=instance)
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute '"+lName+"'");
			break;

		case instanceattr:
			if(Tag!=connect)
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute '"+lName+"'");
			break;

		case terminalattr:
			if(Tag!=connect)
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute '"+lName+"'");
			break;

		case noattr:
		case layer:
			// Nothing to do
			break;
	}
}


//------------------------------------------------------------------------------
void VLSIParser::Value(const RString& value)
{
	switch(Attr)
	{
		case instanceattr:
		case masterattr:
		case points:
			AttrValue+=value;
			break;

		case terminalattr:
			AttrValue2+=value;

		case id:
			(*Ids())+=value;
			break;

		case noattr:
		case layer:
			// Nothing to do
			break;
	}
}



//------------------------------------------------------------------------------
//
// class RVLSIApp
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RVLSIApp::RVLSIApp(const RString& name,int argc, char *argv[])
	: RApplication(name,argc,argv), Log(0), Debug(0), Session(0),VLSIConfig()
{
	VLSIApp=this;
}


//-----------------------------------------------------------------------------
void RVLSIApp::CreateConfig(void)
{
	VLSIConfig.InsertParam(new RParamValue("Log File","/home/pfrancq/vlsi.log"));
	VLSIConfig.InsertParam(new RParamValue("Debug File","/home/pfrancq/debug-vlsi.xml"));
	VLSIConfig.InsertParam(new RParamValue("Population Size",16));
	VLSIConfig.InsertParam(new RParamValue("Max Gen",30));
	VLSIConfig.InsertParam(new RParamValue("Step",false));
	VLSIConfig.InsertParam(new RParamValue("ComputeFreePolygons","false"));
	VLSIConfig.InsertParam(new RParamValue("UseFreePolygons","false"));
	VLSIConfig.InsertParam(new RParamValue("AllOrientations","false"));
	VLSIConfig.InsertParam(new RParamValue("Heuristic","Bottom-Left"));
	VLSIConfig.InsertParam(new RParamValue("Step Gen",0));
	VLSIConfig.InsertParam(RPromLinearCriterion::CreateParam("HeurDist"));
	VLSIConfig.InsertParam(RPromLinearCriterion::CreateParam("HeurArea"));
	VLSIConfig.InsertParam(RPromLinearCriterion::CreateParam("SelectDist"));
	VLSIConfig.InsertParam(RPromLinearCriterion::CreateParam("SelectWeight"));
}


//------------------------------------------------------------------------------
void RVLSIApp::Init(void)
{
	// Parent initialization
	RApplication::Init();
	VLSIConfig.SetConfigInfos("lib/vlsi","default");

	// Get the parameters
	VLSIConfig.Load();
	LogFileName=VLSIConfig.Get("Log File");
	DebugFileName=VLSIConfig.Get("Debug File");
	PopSize=VLSIConfig.GetUInt("Population Size");
	MaxGen=VLSIConfig.GetUInt("Max Gen");
	Step=VLSIConfig.GetBool("Step");
	ComputeFreePolygons=VLSIConfig.GetBool("ComputeFreePolygons");
	UseFreePolygons=VLSIConfig.GetBool("UseFreePolygons");
	AllOrientations=VLSIConfig.GetBool("AllOrientations");
	Heuristic=VLSIConfig.Get("Heuristic");
	StepGen=VLSIConfig.GetUInt("Step Gen");
	HeurDist=VLSIConfig.FindParam<R::RParamStruct>("HeurDist");
	HeurArea=VLSIConfig.FindParam<R::RParamStruct>("HeurArea");
	SelectDist=VLSIConfig.FindParam<R::RParamStruct>("SelectDist");
	SelectWeight=VLSIConfig.FindParam<R::RParamStruct>("SelectWeight");

	try
	{
		// Create (if necessary) Log file
		if(LogFileName!=RString::Null)
		{
			//cout<<"Create log file "<<LogFileName<<"...";
			cout.flush();
			Log=new RTextFile(LogFileName,"utf8");
			Log->Open(RIO::Append);
			//cout<<"OK"<<endl;
		}
	}
	catch(...)
	{
		std::cerr<<"Error: Cannot create log file "<<LogFileName<<std::endl;
	}

	try
	{
		// Create (if necessary) the debug file
		if(DebugFileName!=RString::Null)
			Debug=new RDebugXML(DebugFileName);
	}
	catch(...)
	{
		std::cerr<<"Error: Cannot create debug file "<<DebugFileName<<std::endl;
	}
}


//------------------------------------------------------------------------------
RProblem2D* RVLSIApp::CreateSession(const RString& file)
{
	if(!IsInit())
		ThrowRException("Application not initialized");
	if(Session)
		ThrowRException("A session is already created");

	// Initialize Session
	Session=new RProblem2D("VLSI Board");

	// Read the file
	VLSIParser File(Session,file);
	File.Open(RIO::Read);

	// Initialize
	Session->DetermineBoard();

	WriteLog("Session created");

	return(Session);
}


//------------------------------------------------------------------------------
void RVLSIApp::DeleteSession(void)
{
	if(!Session)
		return;
	delete Session;
	Session=0;
}


//------------------------------------------------------------------------------
void RVLSIApp::WriteLog(const RString& str)
{
	if(Log)
		Log->WriteLog(str);
}


//------------------------------------------------------------------------------
void RVLSIApp::Apply(void)
{
	VLSIConfig.Set("Log File",LogFileName);
	VLSIConfig.Set("Debug File",DebugFileName);
	VLSIConfig.SetUInt("Population Size",PopSize);
	VLSIConfig.SetUInt("Max Gen",MaxGen);
	VLSIConfig.SetBool("Step",Step);
	VLSIConfig.SetBool("ComputeFreePolygons",ComputeFreePolygons);
	VLSIConfig.SetBool("UseFreePolygons",UseFreePolygons);
	VLSIConfig.SetBool("AllOrientations",AllOrientations);
	VLSIConfig.Set("Heuristic",Heuristic);
	VLSIConfig.SetUInt("Step Gen",StepGen);
}


//------------------------------------------------------------------------------
RVLSIApp::~RVLSIApp(void)
{
	// No more App
	VLSIApp=0;

	// Delete the session
	DeleteSession();

	// Get the parameters back
	Apply();
	VLSIConfig.Save();

	if(Log)
		Log->WriteLog("Session deleted");
	delete Debug;
	delete Log;
}
