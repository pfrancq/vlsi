/*

  kdevvlsiview.h

  Description - Header.

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
#ifndef KDEVVLSIVIEW_H
#define KDEVVLSIVIEW_H


//-----------------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>


//-----------------------------------------------------------------------------
// forward declaration
class KDevVLSIDoc;


//-----------------------------------------------------------------------------
// include files for current application
#include "kdevvlsi.h"


//-----------------------------------------------------------------------------
// Enum declaration
/**
* Defines all the possible type of view for a document.
*/
enum VLSIViewType{Nothing,Project,Heuristic,GA,Polygons};


//-----------------------------------------------------------------------------
/**
* The KDevVLSIView class provides the view widget for the document instance
* connected to it and is displayed as a MDI child window in the main view area
* of the KDevVLSIApp class instance. The KDevVLSIApp class also has an
* eventFilter() method that gets installed on every KDevVLSIView instance to
* control events of the type QEvent::Close.	
* The document connected to the view instance keeps a list of all view that
* represent the document contents as there can be more than one view. Views get
* created in KDevVLSIApp::createClient() and automatically added to the list of
* views.
* The KDevVLSIView class inherits QWidget as a base. Another possible
* inheritance besides specialized widgets could be QMainWindow so that you can
* easily set up the main area of your view by setting another view as main
* widget (QMainWindow::setMainWidget() ).
* NOTE: The close event always has to be empty (DON`T CALL
* QWidget::closeEvent(e) in closeEvent()) because the installed event filter
* can only manage a forward implementation. If the QCloseEvent is received by
* the KDevVLSIView, the overwritten event handler has to do nothing as the
* eventFilter has set accept() or ignore() already. If QWidget::closeEvent() is
* called again, the default event handler will accept the close event and the
* window gets destroyed even if the installed eventFilter has set the event to
* be ignored.	
* @author Pascal Francq
* @version $Revision$
*/
class KDevVLSIView : public QWidget
{
	Q_OBJECT

	friend class KDevVLSIDoc;

public:

	/**
	* Constructor for the view.
	* @param pDoc           your document instance that the view represents. Create a
	*                       document before calling the constructor or connect an
	*                       already existing document to a new MDI child widget.
	*/
	KDevVLSIView(KDevVLSIDoc* pDoc, QWidget* parent, const char *name, int wflags);

	/**
	* Destructor for the main view.
	*/
	~KDevVLSIView(void);

	/**
	* Returns a pointer to the document connected to the view.
	*/
	KDevVLSIDoc *getDocument(void) const;

	/**
	* Gets called to redraw the document contents if it has been modified.
	*/
	void update(KDevVLSIView* pSender);

	/**
	* Return the type of the window.
	*/
	virtual VLSIViewType getType(void)=0;

	/**
	* Ask for setting the title.
	*/
	virtual void setTitle(QString _title) {setCaption(_title);}

	/**
	* Contains the implementation for printing functionality and gets called by
	* KDevVLSIApp::slotFilePrint().
	*/
	void print(QPrinter *pPrinter);

protected:

	/**
	* Overwritten QWidget::closeEvent() to catch closing views. Does nothing, as
	* the closeEvents for KDevVLSIView's are processed by
	* KDevVLSIApp::eventFilter(), so this overwitten closeEvent is necessary and
	* has to be empty. Don't overwrite this method !
	*/
	virtual void closeEvent(QCloseEvent* e);

	/**
	* Return a pointer to the application. These views have (normally) the
	* QWorkspace as parent, which himself as QVBox as parent, which is a child
	* of KDevVLSIApp.
	*/
	KDevVLSIApp* getApp(void) {return((KDevVLSIApp*)(parentWidget()->parentWidget()->parentWidget()->parentWidget()));}

	/**
	* The document connected to the view, specified in the constructor.
	*/
	KDevVLSIDoc *doc;
};


//-----------------------------------------------------------------------------
#endif // KDEVVLSIVIEW_H
