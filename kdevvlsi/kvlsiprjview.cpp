/*

  kvlsiprjview.cpp

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
	RObj2D* obj;
	RConnection *con;
	RObj2DConnector* connector;

	// Problem
	item=new QListViewItem(prj,"Problem");
	item2=new QListViewItem(item,item2,"Vertices");
	item3=0;
	for(doc->Problem.Polygon.Start();!doc->Problem.Polygon.End();doc->Problem.Polygon.Next())
	{
		sprintf(tmp,"(%d,%d)",doc->Problem.Polygon()->X,doc->Problem.Polygon()->Y);
		item3=new QListViewItem(item2,item3,tmp);
	}
	item2=new QListViewItem(item,item2,"Terminals");
	item3=0;
	for(doc->Problem.Connectors.Start();!doc->Problem.Connectors.End();doc->Problem.Connectors.Next())
	{
		connector=doc->Problem.Connectors();
		item3=new QListViewItem(item2,item3,connector->GetName());
		for(unsigned int i=0;i<connector->NbPos;i++)
		{
			sprintf(tmp,"Pin at (%d,%d)",connector->Pos[i].X,connector->Pos[i].Y);
			item4=new QListViewItem(item3,item4,tmp);
		}
	}
	sprintf(tmp,"Limits (%d,%d)",doc->Limits.X,doc->Limits.Y);
	item2=new QListViewItem(item,item2,tmp);
	sprintf(tmp,"Global Limits (%d,%d)",doc->GlobalLimits.X,doc->GlobalLimits.Y);
	item2=new QListViewItem(item,item2,tmp);

	// Construct Objects
	sprintf(tmp,"Objects (%u)",doc->Objs.NbPtr);
	item = new QListViewItem(prj,item,tmp);
	item2=0;
	for(doc->Objs.Start();!doc->Objs.End();doc->Objs.Next())
	{
		obj=doc->Objs();
		// Name of the object
		sprintf(tmp,"%s (%u)",obj->Name(),obj->GetId());
		item2 = new QListViewItem(item,item2,tmp);
		item3=0;

		// Vertices
		item3=new QListViewItem(item2,item3,"Vertices");
		item4=0;
		for(obj->Polygon.Start();!obj->Polygon.End();obj->Polygon.Next())
		{
			sprintf(tmp,"(%d,%d)",obj->Polygon()->X,obj->Polygon()->Y);
			item4=new QListViewItem(item3,item4,tmp);
		}
		
		// Connectors
		item3=new QListViewItem(item2,item3,"Terminals");
		item4=0;
		for(obj->Connectors.Start();!obj->Connectors.End();obj->Connectors.Next())
		{
			connector=obj->Connectors();
			item4=new QListViewItem(item3,item4,connector->GetName());
			item5=0;
			for(unsigned int i=0;i<connector->NbPos;i++)
			{
				sprintf(tmp,"Pin at (%d,%d)",connector->Pos[i].X,connector->Pos[i].Y);
				item5=new QListViewItem(item4,item5,tmp);
			}
		}
	}

	// Construct Connections
	if(doc->Cons.NbPtr)
	{
		sprintf(tmp,"Nets (%u)",doc->Cons.NbPtr);
		item=new QListViewItem(prj,item,tmp);
		item2=0;
		for(doc->Cons.Start();!doc->Cons.End();doc->Cons.Next())
		{
			item2=new QListViewItem(item,item2,"Net");
			item3=0;
			con=doc->Cons();
			for(con->Connect.Start();!con->Connect.End();con->Connect.Next())
			{
				sprintf(tmp,"%s\t|\t%s",con->Connect()->Owner->Name(),con->Connect()->Name());
				item3 = new QListViewItem(item2,item3,tmp);
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
