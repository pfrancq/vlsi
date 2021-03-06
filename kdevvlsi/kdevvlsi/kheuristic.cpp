/*

	RVLSI Project Library

	KHeuristic.cpp

	Run a heuristic on the problem - Implementation.

	Copyright 2000-2016 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Universit√© Libre de Bruxelles (ULB).

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
// include files for ANSI C/C++
#include <time.h>


//-----------------------------------------------------------------------------
// include files for R/VLSI Project
#include <rvlsiapp.h>
#include <rrect.h>
#include <robj2dcontainer.h>
#include <rplacementbottomleft.h>
#include <rplacementedge.h>
#include <rplacementcenter.h>
#include <rqt.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt
#include <QMessageBox>


//-----------------------------------------------------------------------------
// include files for current application
#include <kdevvlsi.h>
#include <kheuristic.h>


//-----------------------------------------------------------------------------
// defines
//#define DOUBLESPACE



//-----------------------------------------------------------------------------
//
// class KHeuristic
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KHeuristic::KHeuristic(RProblem2D* session,const RString& heuristic)
	: QMdiSubWindow(), Ui_KHeuristic(), RLayout(session,true), Session(session), Random(RRandom::Good,12345),
	  Grid(session->GetLimits()), Free(0), PlacementHeuristic(0), NbFree(0), Step(VLSIApp->Step), Heuristic(heuristic)
{
	// Set the widget part
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	Draw->setLayout(this);
	setWindowTitle(ToQString(Heuristic));

	// Create the Heuristic
	if(Heuristic=="Bottom-Left")
		PlacementHeuristic = new RPlacementBottomLeft(Session->GetNbObjs(),VLSIApp->ComputeFreePolygons,VLSIApp->UseFreePolygons,Random,VLSIApp->HeurArea,VLSIApp->HeurDist,false,VLSIApp->AllOrientations);
	else if(Heuristic=="Edge")
		PlacementHeuristic = new RPlacementEdge(Session->GetNbObjs(),VLSIApp->ComputeFreePolygons,VLSIApp->UseFreePolygons,Random,VLSIApp->HeurArea,VLSIApp->HeurDist,false,VLSIApp->AllOrientations);
	else if(Heuristic=="Center")
		PlacementHeuristic = new RPlacementCenter(Session->GetNbObjs(),VLSIApp->ComputeFreePolygons,VLSIApp->UseFreePolygons,Random,VLSIApp->HeurArea,VLSIApp->HeurDist,false,VLSIApp->AllOrientations);
	else
		mThrowRException("'"+Heuristic+"' is not a valid heuristic type");
	PlacementHeuristic->Init(Session,this,&Grid);
	Free=PlacementHeuristic->GetFreePolygons();
	Draw->setPolys(Free);
}


//-----------------------------------------------------------------------------
void KHeuristic::RunHeuristic(void)
{
	Draw->setStepMode(Step);
	Stop=false;

	// Run the heuristic
	if(Step)
		NextStep();
	else
		while((!PlacementHeuristic->IsEnd())&&(!Stop))
			NextStep();
}


//-----------------------------------------------------------------------------
void KHeuristic::NextStep(void)
{
	if(PlacementHeuristic->IsEnd())
		return;

	try
	{
		CurInfo=PlacementHeuristic->NextObject();
		if(Step)
		{
			Draw->paintInfo(CurInfo,true);
			while(Free->GetNb()>NbFree)
				Draw->paintFree((*Free)[NbFree++],true);
		}

		// test if the end
		if(PlacementHeuristic->IsEnd())
		{
			PlacementHeuristic->PostRun();
			RRect Rect=PlacementHeuristic->GetResult();
			RPoint ActLimits;
			ActLimits.X=Rect.GetWidth();
			ActLimits.Y=Rect.GetHeight();

			// Construct Result Message
			QString Str("ActLimits=("+QString::number(ActLimits.X)+","+QString::number(ActLimits.Y)+")");
			Str+="  -  Area="+QString::number(ActLimits.X*ActLimits.Y);
			Str+="  -  Dist="+QString::number(PlacementHeuristic->GetDistances());
			Str+="  -  Free(Compute=";
			if(VLSIApp->ComputeFreePolygons)
			{
				Str+="true,Use=";
				if(VLSIApp->UseFreePolygons)
					Str+="true)";
				else
					Str+="false)";
			}
			else
				Str+="false)";
			Str+="   -   Orienation=";
			if(VLSIApp->AllOrientations)
				Str+="full";
			else
				Str+="random";

			// Set the variables and repaint all
			Result->setText(Str);
			Draw->repaint();
			emit endRun();
		}
	}
	catch(RException& e)
	{
		QMessageBox::critical(0,QWidget::tr("R exception"),ToQString(e.GetMsg()),QMessageBox::Ok);
		Draw->repaint();
		Stop=true;
	}
	catch(std::exception& e)
	{
		QMessageBox::critical(0,QWidget::tr("std::exception"),QWidget::trUtf8(e.what()),QMessageBox::Ok);
		Draw->repaint();
		Stop=true;
	}
	catch(...)
	{
		QMessageBox::critical(0,QWidget::tr("Unknown exception"),QWidget::trUtf8("Unknown problem"),QMessageBox::Ok);
		Draw->repaint();
		Stop=true;
	}
}


//-----------------------------------------------------------------------------
void KHeuristic::RunToEnd(void)
{
	Step=false;
	Draw->setStepMode(Step);
	while(!PlacementHeuristic->IsEnd())
	{
		NextStep();
	}
}


//-----------------------------------------------------------------------------
void KHeuristic::SelectObjects(void)
{
	RPromKernel Kernel("PlacementCenter",Session->GetNbObjs(),2);
	Kernel.AddCriterion(new RPromLinearCriterion(RPromCriterion::Maximize,VLSIApp->SelectWeight,"Weight"));
	Kernel.AddCriterion(new RPromLinearCriterion(RPromCriterion::Minimize,VLSIApp->SelectDist,"Distance"));
	RObj2DContainer* Temp(GetNewAggregator());
	bool* Selected;
	Selected=new bool [Session->GetNbObjs()];
	memset(Selected,0,Session->GetNbObjs()*sizeof(bool));
	FillAggregator(Temp,Selected,&Kernel,Random,Session->MustWeightedDistances());
	Draw->repaint();
	delete[] Selected;
	DestroyAggregators();
}


//-----------------------------------------------------------------------------
KHeuristic::~KHeuristic(void)
{
	if(PlacementHeuristic)
		delete PlacementHeuristic;
}
