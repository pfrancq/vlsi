/*

  kdevvlsidoc.cpp

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
// include files for Rainbow
#include <rstd/rtextfile.h>
using namespace RStd;
#include <rga/rplacementbottomleft.h>
#include <rga/rplacementedge.h>
#include <rga/rplacementcenter.h>
using namespace RGA;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qdir.h>
#include <qfileinfo.h>
#include <qwidget.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <klocale.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kio/job.h>
#include <kio/netaccess.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kdevvlsi.h"
#include "kdevvlsiview.h"
#include "kvlsiheuristicview.h"
#include "kdevvlsidoc.h"


//-----------------------------------------------------------------------------
//
// class KDevVLSIDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KDevVLSIDoc::KDevVLSIDoc(void)
	:  curView(0),PlacementHeuristic(0)
{
	pViewList = new QList<KDevVLSIView>;
	pViewList->setAutoDelete(false);
}


//-----------------------------------------------------------------------------
KDevVLSIDoc::~KDevVLSIDoc(void)
{
	delete pViewList;
}


//-----------------------------------------------------------------------------
void KDevVLSIDoc::addView(KDevVLSIView *view)
{
	pViewList->append(view);
	changedViewList();
}


//-----------------------------------------------------------------------------
void KDevVLSIDoc::removeView(KDevVLSIView *view)
{
	pViewList->remove(view);
	if(!pViewList->isEmpty())
		changedViewList();
	else
		deleteContents();
}


//-----------------------------------------------------------------------------
void KDevVLSIDoc::changedViewList(void)
{	
	KDevVLSIView *w;

	if((int)pViewList->count()==1)
	{
		w=pViewList->first();
		w->setTitle(URL().fileName());
	}
	else
	{	
		int i;
		for( i=1,w=pViewList->first(); w!=0; i++, w=pViewList->next())
			w->setTitle(/*QString(*/URL().fileName()/*+":%1").arg(i)*/);	
	}
}


//-----------------------------------------------------------------------------
bool KDevVLSIDoc::isLastView(void)
{
	return((int)pViewList->count()==1);
}


