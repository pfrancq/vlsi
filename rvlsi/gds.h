/*

	R Project Library

	GDS.h

	GDS file - Header.

	Copyright 1999-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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



//------------------------------------------------------------------------------
#ifndef GDS_H
#define GDS_H


//------------------------------------------------------------------------------
// include files for ANSI
#ifdef unix
	#include <unistd.h>
#endif


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
#include <rvlsi/files.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward declarations
class RGDSRecord;
class RGDSFile;


//------------------------------------------------------------------------------
/**
* @short A GDS Record.
*/
class RGDSRecord
{
public:
	int Len;
	unsigned char RecType;
	unsigned char DataType;
	unsigned char* Data;

	RGDSRecord(RGDSFile*,unsigned char *(&Buffer),unsigned &BufferLen);
	int Compare(RGDSRecord* rec) { return(0); }
	inline int GetTypeLen(void)
	{
		switch(DataType)
		{
			case 2: return(2);
			case 3:
			case 4: return(4);
			case 5: return(8);
			case 6: return(Len-4);
		}
		return(0);
	}
	inline int GetMaxIndex(void)
	{
		int i=GetTypeLen();

		if(i)
			return((Len-4)/i);
		else
			return(0);
	}
	int GetType2(int idx);
	int GetType3(int idx);
	double GetType4(int idx);
	double GetType5(int idx);
	char* GetType6(void);
	char* GetRecType(void);
	char* GetRecDesc(void);
	~RGDSRecord(void);
};



//------------------------------------------------------------------------------
/**
* @short: A GDSII data file.
*/
class RGDSFile : public RDataFile, public RContainer<RGDSRecord,unsigned int,true,false>
{
private:
	RCell* CurrCell;
	RLibrary *CurrLib;
public:
	char* LibName;
	double Units;

	RGDSFile(const RString& name);
	virtual const char* StringType(void) {return("=GDSII");}
	virtual const char* TreeType(void) {return(" - GDSII File");}
	virtual bool Analyse(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
