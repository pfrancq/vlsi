/*

  kdevvlsi.h

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
#ifndef KDEVVLSI_H
#define KDEVVLSI_H


//-----------------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromethee/rpromcriterion.h>
#include <rga/rga2d.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qstrlist.h>
#include <qworkspace.h>
#include <qmultilineedit.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kapp.h>
#include <kmainwindow.h>
#include <kaction.h>
#include <kurl.h>


//-----------------------------------------------------------------------------
// forward declaration of the KDevVLSI classes
class KDevVLSIDoc;
class KDevVLSIView;


//-----------------------------------------------------------------------------
/**
* The base class for KDevVLSI application windows. It sets up the main window
* and reads the config file as well as providing a menubar, toolbar and
* statusbar. In initView(), your main view is created as the MDI child window
* manager. Child windows are created in createClient(), which gets a document
* instance as it's document to display whereby one document can have several
* views.The MDI child is an instance of KDevVLSIView, the document an instance
* of KDevVLSIDoc. KDevVLSIApp reimplements the methods that KTMainWindow
* provides for main window handling and supports full session management as
* well as keyboard accelerator configuration by using KAccel.
* @see KMainWindow
* @see KApplication
* @see KConfig
* @see KAccel
*
* @author Pascal Francq.
*/
class KDevVLSIApp : public KMainWindow
{
	Q_OBJECT

	/**
	* Run the heuristics in step mode.
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
	* Prom�h� Parameters for Heuristic Distance.
	*/
	RPromCriterionParams HeurDist;

	/**
	* Prom�h� Parameters for Heuristic Area.
	*/
	RPromCriterionParams HeurArea;

	/**
	* Prom�h� Parameters for Selection Distance.
	*/
	RPromCriterionParams SelectDist;

	/**
	* Prom�h� Parameters for Selection Weight.
	*/
	RPromCriterionParams SelectWeight;

	/**
	* Heuristic to used for the GA.
	*/
	HeuristicType GAHeur;

	/**
	* Maximum number of generation.
	*/
	unsigned int GAMaxGen;

	/**
	* Step of generation.
	*/
	unsigned int GAStepGen;

	/**
	* Size of the Population.
	*/
	unsigned int GAPopSize;

public:

	/**
	* Construtor of KDevVLSIApp, calls all init functions to create the
	* application.
	* @see initMenuBar initToolBar
	*/
	KDevVLSIApp(void);

	/**
	* Destrutor of KDevVLSIApp.
	*/
	~KDevVLSIApp(void);

	/**
	* Opens a file specified by commandline option.
	*/
	void openDocumentFile(const KURL& url=0);

protected:

	/**
	* queryClose is called by KTMainWindow on each closeEvent of a window.
	* Against the default implementation (only returns true), this overridden
	* function retrieves all modified documents from the open document list and
	* asks the user to select which files to save before exiting the
	* application.
	* @see KTMainWindow#queryClose
	* @see KTMainWindow#closeEvent
	*/
	virtual bool queryClose(void);

	/**
	* queryExit is called by KTMainWindow when the last window of the
	* application is going to be closed during the closeEvent(). Against the
	* default implementation that just returns true, this calls saveOptions() to
	* save the settings of the last window's properties.
	* @see KTMainWindow#queryExit
	* @see KTMainWindow#closeEvent
	*/
	virtual bool queryExit(void);

	/**
	* Saves the window properties for each open window during session end to the
	* session config file, including saving the currently opened file by a
	* temporary filename provided by KApplication.
	* @see KTMainWindow#saveProperties
	*/
	virtual void saveProperties(KConfig *_cfg);

	/**
	* reads the session config file and restores the application's state
	* including the last opened files and documents by reading the temporary
	* files saved by saveProperties().
	* @see KTMainWindow#readProperties
	*/
	virtual void readProperties(KConfig *_cfg);

	/**
	* Event filter to catch close events for MDI child windows and is installed
	* in createClient() on every child window. Closing a window calls the
	* eventFilter first which removes the view from the connected documents'
	* view list. If the last view is going to be closed, the eventFilter() tests
	* if the document is modified; if yes, it asks the user to save the
	* document. If the document title contains "Untitled", slotFileSaveAs() gets
	* called to get a save name and path.
	*/
	virtual bool eventFilter(QObject* object, QEvent* event);

	/**
	* Creates a new child window. The document that will be connected to it has
	* to be created before and the instances filled, with e.g. openDocument().
	* Then call createClient() to get a new MDI child window.
	* @see KDevVLSIDoc#addView
	* @see KDevVLSIDoc#openDocument
	* @param doc            pointer to the document instance that the view will
	*                       be connected to.
	*/
	void createClient(KDevVLSIDoc* doc);

private slots:

	/**
	* Clears the document in the actual view to reuse it as the new document.
	*/
	void slotFileNew(void);

	/**
	* Open a file and load it into the document.
	*/
	void slotFileOpen(void);

	/**
	* Opens a file from the recent files menu.
	*/
	void slotFileOpenRecent(const KURL& url);

	/**
	* Save a document.
	*/
	void slotFileSave(void);

	/**
	* Save a document by a new filename
	*/
	void slotFileSaveAs(void);