//-----------------------------------------------------------------------------
void KDevVLSIDoc::updateAllViews(KDevVLSIView *sender)
{
	KDevVLSIView *w;

	for(w=pViewList->first(); w!=0; w=pViewList->next())
	{
		w->update(sender);
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIDoc::slotHeuristic(HeuristicType h,bool pStep,KVLSIHeuristicView* v)
{
	if(!NbObjs) return;

	// Verify that no current heuristic works.
	if(curView&&Run)
	{
		if(KMessageBox::warningContinueCancel(curView,i18n("An heuristic is calculated.\nAre you sure you want to stop it?"),
									i18n("Warning"),i18n("Stop"),true)==KMessageBox::Cancel)
			return;
		emit breakRun();
		curView->close();
	}
	curView=v;
   connect(this,SIGNAL(endRun(void)),curView,SLOT(slotEndRun(void)));
   connect(this,SIGNAL(beginRun(void)),curView,SLOT(slotBeginRun(void)));
   connect(this,SIGNAL(breakRun(void)),curView,SLOT(slotBreakRun(void)));

	// Clear geometric information
	for(unsigned i=0;i<NbObjs;i++)
		Infos[i]->Clear();

	// Init the heuristic
	if(PlacementHeuristic) delete PlacementHeuristic;
	switch(h)
	{
		case BottomLeft:
			PlacementHeuristic = new RPlacementBottomLeft(NbObjs,true);
			break;

		case Edge:
			PlacementHeuristic = new RPlacementEdge(NbObjs,true);
			break;

		case Center:
			PlacementHeuristic = new RPlacementCenter(NbObjs,true);
			break;
	}
	PlacementHeuristic->Init(Limits,grid,Objs,Infos,NbObjs);
	nbFree=0;

	// Run the heuristic
	emit beginRun();
	Run=true;
	if(pStep)
		slotNextStep(pStep);
	else
		while(!PlacementHeuristic->IsEnd())
			slotNextStep(pStep);
}


//-----------------------------------------------------------------------------
void KDevVLSIDoc::slotNextStep(bool pStep)
{
	CurInfo=PlacementHeuristic->NextObject();
	RFreePolygons *frees;

	if(pStep)
	{
		curView->addInfo(CurInfo);
		frees=PlacementHeuristic->GetFreePolygons();
		while(frees->NbPtr>nbFree)
			curView->addFree(frees->Tab[nbFree++]);
   }

	// test if the end
	if(PlacementHeuristic->IsEnd())
	{
		PlacementHeuristic->PostRun(Limits);
		RRect Rect=PlacementHeuristic->GetResult();
		ActLimits.X=Rect.Width();
		ActLimits.Y=Rect.Height();
		Run=false;
		emit endRun();
   	disconnect(curView);
		curView=0;
		return;
	}
}


//-----------------------------------------------------------------------------
void KDevVLSIDoc::slotRun(void)
{
	if(!Run) return;
	while(!PlacementHeuristic->IsEnd())
		slotNextStep(false);
}


//-----------------------------------------------------------------------------
void KDevVLSIDoc::setURL(const KURL &url)
{
	doc_url=url;
}


//-----------------------------------------------------------------------------
const KURL& KDevVLSIDoc::URL(void) const
{
	return doc_url;
}


//-----------------------------------------------------------------------------
void KDevVLSIDoc::closeDocument(void)
{
	KDevVLSIView *w;
	unsigned int i;
	RGeoInfo **info;
	RObj2D **obj;

	// Delete Views
	if(!isLastView())
	{
		for(w=pViewList->first(); w!=0; w=pViewList->next())
		{
			if(!w->close())
			break;
		}
	}
	if(isLastView())
	{
		w=pViewList->first();
		w->close();
	}

	// Delete Objs
	if(Objs)
	{
		for(i=NbObjs+1,obj=Objs;--i;obj++)
			delete (*obj);
		delete[] Objs;
		Objs=0;
	}

	// Delete Infos
	if(Infos)
	{
		for(i=NbObjs+1,info=Infos;--i;info++)
			delete (*info);
		delete[] Infos;
		Infos=0;
	}
	NbObjs=0;

	// Delete Grid
	if(grid)
	{
		delete grid;
		grid=0;
	}
}


//-----------------------------------------------------------------------------
bool KDevVLSIDoc::newDocument(void)
{
	// TODO: Add your document initialization code here
	modified=false;
	return true;
}


//-----------------------------------------------------------------------------
bool KDevVLSIDoc::openDocument(const KURL &url, const char* /*format*/ /*=0*/)
{
	QString tmpfile;
	static char Tmp[300];
	RPoint Pt;
	unsigned int i;
	unsigned int NbPts;
	RGeoInfo **info;
	RObj2D **obj;

	KIO::NetAccess::download(url,tmpfile);
	strcpy(Tmp,tmpfile);
	RTextFile f(Tmp,RTextFile::Read);

	// Read Limits and NbObjects -> Create them
	f>>Limits.X>>Limits.Y>>NbObjs;
	Objs=new RObj2D*[NbObjs];
	Infos=new RGeoInfo*[NbObjs];

	// Read Objs
	for(i=0,obj=Objs,info=Infos;i<NbObjs;i++,obj++,info++)
	{
		(*obj) = new RObj2D(i,false);
		(*info) = new RGeoInfo(*obj);
		f>>NbPts;
		for(unsigned int j=0;j<NbPts;j++)
		{
			f>>Pt.X>>Pt.Y;
			(*obj)->Polygon.InsertPtr(new RPoint(Pt));
		}
		(*obj)->SetOri(Normal);
		(*obj)->SetOri(NormalX);
		(*obj)->SetOri(NormalY);
		(*obj)->SetOri(NormalYX);
		(*obj)->SetOri(Rota90);
		(*obj)->SetOri(Rota90X);
		(*obj)->SetOri(Rota90Y);
		(*obj)->SetOri(Rota90YX);
		(*obj)->Init();
	}

	// Init Grid
	grid=new RGrid(Limits);

	// Remove file
	KIO::NetAccess::removeTempFile(tmpfile);
	doc_url=url;	
	modified=false;
	return true;
}


//-----------------------------------------------------------------------------
bool KDevVLSIDoc::saveDocument(const KURL& /*url*/, const char* /*format*/ /*=0*/)
{
//	QFile f( filename );
//	if ( !f.open( IO_WriteOnly ) )
//		return false;
//
//  // TODO: Add your document saving code here
//
//  f.close();
//
//  modified=false;
//	m_filename=filename;
//	m_title=QFileInfo(f).fileName();
	return true;
}


//-----------------------------------------------------------------------------
void KDevVLSIDoc::deleteContents(void)
{
	// TODO: Add implementation to delete the document contents
}


//-----------------------------------------------------------------------------
bool KDevVLSIDoc::canCloseFrame(KDevVLSIView* pFrame)
{
	if(!isLastView())
		return true;
		
	bool ret=false;
	if(isModified())
	{
		KURL saveURL;
		switch(KMessageBox::warningYesNoCancel(pFrame, i18n("The current file has been modified.\n"
                          "Do you want to save it?"),URL().fileName()))
		{
			case KMessageBox::Yes:
				if(URL().fileName().contains(i18n("Untitled")))
				{
					saveURL=KFileDialog::getSaveURL(QDir::currentDirPath(),i18n("*|All files"), pFrame, i18n("Save as..."));
					if(saveURL.isEmpty())
						return false;
				}
				else
					saveURL=URL();
					
				if(!saveDocument(saveURL))
				{
 					switch(KMessageBox::warningYesNo(pFrame,i18n("Could not save the current document !\n"
																												"Close anyway ?"), i18n("I/O Error !")))
					{
 						case KMessageBox::Yes:
 							ret=true;
 						case KMessageBox::No:
 							ret=false;
 					}	        			
				}
				else
					ret=true;
				break;

			case KMessageBox::No:
				ret=true;
				break;

			case KMessageBox::Cancel:

			default:
				ret=false; 				
				break;
		}
	}
	else
		ret=true;
		
	return(ret);
}
