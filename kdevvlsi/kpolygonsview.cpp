/*

	RVLSI Project Library

	KPolygonsView.cpp

	View a set of polygons from a file - Implementation.

	Copyright 2000-2016 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R Project
#include <rqt.h>


//-----------------------------------------------------------------------------
// include files for current project
#include <kpolygonsview.h>



//-----------------------------------------------------------------------------
//
// class KPolygonsView
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KPolygonsView::KPolygonsView(void)
	: QMdiSubWindow(), Ui_KPolygonsView(), Layout(0)
{
	// Set the widget part
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle("No file loaded");
}


//-----------------------------------------------------------------------------
void KPolygonsView::Load(QString& /*url*/)
{
	// Load the file
/*	RTextFile f(FromQString(url.path()));
	size_t nb,i,j;
	tCoord X,Y;
	RGeoInfo **info;
	RPoint Limits(0,0),Origin(0,0);
	RPolygon *p;

	f.Open(RIO::Read);
	f>>NbInfos;
	Infos=new RGeoInfo*[NbInfos];
	for(i=NbInfos+1,info=Infos;--i;info++)
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
	Draw->setInfos(Infos,Limits,Origin);
	setWindowTitle(url.path());-*/
}


//-----------------------------------------------------------------------------
KPolygonsView::~KPolygonsView(void)
{
	delete Layout;
}
