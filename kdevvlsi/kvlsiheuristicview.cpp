/*

  kvlsiheuristicview.cpp

  Description - Implementation.

  (c) 2000 by P. Francq.

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



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <time.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rga2d/rplacementbottomleft.h>
#include <rga2d/rplacementedge.h>
#include <rga2d/rplacementcenter.h>
using namespace RGA;


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <klocale.h>
#include <kmessagebox.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdevvlsi.h"
#include "kvlsiheuristicview.h"
#include "kdevvlsidoc.h"


//-----------------------------------------------------------------------------
// defines
//#define DOUBLESPACE



//-----------------------------------------------------------------------------
//
// class KVLSIHeuristicView
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KVLSIHeuristicView::KVLSIHeuristicView(KDevVLSIDoc* pDoc,HeuristicType pType,QWidget *parent, const char *name,int wflags)
	: KDevVLSIView(pDoc,parent,name,wflags), RGeoInfos(pDoc,true), Random(0), type(pType), grid(0),
	  free(0), PlacementHeuristic(0), nbFree(0)
{
	nbObjs = pDoc->Objs.NbPtr;
	draw=new QDrawPolygons(pDoc,this);
	draw->setNbInfos(nbObjs);
	draw->setInfos(Tab);
	result=new QLabel(this);
	grid=new RGrid(pDoc->Limits);

	// Init the random generator
	Random = new RRandomGood(12345);

	
	// Init the heuristic
	calcFree=theApp->calcFree;
	useFree=theApp->useFree;
	allOri=theApp->allOri;
	RPromCriterionParams HeurDist=theApp->HeurDist;
	RPromCriterionParams HeurArea=theApp->HeurArea;
	step=theApp->step;

	switch(pType)
	{
		case BottomLeft:
			PlacementHeuristic = new RPlacementBottomLeft(pDoc->Objs.NbPtr,calcFree,useFree,Random,allOri);
			break;

		case Edge:
			PlacementHeuristic = new RPlacementEdge(pDoc->Objs.NbPtr,calcFree,useFree,Random,allOri);
			break;

		case Center:
			PlacementHeuristic = new RPlacementCenter(pDoc->Objs.NbPtr,calcFree,useFree,Random,allOri);
			break;
	}
	PlacementHeuristic->Init(pDoc,Tab,grid);
	PlacementHeuristic->SetAreaParams(HeurArea);
	PlacementHeuristic->SetDistParams(HeurDist);
	free=PlacementHeuristic->GetFreePolygons();
	draw->setPolys(free);
	connect(this,SIGNAL(endRun()),theApp,SLOT(slotEndHeuristic(void)));
}


//-----------------------------------------------------------------------------
void KVLSIHeuristicView::setTitle(QString _title)
{
	time_t now;
	static char today[30];
	struct tm *l_time;

	switch(type)
	{
		case BottomLeft:
			_title="Bottom-Left Heuristic: "+_title;
			break;

		case Edge:
			_title="Edge Heuristic: "+_title;
			break;

		case Center:
			_title="Center Heuristic: "+_title;
			break;
	}
	now=time((time_t *)0);
  	l_time = localtime(&now);
	sprintf(today," (%u-%u-%u %u:%u:%u)",l_time->tm_year+1900,l_time->tm_mon+1,l_time->tm_mday,l_time->tm_hour,l_time->tm_min,l_time->tm_sec);
	setCaption(_title+today);
}


//-----------------------------------------------------------------------------
void KVLSIHeuristicView::resizeEvent(QResizeEvent *)
{
	result->resize(width(),result->height());
	result->move(0,height()-result->height());
	draw->resize(width(),height()-result->height());
}


//-----------------------------------------------------------------------------
void KVLSIHeuristicView::RunHeuristic(void)
{
	// Run the heuristic
	if(step)
		NextStep();
	else
		while(!PlacementHeuristic->IsEnd())
			NextStep();
}


//-----------------------------------------------------------------------------
void KVLSIHeuristicView::NextStep(void)
{
	static char Str[150];
	static char Tmp[150];
	RPoint ActLimits;

	try
	{
		CurInfo=PlacementHeuristic->NextObject();
		if(step)
		{
			draw->addInfo(CurInfo);
			while(free->NbPtr>nbFree)
				addFree(free->Tab[nbFree++]);
		}

		// test if the end
		if(PlacementHeuristic->IsEnd())
		{
			PlacementHeuristic->PostRun(getDocument()->Limits);
			RRect Rect=PlacementHeuristic->GetResult();
			ActLimits.X=Rect.Width();
			ActLimits.Y=Rect.Height();
			
			// Construct Result Message
			sprintf(Str,"ActLimits=(%u,%u)  -  Area=%u  -  Free(Calc=",ActLimits.X,ActLimits.Y,ActLimits.X*ActLimits.Y);
			if(calcFree)
			{
				strcat(Str,"true,Use=");
				if(useFree)
					strcat(Str,"true)");
				else
					strcat(Str,"false)");
			}
			else
				strcat(Str,"false)");
			strcat(Str,"   -   Ori=");
			if(allOri)
				strcat(Str,"full");
			else
				strcat(Str,"random");
			sprintf(Tmp,"  -  Area Crit.(p=%.3f,q=%.3f,w=%.1f)",HeurArea.P,HeurArea.Q,HeurArea.Weight);
			strcat(Str,Tmp);
			sprintf(Tmp,"  -  Dist Crit.(p=%.3f,q=%.3f,w=%.1f)",HeurDist.P,HeurDist.Q,HeurDist.Weight);
			strcat(Str,Tmp);
	
			// Set the variables and repaint all
			result->setText(Str);
			draw->setChanged();
			emit endRun();
		}
		KApplication::kApplication()->processEvents(1000);
	}
	catch(RPlacementHeuristicException& e)
    {
		KMessageBox::error(this,QString(e.Msg));
		draw->setChanged();
    }
}


//-----------------------------------------------------------------------------
void KVLSIHeuristicView::RunToEnd(void)
{
	step=false;
	while(!PlacementHeuristic->IsEnd())
	{
		NextStep();
	}
}


//-----------------------------------------------------------------------------
void KVLSIHeuristicView::SelectObjects(void)
{
	RObj2DContainer* ptr=new RObj2DContainer(doc->Objs.NbPtr+1,doc->Objs.NbPtr);

	memset(Selected,0,doc->Objs.NbPtr*sizeof(bool));
	doc->Cons.SetParams(theApp->SelectDist,theApp->SelectWeight,Random);
	GetSetInfos(ptr,grid,Selected);
	delete ptr;
	draw->setChanged();
}


//-----------------------------------------------------------------------------
KVLSIHeuristicView::~KVLSIHeuristicView()
{
	if(PlacementHeuristic)
		delete PlacementHeuristic;

	if(grid)
		delete grid;

	if(Random)
		delete Random;
}
