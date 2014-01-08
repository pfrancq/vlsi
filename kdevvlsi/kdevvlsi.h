/*

	RVLSI Project Library

	KDevVLSI.h

	Main Window - Header.

	Copyright 2000-2014 by Pascal Francq (pascal@francq.info).

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
// include files for R Project
#include <rvlsiapp.h>
using namespace R;
using namespace RVLSI;


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QMdiArea>
#include <QtCore/QList>
#include <QtGui/QLabel>
#include <kaction.h>
#include <krecentfilesaction.h>
#include <kxmlguiwindow.h>


//-----------------------------------------------------------------------------
/**
 * This class represents the main window.
 * @author Pascal Francq.
 * @short Main Window.
 */
class KDevVLSI : public KXmlGuiWindow, public RVLSIApp
{
	Q_OBJECT

	/**
	 * Desktop of the application.
	 */
	QMdiArea* Desktop;

	/**
	 * Label to hold an image representing the status of the file opened.
	 */
	QLabel* Status;

	/**
	 * Connect to the session.
	 */
	KAction* aFileOpen;

	/**
	 * Open a recent file.
	 */
	KRecentFilesAction* aFileOpenRecent;

	/**
	 * Import a VLSI project.
	 */
	KAction* aImport;

	/**
	 * All available actions once a file is connected.
	 */
	QList<KAction*> Actions;

	/**
	 * "Document" representing the problem to analyze.
	 */
	RProblem2D* Doc;

public:

	/**
	* Constructor of KDevVLSI.
	* @param argc            Number of arguments.
	* @param argv            Values of arguments.
	*/
	KDevVLSI(int argc, char *argv[]);

private:

	/**
	 * Create an action for a given menu item.
	 * @param title          Title of the menu item.
	 * @param name           Name of the action (as appearing in .rc file).
	 * @param slot           Corresponding slot.
	 * @param icon           Icon associated.
	 * @param key            Shortcut associated.
	 */
	KAction* addAction(const char* title,const char* name,const char* slot,const char* icon=0,const char* key=0);

	/**
	* Initializes the KActions of the application.
	*/
	void initActions(void);

	/**
	* Save general Options like all bar positions and status as well as the
	* geometry and the recent file list to the configuration file.
	*/
	void saveOptions(void);

	/**
	* Read general Options again and initialize all variables like the recent
	* file list.
	*/
	void readOptions(void);

	/**
	 * Call when a file is opened or not. Basically, it enable/disable
	 * the menu items.
	 * @param opened      Opened?
	 */
	void fileOpened(bool opened);

	/**
	* Open a specific file.
	* @param url             URL of the file.
	*/
	void openDocumentFile(const KUrl& url);

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
	void openRecentFile(const KUrl& url);

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
	* Initialize the GA.
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

public:

	/**
	* Destructor of KDevVLSI.
	*/
	~KDevVLSI(void);
};


//-----------------------------------------------------------------------------
#endif // KDevVLSI_H
