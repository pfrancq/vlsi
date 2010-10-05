/*

	RVLSI Project Library

	KHeuristic.h

	Run a heuristic on the problem - Header.

	Copyright 2000-2010 by Pascal Francq (pascal@francq.info).

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
#ifndef KHeuristic_H
#define KHeuristic_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgeoinfo.h>
#include <rlayout.h>
#include <rplacementheuristic.h>
using namespace R;



//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//-----------------------------------------------------------------------------
// include files for current application
#include <ui_kheuristic.h>


//-----------------------------------------------------------------------------
/**
* The KHeuristic class provides a representation of a result of a
* specific heuristic.
* @author Pascal Francq
*/
class KHeuristic : public QMdiSubWindow, public Ui_KHeuristic, public RLayout
{
	Q_OBJECT

	/**
	 * The session.
	 */
	RProblem2D* Session;

	/**
	* Random number generator.
	*/
	RRandom Random;

	/**
	* Grid.
	*/
	RGrid Grid;

	/**
	* Pointer to free polygons to paint.
	*/
	RFreePolygons* Free;

	/**
	* Heuristic used.
	*/
	RPlacementHeuristic* PlacementHeuristic;

	/**
	* Count the number of free polygons actually calculated.
	*/
	size_t NbFree;

	/**
	* Current information to treat.
	*/
	RGeoInfo* CurInfo;

	/**
	* Stop the heuristic.
	*/
	bool Stop;

	/**
	 * Step mode ?
	 */
	bool Step;

	/**
	 * Type of the heuristic.
	 */
	RString Heuristic;

public:

	/**
	* Construct a heuristic view.
	* @param session         VLSI session.
	* @param heuristic       Type of the heuristic.
	*/
	KHeuristic(RProblem2D* session,const RString& heuristic);

	/**
	* Start the heuristic.
	*/
	void RunHeuristic(void);

	/**
	* Next step for the chosen Heuristic.
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
//	void addInfo(RGeoInfo* info) {Draw->addInfo(info);}

	/**
	* Set the connections
	*/
//	void setCons(RConnections* c) {Draw->setCons(c);}

	/**
	* Add a free polygon "on the fly"
	*/
//	void addFree(RFreePolygon* poly) {Draw->addFree(poly);}

	/**
	* The widget has changed and has to be repainted.
	*/
//	void setChanged(void) {Draw->setChanged();}

	/**
	* See if the heuristic is running or not.
	* @return true if the heuristic is running.
	*/
	bool Running(void) {return(!PlacementHeuristic->IsEnd());}

signals:

	/**
	* This signal indicates that the heuristic has finish to run.
	*/
	void endRun(void);

public:

	/**
	*  Destruct a heuristic view.
	*/
	~KHeuristic(void);
};


//-----------------------------------------------------------------------------
#endif
