/*

	RVLSI Project Library

	Files.cpp

	VLSI input and output Files - Implementation.

	Copyright 1998-2012 by Pascal Francq (pascal@francq.info).
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
// include files for R Project
#include <rtextfile.h>


//------------------------------------------------------------------------------
// include files for RVLSI Project
#include <files.h>
#include <edif.h>
#include <gds.h>
using namespace R;
using namespace RVLSI;
using namespace std;



//------------------------------------------------------------------------------
//
// Class RDataFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDataFile::RDataFile(RProject* project,const RURI& uri)
	: Project(project), URI(uri), Type(vdtNothing)
{
	if(!Project)
		mThrowRException("Cannot have a null project");
}


//------------------------------------------------------------------------------
int RDataFile::Compare(const RDataFile& file) const
{
	if(Type==file.Type)
		return(URI.Compare(file.URI));
	else
	 return(Type-file.Type);
}


//------------------------------------------------------------------------------
RDataFile::~RDataFile(void)
{
}



//------------------------------------------------------------------------------
//
// Class RProject
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RProject::RProject(const RURI& uri)
	: RContainer<RDataFile,true,true>(10,5), RStructure(), URI(uri)
{
}


//------------------------------------------------------------------------------
void RProject::Analyse(void)
{
	// Open the file
	RTextFile f(URI);
	f.SetRem("#");
	f.SetRemStyle(RTextFile::SingleLineComment);
	f.Open(RIO::Read);

	while(!f.End())
	{
		// Each line is in the form : file=type
		RString Line(f.GetLine());
		int pos(Line.Find('='));
		if(pos==-1)
			mThrowRIOException(&f,"Each line must have the form : 'file'='type'");
		RString Type(Line.Mid(pos+1));
		RURI File(Line.Mid(0,pos));
		if(Type=="PL2D")
		{
			if(!PL2D().IsEmpty())
				mThrowRIOException(&f,"PL2D file already defined");
			PL2D=File;
		}
		else if(Type=="LOG")
		{
			if(!LogName().IsEmpty())
				mThrowRIOException(&f,"log file already defined");
			LogName=File;
		}
		else if(Type=="GDSII")
		{
			if(!RFile::Exists(File))
				mThrowRIOException(&f,"File '"+File()+"' does not exist");
			InsertPtr(new RGDSFile(this,Line.Mid(0,pos)));
		}
		else if(Type=="EDIF2")
		{
			if(!RFile::Exists(File))
				mThrowRIOException(&f,"File '"+File()+"' does not exist");
			InsertPtr(new REDIFFile(this,Line.Mid(0,pos)));
		}
		else
			mThrowRIOException(&f,"Type must be 'PL2D', 'LOG', 'GDSII' or 'EDIF2");
	}
	if(PL2D().IsEmpty())
		mThrowRIOException(&f,"File does not contain a PL2D file");

	// Log file
	RTextFile* Log(0);
	if(!LogName().IsEmpty())
	{
		Log=new RTextFile(LogName);
		Log->Open(RIO::Create);
	}

	// Analyzing the files
	RCursor<RDataFile> file(*this);
	for(file.Start();!file.End();file.Next())
	{
//		cout<<"  "<<file()->GetURI()()<<" ...";
		file()->Analyse(Log);
//		cout<<"  OK"<<endl;
	}

	// Create PL2D file
	RTextFile Out(PL2D,"utf-8");
	Out.Open(RIO::Create);
	Out<<"<?xml version=\"1.0\"?>"<<endl;
	Out<<"<!DOCTYPE vlsi>"<<endl;
	Out<<"<vlsi xmlns=\"http://www.otlet-institute.org/vlsi\" xmlns:svg=\"http://www.w3.org/2000/svg\">"<<endl;
	Out<<"\t<circuit>"<<endl;
	Out<<"\t</circuit>"<<endl;
	Out<<"\t<masters>"<<endl;
	RCursor<RCell> Cell(Cells);
	for(Cell.Start();!Cell.End();Cell.Next())
	{
		Out<<"\t\t<master id=\""+Cell()->GetName()+"\">"<<endl;
		RCursor<RPolygon> Polygon(Cell()->GetPolygons());
		for(Polygon.Start();!Polygon.End();Polygon.Next())
		{
			RString Tmp("\t\t\t<svg:polygon svg:points=\"");
			RCursor<RPoint> Cur(Polygon()->GetVertices());
			for(Cur.Start();!Cur.End();)
			{
				Tmp+=RString::Number(Cur()->X)+","+RString::Number(Cur()->Y);
				Cur.Next();
				if(!Cur.End())
					Tmp+=" ";
			}
			Out<<Tmp+"\" />"<<endl;
		}
		RCursor<RPort> Port(Cell()->GetPorts());
		for(Port.Start();!Port.End();Port.Next())
		{
			Out<<"\t\t\t<terminal id=\""+Port()->GetName()+"\">"<<endl;
			/*<pin id="Pin1" layer="Met1">
				<svg:polygon svg:points="3,3 4,4" />
			</pin>*/
			Out<<"\t\t\t</terminal>"<<endl;
		}
		Out<<"\t\t</master>"<<endl;
	}
	Out<<"\t</masters>"<<endl;
	Out<<"\t<instances>"<<endl;
	for(Cell.Start();!Cell.End();Cell.Next())
	{
		RCursor<RInstance> Inst(Cell()->GetInstances());
		for(Inst.Start();!Inst.End();Inst.Next())
		{
		}
	}
	Out<<"\t</instances>"<<endl;
	Out<<"\t<connections>"<<endl;
	Out<<"\t</connections>"<<endl;
	Out<<"</vlsi>";

	if(Log)
		delete Log;
}


//------------------------------------------------------------------------------
RProject::~RProject(void)
{
}
