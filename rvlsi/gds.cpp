/*

	R Project Library

	GDS.cpp

	A GDS File - Implementation.

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
#include <rvlsi/gds.h>
using namespace R;



//------------------------------------------------------------------------------
//
// Class RGDSRecord
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGDSRecord::RGDSRecord(RGDSFile*,unsigned char *(&Buffer),unsigned& BufferLen)
{
	unsigned char tmp;

	tmp=(*(Buffer++));
	Len=(tmp<<8)+(*(Buffer++));
	if(Len<4)
	{
		BufferLen=0;
		Len=0;
		RecType=0;
		DataType=0;
		Data=0;
	}
	else
	{
		RecType=(*(Buffer++));
		DataType=(*(Buffer++));
		BufferLen-=Len;
		if(Len>4)
		{
			Data=new unsigned char[Len-4];
			memcpy(Data,Buffer,sizeof(unsigned char)*(Len-4));
			Buffer+=(Len-4);
		}
		else
			Data=0;
	}
}


//------------------------------------------------------------------------------
int RGDSRecord::GetType2(int idx)
{
	unsigned char *Buffer=&Data[idx*2];
	int nb;
	unsigned int nb2;

	if(Buffer[0]>128)
	{
		nb2=Buffer[0];
		nb2<<=8;
		nb2+=Buffer[1];
		nb2=~nb2;
		#ifdef __BORLANDC__
	    	#pragma warn -ngu
		#endif
		nb=-(nb2+1);
		#ifdef __BORLANDC__
			#pragma warn .ngu
		#endif
	}
	else
	{
		nb=Buffer[0];
		nb<<=8;
		nb+=Buffer[1];
	}
	return(nb);
}


//------------------------------------------------------------------------------
int RGDSRecord::GetType3(int idx)
{
	unsigned char *Buffer=&Data[idx*4];
	long nb,tmp;
	unsigned long nb2,tmp2;

	if(Buffer[0]>128)
	{
		nb2=Buffer[0];
		nb2<<=24;
		tmp2=Buffer[1];
		tmp2<<=16;
		nb2+=tmp2;
		tmp2=Buffer[2];
		tmp2<<=8;
		nb2+=tmp2+Buffer[3];
		nb2=~nb2;
		#ifdef __BORLANDC__
			#pragma warn -ngu
		#endif
		nb=-(nb2+1);
		#ifdef __BORLANDC__
			#pragma warn .ngu
		#endif
	}
	else
	{
		nb=Buffer[0];
		nb<<=24;
		tmp=Buffer[1];
		tmp<<=16;
		nb+=tmp;
		tmp=Buffer[2];
		tmp<<=8;
		nb+=tmp+Buffer[3];
	}
	return(nb);
}


//------------------------------------------------------------------------------
double RGDSRecord::GetType4(int idx)
{
	double Mult=2,Sum=0,Exp16;
	int Act,Sign,i,j;
	unsigned char *Buffer=&Data[idx*4];

	Act=(*(Buffer++));

	// Read Sign
	if(Act>128)
	{
		Sign=-1;
		Act-=128;
	}
	else
		Sign=1;

	// Read Exponent
	Exp16=pow(static_cast<double>(16),static_cast<double>(Act-64));

	// Read Mantisse
	i=0;
	for(i=0;i<4;i++)
	{
		Act=(*(Buffer++));
		j=128;
		while(j>0)
		{
			if((Act&j)==j)
				Sum+=Exp16/Mult;
			Mult*=2;
			j/=2;
		}
	}
//	BufferLen-=4;
	return(Sum*Sign);
}


//------------------------------------------------------------------------------
double RGDSRecord::GetType5(int idx)
{
	double Mult=2,Sum=0,Exp16;
	int Act,Sign,i,j;
	unsigned char *Buffer=&Data[idx*8];

	Act=(*(Buffer++));

	// Read Sign
	if(Act>128)
	{
		Sign=-1;
		Act-=128;
    }
	else
	Sign=1;

	// Read Exponent
	Exp16=pow(static_cast<double>(16),static_cast<double>(Act-64));

	// Read Mantisse
	i=0;
	for(i=0;i<7;i++)
	{
		Act=(*(Buffer++));
		j=128;
		while(j>0)
		{
			if((Act&j)==j)
				Sum+=Exp16/Mult;
			Mult*=2;
			j/=2;
		}
	}
//	BufferLen-=8;
	return(Sum*Sign);
}


//------------------------------------------------------------------------------
char* RGDSRecord::GetType6(void)
{
	static char Tmp[500];

	memcpy(Tmp,Data,Len-4);
	Tmp[Len-4]=0;
	return(Tmp);
}


//------------------------------------------------------------------------------
char* RGDSRecord::GetRecType(void)
{
	static char tmp[200];

	switch(RecType)
	{
		case 0 : strcpy(tmp,"Header"); break;
		case 1 : strcpy(tmp,"Begin Library"); break;
		case 2 : strcpy(tmp,"Library Name"); break;
		case 3 : strcpy(tmp,"Units"); break;
		case 4 : strcpy(tmp,"End Library"); break;
		case 5 : strcpy(tmp,"Begin Structure"); break;
		case 6 : strcpy(tmp,"Structure Name"); break;
		case 7 : strcpy(tmp,"End Structure"); break;
		case 8 : strcpy(tmp,"Boundary Element"); break;
		case 9 : strcpy(tmp,"Path Element"); break;
		case 10: strcpy(tmp,"Structure Reference Element"); break;
		case 11: strcpy(tmp,"Array Reference Element"); break;
		case 12: strcpy(tmp,"Text Element"); break;
		case 13: strcpy(tmp,"Layer"); break;
		case 14: strcpy(tmp,"Data Type"); break;
		case 15: strcpy(tmp,"Width"); break;
		case 16: strcpy(tmp,"Array of XY"); break;
		case 17: strcpy(tmp,"End Element"); break;
		case 18: strcpy(tmp,"Name of Referenced Structure"); break;
		case 19: strcpy(tmp,"Nb Columns & Rows"); break;
		case 21: strcpy(tmp,"Begin Node"); break;
		case 22: strcpy(tmp,"Text Type"); break;
		case 23: strcpy(tmp,"Presentation"); break;
		case 25: strcpy(tmp,"String"); break;
		case 26: strcpy(tmp,"Transformation"); break;
		case 27: strcpy(tmp,"Magnification Factor"); break;
		case 28: strcpy(tmp,"Angle"); break;
		case 31: strcpy(tmp,"Name of reference Library"); break;
		case 32: strcpy(tmp,"Fonts"); break;
		case 33: strcpy(tmp,"Path Type"); break;
		case 34: strcpy(tmp,"Generations"); break;
		case 35: strcpy(tmp,"Name of Attribute Definition File"); break;
		case 38: strcpy(tmp,"EL Flags"); break;
		case 42: strcpy(tmp,"Node Type"); break;
		case 43: strcpy(tmp,"Attribute Number"); break;
		case 44: strcpy(tmp,"Attribute Value"); break;
		case 45: strcpy(tmp,"Begin Box"); break;
		case 46: strcpy(tmp,"Box Type"); break;
		case 47: strcpy(tmp,"Plex"); break;
		case 50: strcpy(tmp,"Tape Number"); break;
		case 51: strcpy(tmp,"Tape Code"); break;
		case 54: strcpy(tmp,"Format of Stream"); break;
		case 55: strcpy(tmp,"Mask"); break;
		case 56: strcpy(tmp,"End Mask"); break;
		default: strcpy(tmp,"Unknow"); break;
	}
	return(tmp);
}


//------------------------------------------------------------------------------
char* RGDSRecord::GetRecDesc(void)
{
	static char tmp[200];
	char tmp2[30];
	unsigned char c,c2;
	int i,j;
	int tmplong;
	bool Ok;

	switch(RecType)
	{
		case 0:
			strcpy(tmp,"Version ");
			i=(Data[0]<<8)+Data[1];
			switch(i)
			{
				case 0: strcat(tmp,"Prior to 3.0"); break;
				case 3: strcat(tmp,"3.0"); break;
				case 4: strcat(tmp,"4.0"); break;
				case 5: strcat(tmp,"5.0"); break;
				case 600: strcat(tmp,"6.0"); break;
				default: strcat(tmp,"?"); break;
			}
			break;

		case 1:
			sprintf(tmp,"Last Modify: %d/%d/%d %d:%d:%d  -  Last Access: %d/%d/%d %d:%d:%d",
			        GetType2(2),GetType2(1),GetType2(0),GetType2(3),GetType2(4),GetType2(5),
			        GetType2(8),GetType2(7),GetType2(6),GetType2(9),GetType2(10),GetType2(11));
			break;

		case 2:
			strcpy(tmp,GetType6());
			break;

		case 3:
			sprintf(tmp,"Db Unit in user units: %g  -  Db Unit in meter: %g",GetType5(0),GetType5(1));
			break;

		case 5:
			sprintf(tmp,"Last Modify: %d/%d/%d %d:%d:%d  -  Last Access: %d/%d/%d %d:%d:%d",
			        GetType2(2),GetType2(1),GetType2(0),GetType2(3),GetType2(4),GetType2(5),
			        GetType2(8),GetType2(7),GetType2(6),GetType2(9),GetType2(10),GetType2(11));
			break;

		case 6:
			strcpy(tmp,GetType6());
			break;

		case 13:
			sprintf(tmp,"Layer Number: %d",GetType2(0));
			break;

		case 14:
			sprintf(tmp,"Data Number: %d",GetType2(0));
			break;

		case 15:
			tmplong=GetType3(0);
			sprintf(tmp,"%d Db Units",abs(tmplong));
			if(tmplong<0) strcat(tmp," (Absolute)");
			break;

		case 16:
			i=0;
			j=GetMaxIndex();
			strcpy(tmp,"");
			while(i<j)
			{
				sprintf(tmp2,"Pt (%d,%d)",GetType3(i),GetType3(i+1));
				i+=2;
				strcat(tmp,tmp2);
				if(i<j) strcat(tmp,"  -  ");
			}
			break;

		case 18:
			strcpy(tmp,GetType6());
			break;

		case 19:
			sprintf(tmp,"%d Columns and %d rows",GetType2(0),GetType2(1));
			break;

		case 22:
			sprintf(tmp,"Type: %d",GetType2(0));
			break;

		case 23:
			c=Data[1];
			#ifdef __BORLANDC__
				#pragma warn -sig
			#endif
			c2=(c&48)>>4;
			sprintf(tmp,"Font %d  -  Vertical: ",c2);
			c2=(c&12)>>2;
			#ifdef __BORLANDC__
				#pragma warn .sig
			#endif
			switch(c2)
			{
				case 0: strcat(tmp,"Top"); break;
				case 1: strcat(tmp,"Middle"); break;
				case 2: strcat(tmp,"Bottom"); break;
			}
			strcat(tmp,"  -  Horizontal: ");
			#ifdef __BORLANDC__
				#pragma warn -sig
			#endif
			c2=(c&3);
			#ifdef __BORLANDC__
				#pragma warn .sig
			#endif
			switch(c2)
			{
				case 0: strcat(tmp,"Left"); break;
				case 1: strcat(tmp,"Center"); break;
				case 2: strcat(tmp,"Right"); break;
			}
			break;

		case 25:
			strcpy(tmp,GetType6());
			break;

		case 26:
			Ok=false;
			c=Data[0];
			strcpy(tmp,"");
			if(c==128)
			{
				strcat(tmp,"Reflection");
				Ok=true;
			}
			c=Data[1];
			#ifdef __BORLANDC__
				#pragma warn -sig
			#endif
			c2=(c&4);
			#ifdef __BORLANDC__
				#pragma warn .sig
			#endif
			if(c2)
			{
				if(Ok) strcat(tmp,"  -  ");
				strcat(tmp,"Absolute Magnification");
				Ok=true;
			}
			#ifdef __BORLANDC__
				#pragma warn -sig
			#endif
			c2=(c&2);
			#ifdef __BORLANDC__
				#pragma warn .sig
			#endif
			if(c2)
			{
				if(Ok) strcat(tmp,"  -  ");
				strcat(tmp,"Absolute Angle");
				Ok=true;
			}
			if(!Ok) strcat(tmp,"No Transformation");
			break;

		case 27:
			sprintf(tmp,"%g",GetType5(0));
			break;

		case 28:
			sprintf(tmp,"%g°",GetType5(0));
			break;

		default:
			strcpy(tmp,"");
			break;
	}
	return(tmp);
}


//------------------------------------------------------------------------------
RGDSRecord::~RGDSRecord(void)
{
	if(Data)
		delete[] Data;
}



//------------------------------------------------------------------------------
//
// Class RGDSFile
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
RGDSFile::RGDSFile(const RString& name) : RDataFile(name),RContainer<RGDSRecord,unsigned int,true,false>(100,50)
{
/*  unsigned char *File,*Buffer;
  unsigned int BufferLen;
  struct stat statbuf;
  int theHandle;
  RGDSRecord *Rec;

  Type=cstGDSII;
  theHandle=open(name(),O_RDONLY);
  fstat(theHandle, &statbuf);
  BufferLen=statbuf.st_size;
  Buffer=File=new unsigned char[BufferLen];
  read(theHandle,File,BufferLen);
  close(theHandle);
  while(BufferLen)
  {
    Rec=new RGDSRecord(this,Buffer,BufferLen);
    if(Rec->Len)
      InsertPtr(Rec);
    else
      delete Rec;
  }
  delete[] File;*/
}

