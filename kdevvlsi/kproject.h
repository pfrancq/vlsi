/*

	KProject.h

	View of a VLSI project - Header.

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
#ifndef KProjectH
#define KProjectH


//-----------------------------------------------------------------------------
// include files for VLSI project
#include <rproblem2d.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//---------------------------------------------------------------------------
// include files for current application
#include <ui_kproject.h>


//-----------------------------------------------------------------------------
/**
* The KProject provides a representation of a project window.
* @author Pascal Francq
*/
class KProject : public QMdiSubWindow, public Ui_KProject
{
	Q_OBJECT

	/**
	 * The session.
	 */
	RProblem2D* Session;

public:

	/**
	* Construct the project view.
	* @param session         VLSI Session.
	* @param uri             URI of the project.
	*
	*/
	KProject(RProblem2D* session,const QString& uri);

	/**
	* Destruct the project view.
	*/
	~KProject(void);

	/**
	* Construct the project tree.
	*/
	void createPrj(void);
};


//-----------------------------------------------------------------------------
#endif
