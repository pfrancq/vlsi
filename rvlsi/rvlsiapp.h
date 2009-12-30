/*

	RVLSIApp.h

	Application handling a GALILEI Session - Header.

	Copyright 2000-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2003 by the Université Libre de Bruxelles (ULB).

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
#ifndef RVLSIAppH
#define RVLSIAppH


//------------------------------------------------------------------------------
// include files for R
#include <rapplication.h>
#include <rdebug.h>
#include <rproblem2d.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RVLSIApp class represents a generic application running a session.
* @short RVLSI Application
* @author Pascal Francq
*/
class RVLSIApp : public RApplication
{
protected:

	/**
	* A log file.
	*/
	RTextFile* Log;

	/**
	* Debug file.
	*/
	RDebugXML* Debug;

	/**
	* The Session.
	*/
	RProblem2D* Session;

	/**
	* The Log file.
	*/
	RString LogFileName;

	/**
	* The debug file.
	*/
	RString DebugFileName;

	/**
	* Main configuration of VSLI.
	*/
	RConfig VLSIConfig;

public:

	/**
	* Run the heuristics in step mode.
	*/
	bool Step;

	/**
	* Calculate free polygons.
	*/
	bool ComputeFreePolygons;

	/**
	* Use free polygons.
	*/
	bool UseFreePolygons;

	/**
	* Test all possible orientation.
	*/
	bool AllOrientations;

	/**
	* PROMETHEE Parameters for Heuristic Distance.
	*/
	RParamStruct* HeurDist;

	/**
	* PROMETHEE Parameters for Heuristic Area.
	*/
	RParamStruct* HeurArea;

	/**
	* PROMETHEE Parameters for Selection Distance.
	*/
	RParamStruct* SelectDist;

	/**
	* PROMETHEE Parameters for Selection Weight.
	*/
	RParamStruct* SelectWeight;

	/**
	* Heuristic to used for the GA.
	*/
	RString Heuristic;

	/**
	* Maximum number of generation.
	*/
	size_t MaxGen;

	/**
	* Step of generation.
	*/
	size_t StepGen;

	/**
	* Size of the Population.
	*/
	size_t PopSize;

public:

	/**
	* Construct the application.
	* @param name            Name of the application.
	* @param argc            Number of arguments.
	* @param argv            Arguments.
	*/
	RVLSIApp(const RString& name,int argc, char *argv[]);

	/**
	* Get a pointer over the configuration of GALILEI.
	*/
	RConfig* GetVLSIConfig(void) {return(&VLSIConfig);}

	/**
	* Create the configuration structure. New parameters can be added by
	* defining a new method.
	*/
	virtual void CreateConfig(void);

	/**
	* Initialization of the application.
	*/
	virtual void Init(void);

	/**
	* Create the session.
	* @param file            File containing the VLSI session.
	*/
	RProblem2D* CreateSession(const RString& file);

	/**
	 * Get the session.
	 */
	RProblem2D* GetSession(void) const {return(Session);}

	/**
	* Delete the session.
	*/
	void DeleteSession(void);

	/**
	* Get the name of the log file.
	*/
	RString GetLogFileName(void) const {return(LogFileName);}

	/**
	* Set the name of the log file.
	* @param name            Name.
	*/
	void SetLogFileName(const RString& name) {LogFileName=name;}

	/**
	* @return the name of the debug file.
	*/
	RString GetDebugFileName(void) const {return(DebugFileName);}

	/**
	* Set the name of the debug file.
	* @param name            Name.
	*/
	void SetDebugFileName(const RString& name) {DebugFileName=name;}

	/**
	 * Write to the log file something (if a log file is defined).
	 */
	void WriteLog(const R::RString& str);

	/**
	* Apply the configuration.
	*/
	void Apply(void);

	/**
	* Destruct the application.
	*/
	virtual ~RVLSIApp(void);
};


//------------------------------------------------------------------------------
// Global variables
/**
* Pointer to the current application running a VLSI session.
*/
extern RVLSIApp* VLSIApp;


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
