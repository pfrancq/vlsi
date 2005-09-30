/*

  kvlsiprjview.cpp

  Description - Implementation.

  (c) 2000 by P. Francq.

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
// include files for R
#include <rqt.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kvlsiprjview.h"
#include "kdevvlsidoc.h"



//-----------------------------------------------------------------------------
//
// class KVLSIPrjView
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KVLSIPrjView::KVLSIPrjView(KDevVLSIDoc* pDoc,QWidget *parent,const char *name,int wflags)
	: KDevVLSIView(pDoc,parent,name,wflags),prj(0)
{
	prj=new QListView(this,"Project Elements"+doc->URL().path());
	prj->addColumn("Elements");
	prj->setRootIsDecorated(true);
	prj->setSorting(-1);
	createPrj();
}


//-----------------------------------------------------------------------------
void KVLSIPrjView::createPrj(void)
{
	char tmp[100];
	QListViewItem *item=0,*item2=0,*item3=0,*item4=0,*item5=0;

	// Problem
	item=new QListViewItem(prj,"Problem");
	item2=new QListViewItem(item,item2,"Vertices");
	item3=0;
	RCursor<RPoint> Cur(doc->Problem.Polygon);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		sprintf(tmp,"(%d,%d)",Cur()->X,Cur()->Y);
		item3=new QListViewItem(item2,item3,tmp);
	}
	item2=new QListViewItem(item,item2,"Terminals");
	item3=0;
	RCursor<RObj2DConnector> Cur2(doc->Problem.Connectors);
	for(Cur2.Start();!Cur2.End();Cur2.Next())
	{
		item3=new QListViewItem(item2,item3,ToQString(Cur2()->GetName()));
		for(unsigned int i=0;i<Cur2()->NbPos;i++)
		{
			sprintf(tmp,"Pin at (%d,%d)",Cur2()->Pos[i].X,Cur2()->Pos[i].Y);
			item4=new QListViewItem(item3,item4,tmp);
		}
	}
	sprintf(tmp,"Limits (%d,%d)",doc->Limits.X,doc->Limits.Y);
	item2=new QListViewItem(item,item2,tmp);
	sprintf(tmp,"Global Limits (%d,%d)",doc->GlobalLimits.X,doc->GlobalLimits.Y);
	item2=new QListViewItem(item,item2,tmp);

	// Construct Objects
	sprintf(tmp,"Objects (%u)",doc->Objs.GetNb());
	item = new QListViewItem(prj,item,tmp);
	item2=0;
	RCursor<RObj2D> Objs(doc->Objs);
	for(Objs.Start();!Objs.End();Objs.Next())
	{
		// Name of the object
		item2 = new QListViewItem(item,item2,ToQString(Objs()->Name)+" ("+QString::number(Objs()->GetId()));
		item3=0;

		// Vertices
		item3=new QListViewItem(item2,item3,"Vertices");
		item4=0;
		Cur.Set(Objs()->Polygon);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			sprintf(tmp,"(%d,%d)",Cur()->X,Cur()->Y);
			item4=new QListViewItem(item3,item4,tmp);
		}

		// Connectors
		item3=new QListViewItem(item2,item3,"Terminals");
		item4=0;
		Cur2.Set(Objs()->Connectors);
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			item4=new QListViewItem(item3,item4,ToQString(Cur2()->GetName()));
			item5=0;
			for(unsigned int i=0;i<Cur2()->NbPos;i++)
			{
				sprintf(tmp,"Pin at (%d,%d)",Cur2()->Pos[i].X,Cur2()->Pos[i].Y);
				item5=new QListViewItem(item4,item5,tmp);
			}
		}
	}

	// Construct Connections
	if(doc->Cons.GetNb())
	{
		sprintf(tmp,"Nets (%u)",doc->Cons.GetNb());
		item=new QListViewItem(prj,item,tmp);
		item2=0;
		RCursor<RConnection> Cons(doc->Cons);
		for(Cons.Start();!Cons.End();Cons.Next())
		{
			item2=new QListViewItem(item,item2,"Net");
			item3=0;
			Cur2.Set(Cons()->Connect);
			for(Cur2.Start();!Cur2.End();Cur2.Next())
			{
				item3 = new QListViewItem(item2,item3,ToQString(Cur2()->Owner->Name)+"\t|\t"+ToQString(Cur2()->Name));
			}
		}
	}
}


//-----------------------------------------------------------------------------
void KVLSIPrjView::resizeEvent(QResizeEvent *)
{
	prj->resize(width(),height());
}


//-----------------------------------------------------------------------------
KVLSIPrjView::~KVLSIPrjView()
{
}
