/*

  kdevvlsidoc.h

  Description - Header.

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
#ifndef KDEVVLSIDOC_H
#define KDEVVLSI_H


//-----------------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 


//-----------------------------------------------------------------------------
// include files for R Project
#include <rga2d/rproblem2d.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for QT
#include <qobject.h>
#include <qstring.h>
#include <qlist.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kurl.h>


//-----------------------------------------------------------------------------
// forward declaration of the KDevVLSI classes
class KDevVLSIView;
class KVLSIGAView;
class KVLSIPrjView;
class KVLSIHeuristicView;
class KDevVLSIApp;


//-----------------------------------------------------------------------------
/**
* KDevVLSIDoc provides a document object for a document-view model.
*
* The KDevVLSIDoc class provides a document object that can be used in
* conjunction with the classes KDevVLSIApp and KDevVLSIView to create a
* document-view model for MDI (Multiple Document Interface) KDE 2 applications
* based on KApplication and KTMainWindow as main classes and QWorkspace as MDI
* manager widget. Thereby, the document object is created by the KDevVLSIApp
* instance (and kept in a document list) and contains the document structure
* with the according methods for manipulating the document data by KDevVLSIView
* objects. Also, KDevVLSIDoc contains the methods for serialization of the
* document data from and to files.
* @author Pascal Francq
* @version $Revision$
*/
class KDevVLSIDoc : public QObject, public RProblem2D
{
	Q_OBJECT

	/**
	* The modified flag of the current document.
	*/
	bool modified;

	/**
	* The URL of the document.
	*/
	KURL doc_url;

	/**
	* The list of the views currently connected to the document.
	*/
	QList<KDevVLSIView>* pViewList;	

public:

	/**
	* Constructor for the fileclass of the application.
	*/
	KDevVLSIDoc(void);

	/**
	* Destructor for the fileclass of the application.
	*/
	~KDevVLSIDoc(void);

	/**
	* Adds a view to the document which represents the document contents.
	* Usually this is your main view.
	*/
	void addView(KDevVLSIView *view);
	
	/**
	* Removes a view from the list of currently connected views.
	*/
	void removeView(KDevVLSIView *view);

	/**
	* Gets called if a view is removed or added.
	*/
	void changedViewList(void);

	/**
	* Returns the first view instance.
	*/
	KDevVLSIView* firstView(void){ return pViewList->first(); };

	/**
	* Returns true, if the requested view is the last view of the document.
	*/
	bool isLastView(void);

	/**
	* This method gets called when the user is about to close a frame window. It
	* checks, if more than one view is connected to the document (then the frame
	* can be closed), if pFrame is the last view and the document is modified,
	* the user gets asked if he wants to save the document.
	*/
	bool canCloseFrame(KDevVLSIView* pFrame);

	/**
	* Sets the modified flag for the document after a modifying action on the
	* view connected to the document.
	*/
	void setModified(bool _m=true){ modified=_m; };

	/**
	* Returns if the document is modified or not. Use this to determine if your
	* document needs saving by the user on closing.
	*/
	bool isModified(){ return(modified); };

	/**
	* Deletes the document's contents.
	*/
	void deleteContents(void);

	/**
	* Initializes the document generally.
	*/
	bool newDocument(void);

	/**
	* Closes the acutal document.
	*/
	void closeDocument(void);

	/**
	* Loads the document by filename and format and emits the updateViews()
	* signal.
	*/
	bool openDocument(const KURL &url, const char *format=0)  throw(RIOException);

	/**
	* Saves the document under filename and format.
	*/	
	bool saveDocument(const KURL &url, const char *format=0);
	
	/**
	* Returns the KURL of the document.
	*/
	const KURL& URL(void) const;

	/**
	* sets the URL of the document.
	*/
	void setURL(const KURL& url);

public slots:

	/**
	* Calls repaint() on all views connected to the document object and is
	* called by the view by which the document has been changed.
	* As this view normally repaints itself, it is excluded from the paintEvent.
	*/
	void updateAllViews(KDevVLSIView* sender);

public:

	friend class KDevVLSIApp;
	friend class KDevVLSIView;
	friend class KVLSIPrjView;
	friend class KVLSIHeuristicView;
	friend class KVLSIGAView;
};


//-----------------------------------------------------------------------------
#endif // KDEVVLSIDOC_H
