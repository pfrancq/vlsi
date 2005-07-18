/*

  kvlsiheuristicview.h

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
#ifndef KVLSIHEURISTICVIEW_H
#define KVLSIHEURISTICVIEW_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgeoinfo.h>
#include <rgeoinfos.h>
#include <rconnections.h>
#include <rplacementheuristic.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
#include <qlabel.h>


//-----------------------------------------------------------------------------
// include files for Qt Widgets
#include <qdrawpolygons.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdevvlsiview.h"


//-----------------------------------------------------------------------------
/**
* The KVLSIHeuristicView class provides a representation of a result of a
* specific heuristic.
* @author Pascal Francq
*/
class KVLSIHeuristicView : public KDevVLSIView, public RGeoInfos
{
	Q_OBJECT

	/**
	* Random number generator.
	*/
	RRandom* Random;

	/**
	* Type of the heuristic.
	*/
	HeuristicType type;

	/**
	* Number of objects.
	*/
	unsigned int nbObjs;

	/**
	* Grid.
	*/
	RGrid* grid;

	/**
	* Pointer to free polygons to paint.
	*/
	RFreePolygons* free;

	/**
	* Show information about the result.
	*/
	QLabel* result;

	/**
	* Show the polygons.
	*/
	QDrawPolygons* draw;

	/**
	* Heuristic used.
	*/
	RPlacementHeuristic* PlacementHeuristic;

	/**
	* Count the number of free polygons actually calculated.
	*/
	unsigned int nbFree;

	/**
	* Current information to treat.
	*/
	RGeoInfo* CurInfo;

	/**
	* Step Mode.
	*/
	bool step;

	/**
	* Calculate free polygons.
	*/
	bool calcFree;

	/**
	* Use free polygons.
	*/
	bool useFree;

	/**
	* Test all possible orientation.
	*/
	bool allOri;

	/**
	* Stop the heuristic.
	*/
	bool Stop;

	/**
	* Prom�h� Parameters for Heuristic Distance.
	*/
	RPromCriterionParams HeurDist;

	/**
	* Prom�h� Parameters for Heuristic Area.
	*/
	RPromCriterionParams HeurArea;

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
	* Start the heuristic.
	*/
	void RunHeuristic(void);

	/**
	* Next step for the choosen Heuristic.
	*/
	void NextStep(void);

	/**
	* Run the heuristic to the end.
	*/
	void RunToEnd(void);

	/**
	* Select the objects.
	*/
	void SelectObjects(void);

	/**
	* Add a new RGeoInfo "on the fly"
	*/
	void addInfo(RGeoInfo* info) {draw->addInfo(info);}

	/**
	* Set the connections
	*/
	void setCons(RConnections* c) {draw->setCons(c);}

	/**
	* Add a free polygon "on the fly"
	*/
	void addFree(RFreePolygon* poly) {draw->addFree(poly);}

	/**
	* The widget has changed and has to be repainted.
	*/
	void setChanged(void) {draw->setChanged();}

	/**
	* See if the heurisitic is running or not.
	* @return true if the heuristic is running.
	*/
	bool Running(void) {return(!PlacementHeuristic->IsEnd());}

protected:

	/**
	* Resize event method.
	*/
	virtual void resizeEvent(QResizeEvent *);

signals:

	/**
	* This signal indicates that the heuristic has finish to run.
	*/
	void endRun(void);

};


//-----------------------------------------------------------------------------
#endif
