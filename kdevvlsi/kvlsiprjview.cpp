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
KVLSIPrjView::~KVLSIPrjView()
{
}


//-----------------------------------------------------------------------------
void KVLSIPrjView::createPrj(void)
{
	char tmp[30];
	QListViewItem *item,*item2,*item3;
	unsigned int i,j;
	RObj2D **obj;
	RPolygon *poly;
	RPoint **pt;

	// Construct Objects
	item = new QListViewItem(prj,"Objects");
	item2=0;
	for(i=0,obj=doc->Objs;i<doc->NbObjs;i++,obj++)
	{
		sprintf(tmp,"Object n°%u",i);
		item2 = new QListViewItem(item,item2,tmp);
		item3=0;
		poly=&(*obj)->Polygon;
		for(j=poly->NbPtr+1,pt=poly->Tab;--j;pt++)
		{
			sprintf(tmp,"(%u,%u)",(*pt)->X,(*pt)->Y);
			item3=new QListViewItem(item2,item3,tmp);
		}
	}
}


//-----------------------------------------------------------------------------
void KVLSIPrjView::resizeEvent(QResizeEvent *)
{
	prj->resize(width(),height());
}
