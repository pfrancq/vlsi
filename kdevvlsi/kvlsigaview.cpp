/*

  kvlsigaview.cpp

  Description - Implementation.

  (c) 2001 by P. Francq.

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
// include files for Qt/KDE
#include <klocale.h>
#include <kmessagebox.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kvlsigaview.h"
#include "kdevvlsidoc.h"



//-----------------------------------------------------------------------------
//
// class KVLSIGAView
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KVLSIGAView::KVLSIGAView(KDevVLSIDoc* pDoc,QWidget *parent, const char *name,int wflags)
	: KDevVLSIView(pDoc,parent,name,wflags), CurId(0), Instance(0)
{
	static char tmp[100];

	setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, sizePolicy().hasHeightForWidth() ) );
	TabWidget = new QTabWidget( this, "TabWidget" );
	TabWidget->setGeometry(rect());
	TabWidget->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, TabWidget->sizePolicy().hasHeightForWidth() ) );
	TabWidget->setBackgroundOrigin( QTabWidget::ParentOrigin );

	// Stat part
	StatSplitter=new QSplitter(QSplitter::Vertical,TabWidget,"Statistic");
	TabWidget->insertTab(StatSplitter,"Statistic");
	StatSplitter->setGeometry(rect());
	Monitor=new	QGAMonitor(StatSplitter);
	Monitor->setHScale(theApp->GAMaxGen);
	connect(this,SIGNAL(signalSetGen(unsigned int,unsigned int,double)),Monitor,SLOT(slotSetGen(unsigned int,unsigned int,double)));
	Debug=new QXMLContainer(StatSplitter);

	// Solution part
	Best = new QDrawPolygons(pDoc,TabWidget);
	TabWidget->insertTab(Best,"Best Solution");
	Best->setNbInfos(doc->Objs.NbPtr);
	Best->setCons(&doc->Cons);

	// Solution part
	Sol = new QDrawPolygons(pDoc,TabWidget);
	sprintf(tmp,"Solution (0/%u)",((KDevVLSIApp*)parentWidget()->parentWidget()->parentWidget())->GAPopSize-1);
	TabWidget->insertTab(Sol,tmp);
	Sol->setNbInfos(doc->Objs.NbPtr);
	Sol->setCons(&doc->Cons);

	// Create GA
	try
	{
		Gen=0;
		Instance=new RInstVLSI(theApp->GAMaxGen,theApp->GAPopSize,pDoc,theApp->GAHeur);
		Instance->AddReceiver(this);
		Instance->Init();
		Instance->SetAreaParams(theApp->HeurArea);
		Instance->SetDistParams(theApp->HeurDist);
	}
	catch(eGA& e)
	{
		KMessageBox::error(this,QString(e.Msg));
		Instance=0;
	}
	catch(bad_alloc)
	{
		KMessageBox::error(this,"Memory Problems");
		Instance=0;
	}
	catch(...)
	{
		KMessageBox::error(this,"Unknow Problem");
		Instance=0;
	}

}


//---------------------------------------------------------------------------
void KVLSIGAView::receiveGenSig(GenSig* sig)
{
	emit signalSetGen(sig->Gen,sig->BestGen,sig->Best->Fitness->Value);
	Sol->setInfos(Instance->Chromosomes[CurId]->Tab);
	Sol->setChanged();

}


//---------------------------------------------------------------------------
void KVLSIGAView::receiveInteractSig(InteractSig* /*sig*/)
{
	KApplication::kApplication()->processEvents(1000);
}


//---------------------------------------------------------------------------
void KVLSIGAView::receiveBestSig(BestSig* sig)
{
	static char tmp[100];

	sprintf(tmp,"Best Solution (Id=%u)",sig->Best->Id);
	TabWidget->changeTab(Best,tmp);
	Best->setInfos(sig->Best->Tab);
	Best->setChanged();
}


//---------------------------------------------------------------------------
void KVLSIGAView::RunGA(void)
{
	if(Instance)
	{
		try
		{		
			if(theApp->GAMaxGen>Gen)
			{
				if(theApp->GAStepGen==0)
					Gen=theApp->GAMaxGen;
				else
				{
					Gen+=theApp->GAStepGen;
					if(Gen>theApp->GAMaxGen) Gen=theApp->GAMaxGen;
				}
			}
			Instance->MaxGen=Gen;
			Instance->Run();
			if(Gen==theApp->GAMaxGen)
				theApp->GAPause->setEnabled(false);
		}
		catch(eGA& e)
		{
			KMessageBox::error(this,QString(e.Msg));
		}
	}

}


//---------------------------------------------------------------------------
void KVLSIGAView::PauseGA(void)
{
	ExternBreak=true;
}


//---------------------------------------------------------------------------
void KVLSIGAView::StopGA(void)
{
}


//---------------------------------------------------------------------------
void KVLSIGAView::keyReleaseEvent(QKeyEvent* e)
{
	static char tmp[100];
//	QGoToPopDlg *dlg;

	if(TabWidget->currentPage()!=Sol)
	{
		KDevVLSIView::keyReleaseEvent(e);
		return;
	}
	switch(e->key())
	{
		case Key_PageUp:
			if(CurId<Instance->PopSize-1) CurId++; else CurId=0;
			sprintf(tmp,"Solution (%u/%u)",CurId,Instance->PopSize-1);
			TabWidget->changeTab(Sol,tmp);
			Sol->setInfos(Instance->Chromosomes[CurId]->Tab);
			Sol->setChanged();
			break;

		case Key_PageDown:
			if(CurId>0) CurId--; else CurId=Instance->PopSize-1;
			sprintf(tmp,"Solution (%u/%u)",CurId,Instance->PopSize-1);	
			TabWidget->changeTab(Sol,tmp);
			Sol->setInfos(Instance->Chromosomes[CurId]->Tab);
			Sol->setChanged();
			break;

//		case Key_G:
//			if(e->state()==ControlButton)
//			{
//				dlg= new QGoToPopDlg(Doc->getPopSize());
//				if(dlg->exec())
//				{
//					CurId=dlg->PopIndex->value();
//					slotNewChromo();
//				}
//				delete dlg;
//			}
//			break;

		default:
			e->ignore();
	}
}


//-----------------------------------------------------------------------------
void KVLSIGAView::resizeEvent(QResizeEvent*)
{
	TabWidget->resize(size());
}


//-----------------------------------------------------------------------------
KVLSIGAView::~KVLSIGAView()
{
	if(Instance)
		delete Instance;
}
