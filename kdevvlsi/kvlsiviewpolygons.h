/*

  kvlsiviewpolygons.h

  Description - Header.

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
#ifndef KVLSIVIEWPOLYGONS_H
#define KVLSIVIEWPOLYGONS_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rmath/rpolygon.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdevvlsiview.h"
#include "qdrawpolygons.h"



//-----------------------------------------------------------------------------
/**
* @author Pascal Francq
*/
class KVLSIViewPolygons : public KDevVLSIView
{
	Q_OBJECT

	/**
	* Geometric Information for this heuristic.
	*/
	RGeoInfo** infos;

	/**
	* Number of geometric information.
	*/
	unsigned int nbInfos;

	/**
	* Show the polygons.
	*/
	QDrawPolygons *draw;

public:

	/**
	* Constructor of the window.
	*/
	KVLSIViewPolygons(const char* file,QWidget *parent, const char *name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual VLSIViewType getType(void) {return(Polygons);}

protected:

	/**
	* The function that handle the resize event.
	*/
	virtual void resizeEvent(QResizeEvent*);

public:

	/**
	* Destruct the window.
	*/
	~KVLSIViewPolygons(void);
};


//-----------------------------------------------------------------------------
#endif
