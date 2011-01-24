/*

	RVLSI Project Library

	KGAView.cpp

	Window to manage a GA instance - Implementation.

	Copyright 2000-2011 by Pascal Francq (pascal@francq.info).

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
// include files for R/VLSI Project
#include <rchromovlsi.h>
#include <rvlsiapp.h>


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QKeyEvent>
#include <klocale.h>
#include <kmessagebox.h>


//-----------------------------------------------------------------------------
// include files for current application
#include <kgaview.h>



//-----------------------------------------------------------------------------
//
// class KGAView
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
size_t KGAView::WinNb=0;

//-----------------------------------------------------------------------------
KGAView::KGAView(void)
	: QMdiSubWindow(), Ui_KGAView(), RObject("GAView "+RString::Number(++WinNb)), CurId(0), Instance(0)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle("VLSI Genetic Algorithm");
	Monitor->setParams(VLSIApp->MaxGen,0.0,1.0);

	// Create GA
	try
	{
		Gen=0;
		Instance=new RInstVLSI(VLSIApp->MaxGen,VLSIApp->PopSize,VLSIApp->GetSession(),VLSIApp->Heuristic,
				VLSIApp->HeurDist,VLSIApp->HeurArea,VLSIApp->SelectDist,VLSIApp->SelectWeight,
				Debug);
		Instance->Init();
		reinterpret_cast<RObject*>(this)->InsertObserver(reinterpret_cast<tNotificationHandler>(&KGAView::Generation),"RInst::Generation",Instance);
		reinterpret_cast<RObject*>(this)->InsertObserver(reinterpret_cast<tNotificationHandler>(&KGAView::BestChromo),"RInst::Best",Instance);
		reinterpret_cast<RObject*>(this)->InsertObserver(reinterpret_cast<tNotificationHandler>(&KGAView::Interact),"RInst::Interact",Instance);
	}
	catch(RException& e)
	{
		KMessageBox::error(this,e.GetMsg());
		Instance=0;
	}
	catch(exception& e)
	{
		KMessageBox::error(this,e.what());
		Instance=0;
	}
	catch(...)
	{
		KMessageBox::error(this,"Unknown Problem");
		Instance=0;
	}

	Main->setTabText(2,"Solution ("+QString::number(CurId)+"/"+QString::number(Instance->GetPopSize()-1)+")");
}


//---------------------------------------------------------------------------
void KGAView::RunGA(void)
{
	if(Instance)
	{
		try
		{		
			if(VLSIApp->MaxGen>Gen)
			{
				if(VLSIApp->StepGen==0)
					Gen=VLSIApp->MaxGen;
				else
				{
					Gen+=VLSIApp->StepGen;
					if(Gen>VLSIApp->MaxGen) Gen=VLSIApp->MaxGen;
				}
			}
			Instance->MaxGen=Gen;
			Instance->Run();
			KMessageBox::information(this,"Done");
		}
		catch(RException& e)
		{
			KMessageBox::error(this,e.GetMsg());
		}
		catch(exception& e)
		{
			KMessageBox::error(this,e.what());
		}
		catch(...)
		{
			KMessageBox::error(this,"Unknown Problem");
		}
	}
}


//---------------------------------------------------------------------------
void KGAView::PauseGA(void)
{
	ExternBreak=true;
}


//---------------------------------------------------------------------------
void KGAView::StopGA(void)
{
}


//---------------------------------------------------------------------------
void KGAView::Generation(const R::RNotification& notification)
{
	Monitor->setGenInfo(GetData<size_t>(notification),Instance->GetAgeBest(),Instance->GetBestChromosome()->Fitness->Value);
	Sol->setLayout(Instance->Chromosomes[CurId]);
}


//---------------------------------------------------------------------------
void KGAView::BestChromo(const R::RNotification&)
{
	Best->setLayout(Instance->GetBestChromosome());
}


//---------------------------------------------------------------------------
void KGAView::Interact(const R::RNotification&)
{
	 QCoreApplication::processEvents();
}


//---------------------------------------------------------------------------
void KGAView::keyReleaseEvent(QKeyEvent* e)
{
	if(Main->currentWidget()!=SolTab)
	{
		QWidget::keyReleaseEvent(e);
		return;
	}
	switch(e->key())
	{
		case Qt::Key_PageUp:
			if(CurId<Instance->GetPopSize()-1) CurId++; else CurId=0;
			Main->setTabText(2,"Solution ("+QString::number(CurId)+"/"+QString::number(Instance->GetPopSize()-1)+")");
			Sol->setLayout(Instance->Chromosomes[CurId]);
			break;

		case Qt::Key_PageDown:
			if(CurId>0) CurId--; else CurId=Instance->GetPopSize()-1;
			Main->setTabText(2,"Solution ("+QString::number(CurId)+"/"+QString::number(Instance->GetPopSize()-1)+")");
			Sol->setLayout(Instance->Chromosomes[CurId]);
			break;

		default:
			e->ignore();
	}
}


//-----------------------------------------------------------------------------
KGAView::~KGAView()
{
	if(Instance)
		delete Instance;
}
