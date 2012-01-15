/*

	RVLSI Project Library

	KProject.cpp

	View of a VLSI project - Implementation.

	Copyright 2000-2012 by Pascal Francq (pascal@francq.info).

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
#include <kproject.h>



//-----------------------------------------------------------------------------
//
// class KProject
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KProject::KProject(RProblem2D* session,const QString& uri)
	: QMdiSubWindow(), Ui_KProject(), Session(session)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(uri);
	createPrj();
}


//-----------------------------------------------------------------------------
void KProject::createPrj(void)
{
	QTreeWidgetItem* item=0,*item2=0,*item3=0,*item4=0,*item5=0;

	// Problem
	item=new QTreeWidgetItem(Elements,QStringList()<<"Problem");

	// Limits
	item2=new QTreeWidgetItem(item,QStringList()<<"Limits ("+QString::number(Session->GetLimits().GetWidth())+","+QString::number(Session->GetLimits().GetHeight())+")");
	item2=new QTreeWidgetItem(item,item2);
	item2->setText(0,"Global Limits ("+QString::number(Session->GetBoard().GetWidth())+","+QString::number(Session->GetBoard().GetHeight())+")");

	// Terminals
	item2=new QTreeWidgetItem(item,item2);
	item2->setText(0,"Terminals");
	RCursor<RObj2DConfigConnector> Cur2(Session->GetDefaultConfig()->GetConnectors());
	for(Cur2.Start(),item3=0;!Cur2.End();Cur2.Next())
	{
		if(item3)
		{
			item3=new QTreeWidgetItem(item2,item3);
			item3->setText(0,ToQString(Cur2()->GetConnector()->GetName()));
		}
		else
			item3=new QTreeWidgetItem(item2,QStringList()<<ToQString(Cur2()->GetConnector()->GetName()));

		RCursor<RObj2DConfigPin> Cur3(*Cur2());
		for(Cur3.Start(),item4=0;!Cur3.End();Cur3.Next())
		{
			RRect rect(Cur3()->GetRect());
			if(item4)
			{
				item4=new QTreeWidgetItem(item3,item4);
				item4->setText(0,"Pin at ("+QString::number(rect.GetX1())+","+QString::number(rect.GetY1())+","+QString::number(rect.GetX2())+","+QString::number(rect.GetY2())+")");
			}
			else
				item4=new QTreeWidgetItem(item3,QStringList()<<"Pin at ("+QString::number(rect.GetX1())+","+QString::number(rect.GetY1())+","+QString::number(rect.GetX2())+","+QString::number(rect.GetY2())+")");

		}
	}

	// Construct Objects
	item=new QTreeWidgetItem(Elements,item);
	item->setText(0,"Objects ("+QString::number(Session->GetNbObjs())+")");
	RCursor<RObj2D> Objs(Session->GetObjs());
	for(Objs.Start(),item2=0;!Objs.End();Objs.Next())
	{
		// Name of the object
		if(item2)
		{
			item2=new QTreeWidgetItem(item,item2);
			item2->setText(0,ToQString(Objs()->GetName())+" ("+QString::number(Objs()->GetId())+")");
		}
		else
			item2=new QTreeWidgetItem(item,QStringList()<<ToQString(Objs()->GetName())+" ("+QString::number(Objs()->GetId())+")");

		// Vertices
		item3=new QTreeWidgetItem(item2,QStringList()<<"Vertices");
		RCursor<RPoint> Cur(Objs()->GetDefaultConfig()->GetPolygon());
		for(Cur.Start(),item4=0;!Cur.End();Cur.Next())
		{
			if(item4)
			{
				item4=new QTreeWidgetItem(item3,item4);
				item4->setText(0,"("+QString::number(Cur()->X)+","+QString::number(Cur()->Y)+")");
			}
			else
				item4=new QTreeWidgetItem(item3,QStringList()<<"("+QString::number(Cur()->X)+","+QString::number(Cur()->Y)+")");

		}

		// Connectors
		item3=new QTreeWidgetItem(item2,item3);
		item3->setText(0,"Terminals");
		Cur2=Objs()->GetDefaultConfig()->GetConnectors();
		for(Cur2.Start(),item4=0;!Cur2.End();Cur2.Next())
		{
			if(item4)
			{
				item4=new QTreeWidgetItem(item3,item4);
				item4->setText(0,ToQString(Cur2()->GetConnector()->GetName()));
			}
			else
				item4=new QTreeWidgetItem(item3,QStringList()<<ToQString(Cur2()->GetConnector()->GetName()));
			RCursor<RObj2DConfigPin> Cur3(*Cur2());
			for(Cur3.Start();!Cur3.End();Cur3.Next())
			{
				RRect rect(Cur3()->GetRect());
				if(item5)
				{
					item5=new QTreeWidgetItem(item4,item5);
					item5->setText(0,"Pin at ("+QString::number(rect.GetX1())+","+QString::number(rect.GetY1())+","+QString::number(rect.GetX2())+","+QString::number(rect.GetY2())+")");
				}
				else
					item5=new QTreeWidgetItem(item4,QStringList()<<"Pin at ("+QString::number(rect.GetX1())+","+QString::number(rect.GetY1())+","+QString::number(rect.GetX2())+","+QString::number(rect.GetY2())+")");
			}
		}
	}

	// Construct Connections
	if(Session->GetNbConnections())
	{
		item=new QTreeWidgetItem(Elements,item);
		item->setText(0,"Nets ("+QString::number(Session->GetNbConnections())+")");
		RCursor<RConnection> Cons(Session->GetConnections());
		for(Cons.Start(),item2=0;!Cons.End();Cons.Next())
		{
			if(item2)
			{
				item2=new QTreeWidgetItem(item,item2);
				item2->setText(0,"Net "+QString(Cons()->GetName()));
			}
			else
				item2=new QTreeWidgetItem(item,QStringList()<<"Net "+QString(Cons()->GetName()));

			RCursor<RObj2DConnector> Cur2(Cons()->GetConnectors());
			for(Cur2.Start(),item3=0;!Cur2.End();Cur2.Next())
			{
				if(item3)
				{
					item3=new QTreeWidgetItem(item2,item3);
					QString Tmp;
					if(Cur2()->GetObj())
						Tmp=ToQString(Cur2()->GetObj()->GetName());
					else
						Tmp="Board";
					item3->setText(0,Tmp+"\t|\t"+ToQString(Cur2()->GetName()));
				}
				else
				{
					QString Tmp;
					if(Cur2()->GetObj())
						Tmp=ToQString(Cur2()->GetObj()->GetName());
					else
						Tmp="Board";
					item3=new QTreeWidgetItem(item2,QStringList()<<Tmp+"\t|\t"+ToQString(Cur2()->GetName()));
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
KProject::~KProject()
{
}
