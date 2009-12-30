/*

	RVLSIApp.cpp

	Application handling a VLSI Session - Implementing.

	Copyright 2000-2009 by Pascal Francq (pascal@francq.info).
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
// include files for RVLSI
#include <rvlsiapp.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// Global variables
RVLSIApp* R::VLSIApp=0;



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

	// Init OK
	HasInitApp=true;
}


//------------------------------------------------------------------------------
RProblem2D* RVLSIApp::CreateSession(const RString& file)
{
	if(!HasInitApp)
		ThrowRException("Application not initialized");
	if(Session)
		ThrowRException("A session is already created");

	// Initialize Session
	Session=new RProblem2D(file);
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
	Config.Set("PlugIns Config","default");
	VLSIConfig.Set("lib/vlsi","default");
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
