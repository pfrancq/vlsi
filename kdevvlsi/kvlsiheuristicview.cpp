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
// include files for current application
#include "kdevvlsi.h"
#include "kvlsiheuristicview.h"
#include "kdevvlsidoc.h"



//-----------------------------------------------------------------------------
//
// class KVLSIHeuristicView
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KVLSIHeuristicView::KVLSIHeuristicView(KDevVLSIDoc* pDoc,HeuristicType pType,QWidget *parent, const char *name,int wflags)
	: KDevVLSIView(pDoc,parent,name,wflags),type(pType), infos(0), free(0)
{
	nbObjs = pDoc->NbObjs;
	draw=new QDrawPolygons(this);
	draw->setNbInfos(nbObjs);
	result=new QLabel(this);
}


//-----------------------------------------------------------------------------
KVLSIHeuristicView::~KVLSIHeuristicView()
{
	RGeoInfo **info;

	// Delete Infos
	if((infos)&&(infos!=doc->Infos))
	{
		for(nbObjs++,info=infos;--nbObjs;info++)
			delete (*info);
		delete[] infos;
	}

	// Delete Free Polygons
	if((free)&&(free!=doc->PlacementHeuristic->GetFreePolygons()))
		delete free;	
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
	sprintf(today," (%u-%u-%u %u:%u)",l_time->tm_year+1900,l_time->tm_mon+1,l_time->tm_mday,l_time->tm_hour,l_time->tm_min);
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
void KVLSIHeuristicView::slotBeginRun(void)
{
	infos=doc->Infos;
	free=doc->PlacementHeuristic->GetFreePolygons();
	draw->setPolys(free);
	draw->setInfos(infos);
	draw->setLimits(doc->Limits);
}


//-----------------------------------------------------------------------------
void KVLSIHeuristicView::slotBreakRun(void)
{
	infos=0;
	free=0;
}


//-----------------------------------------------------------------------------
void KVLSIHeuristicView::slotEndRun(void)
{
	unsigned int i;
	RGeoInfo **info,**info2;
	static char Str[40];

	infos=new RGeoInfo*[nbObjs];

	// Read Objs
	for(i=nbObjs+1,info=infos,info2=doc->Infos;--i;info++,info2++)
	{
		(*info)=new RGeoInfo(**info2);
	}
	free=new RFreePolygons(doc->PlacementHeuristic->GetFreePolygons());

	sprintf(Str,"ActLimits=(%u,%u) - Area=%u",doc->ActLimits.X,doc->ActLimits.Y,doc->ActLimits.X*doc->ActLimits.Y);
	result->setText(Str);	
	draw->setPolys(free);
	draw->setInfos(infos);
	draw->setLimits(doc->Limits);
	draw->setChanged();
}
