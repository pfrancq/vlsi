/*

  kvlsiviewpolygons.cpp

  Description - Implementation.

  (c) 2001 by P. Francq.

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
#include "kvlsiviewpolygons.h"



//-----------------------------------------------------------------------------
//
// class KVLSIViewPolygons
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KVLSIViewPolygons::KVLSIViewPolygons(const char* file,QWidget *parent, const char *name, int wflags )
	: KDevVLSIView((KDevVLSIDoc*)0,parent,name,wflags), infos(0), nbInfos(0)
{
	RTextFile f(file);
	unsigned int nb,i,j;
	RCoord X,Y;
	RGeoInfo **info;
	RPoint Limits(0,0),Origin(0,0);
	RPolygon *p;

	f.Open(Read);
	f>>nbInfos;
	infos=new RGeoInfo*[nbInfos];
	for(i=nbInfos+1,info=infos;--i;info++)
	{
		f>>nb;
		p=new RPolygon(nb);
		(*info)=new RGeoInfo(p);
		(*info)->Assign(Origin);
		for(j=0;j<nb;j++)
		{
			f>>X>>Y;
			if(X>Limits.X) Limits.X=X;
			if(Y>Limits.Y) Limits.Y=Y;
			p->InsertPtr(new RPoint(X,Y));
		}

	}
	draw=new QDrawPolygons(this);
//	draw->setInfos(this);
	draw->setNbInfos(nbInfos);
	draw->setLimits(Limits);
	draw->setChanged();
	setTitle(file);
}


//-----------------------------------------------------------------------------
void KVLSIViewPolygons::resizeEvent(QResizeEvent *)
{
	draw->resize(width(),height());
}


//-----------------------------------------------------------------------------
KVLSIViewPolygons::~KVLSIViewPolygons(void)
{
	RGeoInfo **info;
	unsigned int i;

	if(infos)
	{
		for(i=nbInfos+1,info=infos;--i;info++)
			delete(*info);
		delete[] infos;
	}
}
