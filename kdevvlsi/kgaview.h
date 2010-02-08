/*

	RVLSI Project Library

	KGAView.h

	Window to manage a GA instance - Header.

	Copyright 2000-2009 by Pascal Francq (pascal@francq.info).

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
#ifndef KGAVIEW_H
#define KGAVIEW_H


//-----------------------------------------------------------------------------
// include files for R/VLSI project
#include <rinstvlsi.h>
using namespace R;
using namespace RVLSI;
using namespace std;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//---------------------------------------------------------------------------
// include files for current application
#include <ui_kgaview.h>


//-----------------------------------------------------------------------------
/**
* The KVLSIGAView class provides a representation of a result of a GA that is
* running.
* @author Pascal Francq
*/
class KGAView : public QMdiSubWindow, public Ui_KGAView, public RObject
{
	Q_OBJECT

	/**
	* Identifier of the current showed solution.
	*/
	size_t CurId;

	/**
	* The GA that will be used.
	*/
	RInstVLSI* Instance;

	/**
	* Number of generation already executed.
	*/
	size_t Gen;

	/**
	 * Number of windows.
	 */
	static size_t WinNb;

public:

	/**
	* Constructor for the view.
	*/
	KGAView(void);

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
	 * catch a generation notification.
	 */
	void Generation(const R::RNotification& notification);

	/**
	 * catch a best notification.
	 */
	void BestChromo(const R::RNotification& notification);

	/**
	 * catch a interact notification.
	 */
	void Interact(const R::RNotification& notification);

	/**
	* Key release event method. The implementation is needed to change the
	* chromosome to show when the user uses the keys "Page Up" and "Page Down".
	* With "Ctrl-G", a dialog box appears to select directly the identifier.
	*/
	virtual void keyReleaseEvent(QKeyEvent* e);

public:

	/**
	* Destruct the view.
	*/
	~KGAView();

	// friend classes
	friend class KDevVLSIDoc;
};


//-----------------------------------------------------------------------------
#endif
