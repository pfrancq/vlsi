/*

	KProject.cpp

	View of a VLSI project - Implementation.

	Copyright 2000-2009 by Pascal Francq (pascal@francq.info).

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
KProject::KProject(RProblem2D* session)
	: QMdiSubWindow(), Ui_KProject(), Session(session)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(ToQString(Session->GetURI()()));
	createPrj();
}


//-----------------------------------------------------------------------------
void KProject::createPrj(void)
{
	QTreeWidgetItem* item=0,*item2=0,*item3=0,*item4=0,*item5=0;
	size_t i;

	// Problem
	item=new QTreeWidgetItem(Elements,QStringList()<<"Problem");

	// Limits
	item2=new QTreeWidgetItem(item,QStringList()<<"Limits ("+QString::number(Session->Limits.X)+","+QString::number(Session->Limits.Y)+")");
	item2=new QTreeWidgetItem(item,item2);
	item2->setText(0,"Global Limits ("+QString::number(Session->GlobalLimits.X)+","+QString::number(Session->GlobalLimits.Y)+")");

	// Vertices
	item2=new QTreeWidgetItem(item,item2);
	item2->setText(0,"Vertices");
	RCursor<RPoint> Cur(Session->Problem.Polygon);
	for(Cur.Start(),item3=0;!Cur.End();Cur.Next())
	{
		if(item3)
		{
			item3=new QTreeWidgetItem(item2,item3);
			item3->setText(0,"("+QString::number(Cur()->X)+","+QString::number(Cur()->Y)+")");
		}
		else
			item3=new QTreeWidgetItem(item2,QStringList()<<"("+QString::number(Cur()->X)+","+QString::number(Cur()->Y)+")");
	}

	// Terminals
	item2=new QTreeWidgetItem(item,item2);
	item2->setText(0,"Terminals");
	RCursor<RObj2DConnector> Cur2(Session->Problem.Connectors);
	for(Cur2.Start(),item3=0;!Cur2.End();Cur2.Next())
	{
		if(item3)
		{
			item3=new QTreeWidgetItem(item2,item3);
			item3->setText(0,ToQString(Cur2()->GetName()));
		}
		else
			item3=new QTreeWidgetItem(item2,QStringList()<<ToQString(Cur2()->GetName()));

		for(i=0,item4=0;i<Cur2()->NbPos;i++)
		{
			if(item4)
			{
				item4=new QTreeWidgetItem(item3,item4);
				item4->setText(0,"Pin at ("+QString::number(Cur2()->Pos[i].X)+","+QString::number(Cur2()->Pos[i].Y)+")");
			}
			else
				item4=new QTreeWidgetItem(item3,QStringList()<<"Pin at ("+QString::number(Cur2()->Pos[i].X)+","+QString::number(Cur2()->Pos[i].Y)+")");

		}
	}

	// Construct Objects
	item=new QTreeWidgetItem(Elements,item);
	item->setText(0,"Objects ("+QString::number(Session->Objs.GetNb())+")");
	RCursor<RObj2D> Objs(Session->Objs);
	for(Objs.Start(),item2=0;!Objs.End();Objs.Next())
	{
		// Name of the object
		if(item2)
		{
			item2=new QTreeWidgetItem(item,item2);
			item2->setText(0,ToQString(Objs()->Name)+" ("+QString::number(Objs()->GetId())+")");
		}
		else
			item2=new QTreeWidgetItem(item,QStringList()<<ToQString(Objs()->Name)+" ("+QString::number(Objs()->GetId())+")");

		// Vertices
		item3=new QTreeWidgetItem(item2,QStringList()<<"Vertices");
		Cur.Set(Objs()->Polygon);
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
		Cur2.Set(Objs()->Connectors);
		for(Cur2.Start(),item4=0;!Cur2.End();Cur2.Next())
		{
			if(item4)
			{
				item4=new QTreeWidgetItem(item3,item4);
				item4->setText(0,ToQString(Cur2()->GetName()));
			}
			else
				item4=new QTreeWidgetItem(item3,QStringList()<<ToQString(Cur2()->GetName()));

			for(i=0,item5=0;i<Cur2()->NbPos;i++)
			{
				if(item5)
				{
					item5=new QTreeWidgetItem(item4,item5);
					item5->setText(0,"Pin at ("+QString::number(Cur2()->Pos[i].X)+","+QString::number(Cur2()->Pos[i].Y)+")");
				}
				else
					item5=new QTreeWidgetItem(item4,QStringList()<<"Pin at ("+QString::number(Cur2()->Pos[i].X)+","+QString::number(Cur2()->Pos[i].Y)+")");
			}
		}
	}

	// Construct Connections
	if(Session->Cons.GetNb())
	{
		item=new QTreeWidgetItem(Elements,item);
		item->setText(0,"Nets ("+QString::number(Session->Cons.GetNb())+")");
		RCursor<RConnection> Cons(Session->Cons);
		for(Cons.Start(),item2=0;!Cons.End();Cons.Next())
		{
			if(item2)
			{
				item2=new QTreeWidgetItem(item,item2);
				item2->setText(0,"Net");
			}
			else
				item2=new QTreeWidgetItem(item,QStringList()<<"Net");

			Cur2.Set(Cons()->Connect);
			for(Cur2.Start(),item3=0;!Cur2.End();Cur2.Next())
			{
				if(item3)
				{
					item3=new QTreeWidgetItem(item2,item3);
					item3->setText(0,ToQString(Cur2()->Owner->Name)+"\t|\t"+ToQString(Cur2()->Name));
				}
				else
					item3=new QTreeWidgetItem(item2,QStringList()<<ToQString(Cur2()->Owner->Name)+"\t|\t"+ToQString(Cur2()->Name));
			}
		}
	}
}


//-----------------------------------------------------------------------------
KProject::~KProject()
{
}