//---------------------------------------------------------------------------
bool RGDSFile::Analyse(void)
{
	RGDSRecord **Rec;
	static char Tmp[200];
	char *ptr;
	unsigned int i,NbPts,j;
	RCoord x,y;
	RPolygon *Poly=0;

	CurrCell=0;
	CurrLib=0;
	for(i=NbPtr+1,Rec=Tab;--i;Rec++)
	{
		switch((*Rec)->RecType)
		{
			case 2:
				// Library Name
				LibName=(*Rec)->GetType6();
				ptr=strchr(LibName,'.');
				if(ptr) (*ptr)=0;
				CurrLib=Proj->Libraries->InsertLib(LibName);
				break;

			case 3:
				// Units
				Units=(*Rec)->GetType5(0)/(*Rec)->GetType5(1);
				break;

			case 4:
				// End of a Library
				CurrLib=0;
			break;

			case 6:
				// Library Name
				strcpy(Tmp,(*Rec)->GetType6());
				CurrCell=Proj->Cells->GetInsertPtr<RString>(Tmp);
				break;

			case 7:
				// End of a Cell
				CurrCell=0;
				break;

			case 8:
				// Begin Boundary
				Poly=new RPolygon();
				break;

			case 16:
				// A XY structure
				NbPts=(*Rec)->GetMaxIndex()/2;
				if(Poly&&CurrCell)
				{
					j=0;
					while(j<NbPts)
					{
						x=(*Rec)->GetType3(j*2);
						y=(*Rec)->GetType3(j*2+1);
						j++;
						Poly->InsertPtr(new RPoint(x,y));
					}
				}
				break;

			case 17:
				// End Element
				if(Poly)
				{
					if(CurrCell) CurrCell->Polygons.InsertPtr(Poly);
					Poly=0;
				}
				break;
		}
 	}
	return(true);
}
