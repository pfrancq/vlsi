/*

	RVLSI Project Library

	QVLSIApp.cpp

	Main Application - Header.

	Copyright 2000-2016 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef QVLSIAppH
#define QVLSIAppH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rvlsiapp.h>
using namespace R;
using namespace RVLSI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <QApplication>


//-----------------------------------------------------------------------------
// forward declaration
class KDevVLSI;


//-----------------------------------------------------------------------------
/**
 * The QVLSIApp class provides the main application.
 */
class QVLSIApp :  public RVLSIApp
{
	/**
	 * Main Window;
    */
	KDevVLSI* Main;

	/**
	 * Last opened files.
	 */
	RContainer<RString,true,false> LastOpenedFiles;

public:

	/**
	 * Constructor.
    * @param argc           Number of argument.
    * @param argv           Arguments.
    */
	QVLSIApp(int argc, char *argv[]);

	/**
	 * Create the configuration parameters.
    */
	virtual void CreateConfig(void);

	/**
	 * Initialise the application.
    */
	virtual void Init(void);

	/**
	  * "Run" the application. In practice, it just show the main widget and
	  * call QApplication::instance()->exec().
     */
	virtual void Run(void);

	/**
	 * Destructor.
	 */
	virtual ~QVLSIApp(void);

	friend class KDevVLSI;
};


//-----------------------------------------------------------------------------
#endif

