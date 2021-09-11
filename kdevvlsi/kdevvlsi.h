/*

	RVLSI Project Library

	KDevVLSI.h

	Main Window - Header.

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
#ifndef KDevVLSIH
#define KDevVLSIH


//-----------------------------------------------------------------------------
// include files for Qt
#include <QMainWindow>


//-----------------------------------------------------------------------------
// include files for current project
#include <qvlsiapp.h>
#include <ui_kdevvlsi.h>


//-----------------------------------------------------------------------------
/**
 * This class represents the main window.
 * @author Pascal Francq.
 * @short Main Window.
 */
class KDevVLSI : public QMainWindow, Ui_KDevVLSI
{
	Q_OBJECT

	/**
	 * Application.
	 */
	QVLSIApp* App;

	/**
	 * "Document" representing the problem to analyse.
	 */
	RProblem2D* Doc;

	/**
	 * Number of recent files to stored.
	 */
	enum {MaxRecentFiles=5};

	/**
	 * Actions for last opened files.
	 */
	QAction* recentFiles[MaxRecentFiles];

	/**
	 * Problem information.
	 */
	RString ProblemInfo;

public:

	/**
	* Constructor of KDevVLSI.
	* @param app             Application.
	*/
	KDevVLSI(QVLSIApp* app);

private:

	/**
	 * Connect the menu entries with the slots.
    */
	void connectMenus(void);

	/**
	 * Call when a file is opened or not. Basically, it enable/disable
	 * the menu items.
	 * @param opened      Opened?
	 */
	void fileOpened(void);

	/**
	* Open a specific file.
	* @param url             URL of the file.
	*/
	void openDocumentFile(const RString& url);

	/**
	 * Update the recent file options.
	 * @param url             URL of the file.
	 */
	void updateRecentFileActions(const RString& url);

	/**
	* Changes the status bar contents for the standard label permanently, used
	* to indicate current actions.
	* @param text            Text that is displayed in the status bar.
	*/
	void statusMsg(const QString& text);

private slots:

	/**
	* Open a file and load it into the document.
	*/
	void openFile(void);

	/**
	* Opens a file from the recent files menu.
	*/
	void openRecentFile(void);

	/**
	* Close the actual VLSI project.
	*/
	void closeFile(void);

	/**
	 * Quit the application.
	 */
	void applicationQuit(void);

	/**
	 * Set the preferences.
	 */
	void optionsPreferences(void);

	/**
	* Do the Bottom-Left heuristic.
	*/
	void heuristicBL(void);

	/**
	* Do the Edge heuristic.
	*/
	void heuristicEdge(void);

	/**
	* Do the Center heuristic.
	*/
	void heuristicCenter(void);

	/**
	* One Step in the heuristic.
	*/
	void heuristicNext(void);

	/**
	* End the heuristic in once.
	*/
	void heuristicRun(void);

	/**
	* Select objects for the current heuristic.
	*/
	void heuristicSelect(void);

	/**
	* Initialise the GA.
	*/
	void GAInit(void);

	/**
	* Start the GA.
	*/
	void GAStart(void);

	/**
	* Pause the GA.
	*/
	void GAPause(void);

	/**
	* Stop the GA.
	*/
	void GAStop(void);

	/**
	* View some polygons in a window.
	*/
	void viewPolygons(void);

	/**
	 * Import of project composed from other files.
	 */
	void importProject(void);

	/**
	 * Export the results.
	 */
	void exportResults(void);

	/**
	 * About dialog box.
	 */
	void about(void);

public:

	/**
	* Destructor of KDevVLSI.
	*/
	virtual ~KDevVLSI(void);

	friend class QVLSIApp;
};


//-----------------------------------------------------------------------------
#endif // KDevVLSI_H
