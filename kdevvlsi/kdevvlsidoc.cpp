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
// include files for R Project
#include <rstd/rtextfile.h>
using namespace RStd;
#include <rmath/random.h>
using namespace RMath;
#include <rga2d/rplacementbottomleft.h>
#include <rga2d/rplacementedge.h>
#include <rga2d/rplacementcenter.h>
using namespace RGA;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qdir.h>
#include <qfileinfo.h>
#include <qwidget.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kapp.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kio/job.h>
#include <kio/netaccess.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kdevvlsi.h"
#include "kdevvlsiview.h"
#include "kvlsigaview.h"
#include "kvlsiheuristicview.h"
#include "kdevvlsidoc.h"



//-----------------------------------------------------------------------------
//
// class KDevVLSIDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KDevVLSIDoc::KDevVLSIDoc(void) :
	RProblem2D()
{
	pViewList = new QList<KDevVLSIView>;
	pViewList->setAutoDelete(false);
}


//-----------------------------------------------------------------------------
void KDevVLSIDoc::addView(KDevVLSIView *view)
{
	pViewList->append(view);
	view->setTitle(URL().fileName());
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
		for(i=1,w=pViewList->first(); w!=0; i++, w=pViewList->next())
			w->setTitle(/*QString(*/URL().fileName()/*+":%1").arg(i)*/);
	}
}


//-----------------------------------------------------------------------------
bool KDevVLSIDoc::isLastView(void)
{
	return((int)pViewList->count()==1);
}


//-----------------------------------------------------------------------------
void KDevVLSIDoc::updateAllViews(KDevVLSIView* sender)
{
	KDevVLSIView *w;

	for(w=pViewList->first(); w!=0; w=pViewList->next())
	{
		w->update(sender);
	}
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
	Clear();
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

	KIO::NetAccess::download(url,tmpfile);
	strcpy(Tmp,tmpfile);
	Load(Tmp);

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


//-----------------------------------------------------------------------------
KDevVLSIDoc::~KDevVLSIDoc(void)
{
	delete pViewList;
}
