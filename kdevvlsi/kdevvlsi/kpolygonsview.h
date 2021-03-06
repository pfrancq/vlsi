/*

	RVLSI Project Library

	KPolygonsView.h

	View a set of polygons from a file - Header.

	Copyright 2000-2016 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#ifndef KPolygonsView_H
#define KPolygonsView_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rlayout.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//-----------------------------------------------------------------------------
// include files for current application
#include <ui_kpolygonsview.h>


//-----------------------------------------------------------------------------
/**
* @author Pascal Francq
*/
class KPolygonsView : public QMdiSubWindow, public Ui_KPolygonsView
{
	Q_OBJECT

	/**
	* Layout.
	*/
	RLayout* Layout;

public:

	/**
	* Constructor of the window.
	*/
	KPolygonsView(void);

	/**
	 * Load a given file.
	 * @param url             URL of the file.
	 */
	void Load(QString& url);

	/**
	* Destruct the window.
	*/
	~KPolygonsView(void);
};


//-----------------------------------------------------------------------------
#endif
