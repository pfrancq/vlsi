/*

  kvlsiprjview.h

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
#ifndef KVLSIPRJVIEW_H
#define KVLSIPRJVIEW_H


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
#include <qlistview.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdevvlsiview.h"


//-----------------------------------------------------------------------------
/**
* The KVLSIPrjView provides a representation of a project window.
* @author Pascal Francq
*/
class KVLSIPrjView : public KDevVLSIView
{
	Q_OBJECT

	/**
	* List representing the elements of the projects.
	*/
	QListView *prj;

public:

	/**
	* Construct the project view.
	*/
	KVLSIPrjView(KDevVLSIDoc* pDoc,QWidget *parent, const char *name,int wflags);

	/**
	* Destruct the project view.
	*/
	~KVLSIPrjView();

	/**
	* Return the type of the window.
	*/
	virtual VLSIViewType getType(void) {return(Project);}

	/**
	* Ask for setting the title.
	*/
	virtual void setTitle(QString _title) {setCaption("Project: "+_title);}

	/**
	* Construct the project tree.
	*/
	void createPrj(void);

protected:

	/**
	* Handle the resize of this window.
	*/
	void resizeEvent(QResizeEvent *);

};


//-----------------------------------------------------------------------------
#endif
