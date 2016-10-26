/*

	RVLSIApp.h

	Application handling a GALILEI Session - Header.

	Copyright 2000-2014 by Pascal Francq (pascal@francq.info).
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



//-----------------------------------------------------------------------------
#ifndef RVLSIAppH
#define RVLSIAppH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rapplication.h>
#include <rdebug.h>
#include <rproblem2d.h>


//-----------------------------------------------------------------------------
namespace RVLSI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RVLSIApp class represents a generic application running a session.
* @short RVLSI Application
*/
class RVLSIApp : public R::RApplication
{
protected:

	/**
	* A log file.
	*/
	R::RTextFile* Log;

	/**
	* Debug file.
	*/
	R::RDebugXML* Debug;

	/**
	* The Session.
	*/
	R::RProblem2D* Session;

	/**
	* The Log file.
	*/
	R::RString LogFileName;

	/**
	* The debug file.
	*/
	R::RString DebugFileName;

	/**
	* Main configuration of VSLI.
	*/
	R::RConfig VLSIConfig;

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
	R::RParamStruct* HeurDist;

	/**
	* PROMETHEE Parameters for Heuristic Area.
	*/
	R::RParamStruct* HeurArea;

	/**
	* PROMETHEE Parameters for Selection Distance.
	*/
	R::RParamStruct* SelectDist;

	/**
	* PROMETHEE Parameters for Selection Weight.
	*/
	R::RParamStruct* SelectWeight;

	/**
	* Heuristic to used for the GA.
	*/
	R::RString Heuristic;

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

	/**
	 * Should the distances computed be weighted?.
	 */
	bool WeightedDistances;

public:

	/**
	* Construct the application.
	* @param name            Name of the application.
	* @param argc            Number of arguments.
	* @param argv            Arguments.
	*/
	RVLSIApp(const R::RString& name,int argc, char *argv[]);

	/**
	* Get a pointer over the configuration of GALILEI.
	*/
	R::RConfig* GetVLSIConfig(void) {return(&VLSIConfig);}

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
	* @param info            XML-based information associated to the problem.
	* @param tag             Tag representing the information (Null supposes that
	*                        no information is provided).
	*/
	R::RProblem2D* CreateSession(const R::RString& file,R::RString& info,const R::RString& tag=R::RString::Null);

	/**
	 * Get the session.
	 */
	R::RProblem2D* GetSession(void) const {return(Session);}

	/**
	* Delete the session.
	*/
	void DeleteSession(void);

	/**
	* Get the name of the log file.
	*/
	R::RString GetLogFileName(void) const {return(LogFileName);}

	/**
	* Set the name of the log file.
	* @param name            Name.
	*/
	void SetLogFileName(const R::RString& name) {LogFileName=name;}

	/**
	* @return the name of the debug file.
	*/
	R::RString GetDebugFileName(void) const {return(DebugFileName);}

	/**
	* Set the name of the debug file.
	* @param name            Name.
	*/
	void SetDebugFileName(const R::RString& name) {DebugFileName=name;}

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


//-----------------------------------------------------------------------------
// Global variables
/**
* Pointer to the current application running a VLSI session.
*/
extern RVLSIApp* VLSIApp;


}  //-------- End of namespace RVLSI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
