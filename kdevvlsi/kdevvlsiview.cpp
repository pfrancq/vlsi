/*

  kdevvlsiview.cpp

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

// include files for Qt
#include <qprinter.h>
#include <qpainter.h>
#include <qdir.h>
// include files for KDE

// application specific includes
#include "kdevvlsi.h"
#include "kdevvlsiview.h"
#include "kdevvlsidoc.h"

KDevVLSIView::KDevVLSIView(KDevVLSIDoc* pDoc, QWidget *parent, const char* name, int wflags)
 : QWidget(parent, name, wflags)
{
    doc=pDoc;
}

KDevVLSIView::~KDevVLSIView()
{
}

KDevVLSIDoc *KDevVLSIView::getDocument() const
{
	return doc;
}

void KDevVLSIView::update(KDevVLSIView* pSender){
	if(pSender != this)
		repaint();
}

void KDevVLSIView::print(QPrinter *pPrinter)
{
  if (pPrinter->setup(this))
  {
		QPainter p;
		p.begin(pPrinter);
		
		///////////////////////////////
		// TODO: add your printing code here
		///////////////////////////////
		
		p.end();
  }
}

void KDevVLSIView::closeEvent(QCloseEvent* e){

// DO NOT CALL QWidget::closeEvent(e) here !!
// This will accept the closing by QCloseEvent::accept() by default.
// The installed eventFilter() in KDevVLSIApp takes care for closing the widget
// or ignoring the close event
		
}
