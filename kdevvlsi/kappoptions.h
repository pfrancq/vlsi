/*

	RVLSI Project Library

	KAppOptions.h

	Application options - Header.

	Copyright 2000-2011 by Pascal Francq (pascal@francq.info).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
#ifndef KAppOptionsH
#define KAppOptionsH


//------------------------------------------------------------------------------
// include files for current project
#include <ui_kappoptions.h>


//------------------------------------------------------------------------------
/**
 * This class manages the options of the applications.
 */
class KAppOptions : KDialog, Ui_KAppOptions
{
	Q_OBJECT

public:

	/**
	 * Construct the dialog box.
	 * @param parent         Parent widget.
	 */
	KAppOptions(QWidget* parent);

	/**
	 * Execute the dialog boxes. In practice, when the 'Apply' button is
	 * pressed, the variables are applied.
	 * @param app            Application.
	 */
	void exec(KDevVLSI* app);

	/**
	 * Read the options of a configuration file.
	 */
	static void readOptions(void);

	/**
	 * Save the options of a configuration file.
	 */
	static void saveOptions(void);
};


//------------------------------------------------------------------------------
#endif
