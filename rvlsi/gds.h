/*

  GGS.h

  A GDS file

  By (c) P. Francq, 1999.

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


//---------------------------------------------------------------------------
#ifndef GDSH
#define GDSH


//---------------------------------------------------------------------------
// Includes
#ifdef unix
	#include <unistd.h>
#endif
#include "files.h"
using namespace RVLSI;


//---------------------------------------------------------------------------
namespace RVLSI{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Forward declarations
class RGDSRecord;
class RGDSFile;



//---------------------------------------------------------------------------
// A GDS Record
class RGDSRecord
{
public:
  int Len;
  unsigned char RecType;
  unsigned char DataType;
  unsigned char *Data;

  RGDSRecord(RGDSFile*,unsigned char *(&Buffer),unsigned &BufferLen);
  int Compare(RGDSRecord *rec) { return(0); }
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
  long GetType3(int idx);
  double GetType4(int idx);
  double GetType5(int idx);
  char* GetType6(void);
  char* GetRecType(void);
  char* GetRecDesc(void);
  ~RGDSRecord(void);
};



//---------------------------------------------------------------------------
// A GDSII data file
class RGDSFile : public RDataFile, public RContainer<RGDSRecord,unsigned,true,false>
{
private:
	RCell *CurrCell;
	RLibrary *CurrLib;
public:
  char *LibName;
  double Units;

  RGDSFile(char* name);
  virtual char* StringType(void) {return("=GDSII");}
  virtual char* TreeType(void) {return(" - GDSII File");}
  virtual bool Analyse(void);
};


}  //-------- End of namespace RVLSI ---------------------------------------


//---------------------------------------------------------------------------
#endif
