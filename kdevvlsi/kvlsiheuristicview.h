/*

  kvlsiheuristicview.h

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
#ifndef KVLSIHEURISTICVIEW_H
#define KVLSIHEURISTICVIEW_H


//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rga/rga2d.h>
#include <rga/rgeoinfo.h>
using namespace RGA;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
#include <qlabel.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdevvlsiview.h"
#include "qdrawpolygons.h"


//-----------------------------------------------------------------------------
/**
* The KVLSIHeuristicView class provides a representation of a result of a
* specific heuristic.
* @author Pascal Francq
*/
class KVLSIHeuristicView : public KDevVLSIView
{
   Q_OBJECT

	/**
	* Type of the heuristic.
	*/
	HeuristicType type;

	/**
	* Number of objects.
	*/
	unsigned int nbObjs;

	/**
	* Geometric Information for this heuristic.
	*/
	RGeoInfo **infos;

	/**
	* Pointer to free polygons to paint.
	*/
	RFreePolygons *free;

	/**
	* Show information about the result.
	*/
	QLabel *result;

	/**
	* Show the polygons.
	*/
	QDrawPolygons *draw;

public:

	/**
	* Construct a heuristic view.
	*/
	KVLSIHeuristicView(KDevVLSIDoc* pDoc,HeuristicType pType,QWidget *parent, const char *name,int wflags);

	/**
	*  Destruct a heuristic view.
	*/
	~KVLSIHeuristicView();

	/**
	* Return the type of the window.
	*/
	virtual VLSIViewType getType(void) {return(Heuristic);}

	/**
	* Ask for setting the title.
	*/
	virtual void setTitle(QString _title);

	/**
	* Add a new RGeoInfo "on the fly"
	*/
	void addInfo(RGeoInfo* info) {draw->addInfo(info);}

	/**
	* Add a free polygon "on the fly"
	*/
	void addFree(RFreePolygon* poly) {draw->addFree(poly);}

protected:

	/**
	* Resize event method.
	*/
	virtual void resizeEvent(QResizeEvent *);

public slots:

	/**
	* Call and the begin of an heuristic.
	*/
	void slotBeginRun(void);

	/**
	* Call to tell the heuristic is break.
	*/
	void slotBreakRun(void);

	/**
	* Call to tell the end of an heuristic.
	*/
	void slotEndRun(void);

};


//-----------------------------------------------------------------------------
#endif
