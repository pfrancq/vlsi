/*

  kvlsigaview.h

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
#ifndef KVLSIGAVIEW_H
#define KVLSIGAVIEW_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rga/rgasignals.h>
#include <rvlsi/rgavlsi.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
#include <qtabwidget.h>


//-----------------------------------------------------------------------------
// include files for QtWidgets
#include <rga/qgamonitor.h>
#include <rga/qxmlcontainer.h>
#include <rga2d/qdrawpolygons.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdevvlsiview.h"


//-----------------------------------------------------------------------------
/**
* The KVLSIGAView class provides a representation of a result of a GA that is
* running.
* @author Pascal Francq
*/
class KVLSIGAView : public KDevVLSIView, public RGASignalsReceiver<RInstVLSI,RChromoVLSI,RFitnessVLSI>
{
	Q_OBJECT

	/**
	* The tab widget.
	*/
	QTabWidget* TabWidget;
	
	/**
	* The widget that handle statistics about the running GA.	
	*/
	QGAMonitor* Monitor;
	
	/**
	* The widget that handle the debug information of the running GA.
	*/
	QXMLContainer* Debug;
	
	/**
	* The splitter that separate the monitor and the debug part.
	*/
	QSplitter* StatSplitter;
	
	/**
	* The best solution.
	*/
	QDrawPolygons* Best;
	
	/**
	* The solutions.
	*/
	QDrawPolygons* Sol;
	
	/**
	* Identificator of the current showed solution.
	*/
	unsigned int CurId;

	/**
	* The GA that will be used.
	*/
	RInstVLSI* Instance;

	/**
	* Number of generation already executed.
	*/
	unsigned int Gen;

public:

	/**
	* Constructor for the view.
	* @param pDoc           your document instance that the view represents. Create a
	*                       document before calling the constructor or connect an
	*                       already existing document to a new MDI child widget.
	*/
	KVLSIGAView(KDevVLSIDoc* pDoc,QWidget *parent, const char *name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual VLSIViewType getType(void) {return(GA);}

	/**
	* GA signal to indicate that a new generation has been done.
	*/
	virtual void receiveGenSig(GenSig* sig);
	
	/**
	* GA signal to interact with the system.
	*/
	virtual void receiveInteractSig(InteractSig *sig);

	/**
	* GA signal to signify that the best chromosome has changed.
	*/
	virtual void receiveBestSig(BestSig *sig);

	/**
	* Run the GA.
	*/
	void RunGA(void);

	/**
	* Pause the GA.
	*/
	void PauseGA(void);

	/**
	* Stop the GA.
	*/
	void StopGA(void);

protected:

	/**
	* Key release event method. The implementation is needed to change the
	* chromosome to show when the user uses the keys "Page Up" and "Page Down".
	* With "Ctrl-G", a dialog box appears to select directly the identificator.
	*/
	virtual void keyReleaseEvent(QKeyEvent* e);

	/**
	* The function that handle the resize event.
	*/
	virtual void resizeEvent(QResizeEvent*);

signals:

	/**
	* Signal to emit after a generation is done.
	*/
	void signalSetGen(unsigned int gen,unsigned int best,double value);

public:

	/**
	* Destruct the view.
	*/
	~KVLSIGAView();

	// friend classes
	friend class KDevVLSIDoc;
};


//-----------------------------------------------------------------------------
#endif