	/**
	* Asks for saving if the file is modified, then closes the actual file and
	* window.
	*/
	void slotFileClose(void);

	/**
	* Print the actual file.
	*/
	void slotFilePrint(void);

	/**
	* Closes all documents and quits the application.
	*/
	void slotFileQuit(void);

	/**
	* Reverts the last user action for the active window.
	*/
	void slotEditUndo(void);

	/**
	* Put the marked text/object into the clipboard and remove it from the
	* document.
	*/
	void slotEditCut(void);

	/**
	* Put the marked text/object into the clipboard.
	*/
	void slotEditCopy(void);

	/**
	* Paste the clipboard into the document.
	*/
	void slotEditPaste(void);

	/**
	* Do the Bottom-Left heuristic.
	*/
	void slotHeuristicBL(void);

	/**
	* Do the Edge heuristic.
	*/
	void slotHeuristicEdge(void);

	/**
	* Do the Center heuristic.
	*/
	void slotHeuristicCenter(void);

	/**
	* One Step in the heuristic.
	*/
	void slotHeuristicNext(void);

	/**
	* End the heuristic.
	*/
	void slotEndHeuristic(void);

	/**
	* End the heuristic in once.
	*/
	void slotHeuristicRun(void);

	/**
	* Select objects for the current heuristic.
	*/
	void slotHeuristicSelect(void);

	/**
	* Initialize the GA.
	*/
	void slotGAInit(void);

	/**
	* Start the GA.
	*/
	void slotGAStart(void);

	/**
	* Pause the GA.
	*/
	void slotGAPause(void);

	/**
	* Stop the GA.
	*/
	void slotGAStop(void);

	/**
	* View some polygons in a window.
	*/
	void slotViewPolygons(void);

	/**
	* Toggles the toolbar.
	*/
	void slotViewToolBar(void);

	/**
	* Toggles the statusbar.
	*/
	void slotViewStatusBar(void);

	/**
	* Show the dialog box for the options.
	*/
	void slotSettingsOptions(void);

	/**
	* Creates a new view for the document in the active child window and adds
	* the new view to the list of views the document maintains.
	*/
	void slotWindowNewWindow(void);

	/**
	* Changes the statusbar contents for the standard label permanently, used to
	* indicate current actions.
	* @param text 		the text that is displayed in the statusbar
	*/
	void slotStatusMsg(const QString &text);

	/**
	* Gets called when the window menu is activated; recreates the window menu
	* with all opened window titles.
	*/
	void windowMenuAboutToShow(void);

	/**
	* Activates the MDI child widget when it gets selected from the window menu.
	*/
	void windowMenuActivated(int id);

	/**
	* Put all the windows tile.
	*/
	void slotWindowTile(void);

	/**
	* Put all the window in cascade.
	*/
	void slotWindowCascade(void);

	/**
	* Called when a window is activated.
	*/
	void slotWindowActivated(QWidget* w);

private:

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
	* Initializes the KActions of the application.
	*/
	void initActions(void);

	/**
	* Sets up the statusbar for the main window by initialzing a statuslabel.
	*/
	void initStatusBar(void);

	/**
	* Creates the main view of the KTMainWindow instance and initializes the MDI
	* view area including any needed connections.
	*/
	void initView(void);

	/**
	* The configuration object of the application.
	*/
	KConfig *config;

	/**
	* pWorkspace is the MDI frame widget that handles MDI child widgets.
	* Inititalized in initView().
	*/
	QWorkspace *pWorkspace;

	/**
	* The printer instance.
	*/
	QPrinter *printer;

	/**
	* A counter that gets increased each time the user creates a new document
	* with "File"->"New".
	*/
	int untitledCount;

	/**
	* A list of all open documents. If the last window of a document gets
	* closed, the installed eventFilter removes this document from the list. The
	* document list is checked for modified documents when the user is about to
	* close the application.
	*/
	QList<KDevVLSIDoc> *pDocList;

	// KAction pointers to enable/disable actions
	KAction* fileNew;
	KAction* fileOpen;
	KRecentFilesAction* fileOpenRecent;
	KAction* fileSave;
	KAction* fileSaveAs;
	KAction* fileClose;
	KAction* filePrint;
	KAction* fileQuit;
	KAction* editCut;
	KAction* editCopy;
	KAction* editPaste;
	KAction* windowNewWindow;
	KAction* windowTile;
	KAction* windowCascade;
	KAction* heuristicBL;
	KAction* heuristicEdge;
	KAction* heuristicCenter;
	KAction* heuristicRun;
	KAction* heuristicNext;
	KAction* heuristicSelect;
	KAction* GAInit;
	KAction* GAStart;
	KAction* GAPause;
	KAction* GAStop;
	KAction* ToolsViewPolygons;
	KToggleAction* viewToolBar;
	KToggleAction* viewStatusBar;
	KAction* settingsOptions;
	KActionMenu* windowMenu;
	//QMultiLineEdit* Output;

	// friend classes
	friend class KDevVLSIDoc;
	friend class KVLSIHeuristicView;
	friend class KVLSIGAView;
};


//-----------------------------------------------------------------------------
/**
* Global pointer to the KMainWindow of the Application.
*/
extern KDevVLSIApp* theApp;


//-----------------------------------------------------------------------------
#endif // KDEVVLSI_H
