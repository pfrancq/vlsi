/*

  Edif.cpp

  EDIF file - Implementation.

  (C) 1999-2000 by P. Francq.

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
#include "edif.h"
using namespace RVLSI;



//---------------------------------------------------------------------------
//
// General Variables
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// General Functions
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
inline bool MustIncBuffer(char c)
{
  if(c==' ') return(true);         // Space
  if(c=='\n') return(true);        // Line Feed
  if(c=='\t') return(true);        // Tab
  if(c=='\r') return(true);        // Carriage Return
  return(false);
}



//---------------------------------------------------------------------------
//
// Class "REDIFTag"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
REDIFTag::REDIFTag(unsigned int id,REDIFFile *Owner,char *(&Buffer),unsigned &BufferLen)
  : RBaseNode<REDIFTag>(id,20,10)
{
  static char Temp[200];
  char *ptr=Temp;
  REDIFTag *CurrentSub;
  bool Parent=true;

  Buffer++;   // Past '('
  while((!Parent)||(Parent&&((*Buffer)!=')')&&((*Buffer)!='('))&&((*Buffer)!=' ')&&((*Buffer)!='\n')&&((*Buffer)!='\t')&&((*Buffer)!='\r'))
  {
    if((*Buffer)=='\"')
    {
      if(Parent) Parent=false; else Parent=true;
    }
    (*(ptr++))=(*(Buffer++));
  }
  (*(ptr--))=0;
  TagName=Temp;
//  TagName.StrUpr(Temp);
  while((*ptr)==' ') (*(ptr--))=0;
  TypeName.StrUpr(Temp);
  // Point to the next entity
  while(MustIncBuffer(*Buffer)) Buffer++;
  if(((*Buffer)!=')')&&((*Buffer)!='('))
  {
    ptr=Temp;
    while((!Parent)||(Parent&&((*Buffer)!=')')&&((*Buffer)!='('))&&((*Buffer)!='\n')&&((*Buffer)!='\t')&&((*Buffer)!='\r'))
    {
      if((*Buffer)=='\"')
      {
        if(Parent) Parent=false; else Parent=true;
      }
      (*(ptr++))=(*(Buffer++));
    }
    (*(ptr--))=0;
    while((*ptr)==' ') (*(ptr--))=0;
    Params=Temp;
//    Params.StrUpr(Temp);
  }
  // Point to the next entity
  while(MustIncBuffer(*Buffer)) Buffer++;
  while((*Buffer)!=')')     // Verify if sub tag
  {
    CurrentSub=new REDIFTag(Owner->Struct->NbPtr,Owner,Buffer,BufferLen);
    Owner->Struct->AddNode(this,CurrentSub);
  }
  Buffer++;   // Read the ')'
  // Point to the next entity
  while(MustIncBuffer(*Buffer)) Buffer++;
}


//---------------------------------------------------------------------------
void REDIFTag::InsertInst(REDIFFile* owner)
{
  RString Name(200),Ref(200);
  REDIFTag *ptr;

  ptr=Sub->GetPtr<char*>("name");
  if(!ptr) return;
  Name=ptr->Params;
  ptr=Sub->GetPtr<char*>("viewRef");
  if(!ptr) return;
  ptr=ptr->Sub->GetPtr<char*>("cellRef");
  if(!ptr) return;
  Ref=ptr->Params;
  owner->CurrCell->InsertInst(Name,Ref);
}


//---------------------------------------------------------------------------
void REDIFTag::InsertPortImp(REDIFFile* owner)
{

}


//---------------------------------------------------------------------------
void REDIFTag::InsertNet(REDIFFile* owner)
{
  RNet *net;
  REDIFTag **ptr,*join,*sub;
  tId i;
  RString PortName(200),InstName(200);

  if(!(owner->CurrCell)) return;
  net=owner->CurrCell->InsertNet(Params);
  join=Sub->GetPtr<char*>("joined");
  if(!join) return;
  for(i=join->Sub->NbPtr+1,ptr=join->Sub->Tab;--i;ptr++)
  {
    if((*ptr)->TagName=="portRef") continue;
    PortName=(*ptr)->Params;
    if((*ptr)->Sub->NbPtr)
    {
      sub=(*((*ptr)->Sub->Tab));
      if(sub->TagName=="instanceRef") continue;
      InstName=sub->Params;
    }
    else
      InstName=owner->CurrCell->Name;
    net->InsertRef(PortName,InstName);
  }
}


//---------------------------------------------------------------------------
bool REDIFTag::Analyse(REDIFFile* owner)
{
  REDIFTag **tag;
  unsigned int i;
  unsigned int TypeId;
  bool bSub=true;
  char dir;

  TypeId=(owner->Types->GetPtr(TypeName))->Id;
  switch(TypeId)
  {
    case TYPECELL:                // This is a cell
      owner->CurrCell=owner->Proj->Cells->InsertCell(Params,owner->CurrLib);
      break;
    case TYPELIBRARY:             // This is a library
      owner->CurrLib=owner->Proj->Libraries->InsertLib(Params);
      break;
    case TYPEINSTANCE:                // This is a instance of the current Cell
      InsertInst(owner);
      owner->CurrCell->Abstract=false;
      bSub=false;
      break;
    case TYPEPORTIMP:
      InsertPortImp(owner);
      bSub=false;
      break;
    case TYPENET:
      InsertNet(owner);
      bSub=false;
      break;
    case TYPEPORT:                // This is a port of the current Cell
      dir=2;
      if((*(Sub->Tab))->Params=="INPUT") dir=-1;
      if((*(Sub->Tab))->Params=="OUTPUT") dir=+1;
      if((*(Sub->Tab))->Params=="INOUT") dir=0;
      owner->CurrCell->InsertPort(Params,dir);
      bSub=false;
      break;
  }
  if(!bSub) return(true);
  for(i=Sub->NbPtr+1,tag=Sub->Tab;--i;tag++)
    (*tag)->Analyse(owner);
  switch(TypeId)
  {
    case TYPECELL:                // This is a cell
      owner->CurrCell=NULL;
      break;
    case TYPELIBRARY:             // This is a library
      owner->CurrLib=NULL;
      break;
  }
  return(true);
}



//---------------------------------------------------------------------------
//
// Class "REDIFFile"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
REDIFFile::REDIFFile(const RString &name) : RDataFile(name), CurrCell(NULL), CurrLib(NULL)
{
/*  char *File,*Buffer;
  unsigned int BufferLen;
  struct stat statbuf;
  int theHandle;
  REDIFTag *Top;
	unsigned int CurrentId=1;
	RStringId *ptr;
	
  Type=cstEDIF2;
  Struct=new RTree<REDIFTag>(150,50);
  Types=new RContainer<RStringId,unsigned int,true,true>(10,5);
  ptr=Types->GetInsertPtr<RString>("CELL");
  ptr->Id=CurrentId++;
  ptr=Types->GetInsertPtr<RString>("LIBRARY");
  ptr->Id=CurrentId++;
  ptr=Types->GetInsertPtr<RString>("INSTANCE");
  ptr->Id=CurrentId++;
  ptr=Types->GetInsertPtr<RString>("PORTIMPLEMENTATION");
  ptr->Id=CurrentId++;
  ptr=Types->GetInsertPtr<RString>("NET");
  ptr->Id=CurrentId++;
  ptr=Types->GetInsertPtr<RString>("PORT");
  ptr->Id=CurrentId++;
  theHandle=open(name(),O_RDONLY);
  fstat(theHandle, &statbuf);
  BufferLen=statbuf.st_size;
  Buffer=File=new char[statbuf.st_size+1];
  read(theHandle,File,statbuf.st_size);
  File[statbuf.st_size]=0;
  close(theHandle);
  Top=new REDIFTag(Struct->NbPtr,this,Buffer,BufferLen);
  Struct->AddNode(NULL,Top);
  delete[] File;*/
}


//---------------------------------------------------------------------------
bool REDIFFile::Analyse(void)
{
  REDIFTag **tag;
  tId i;

  for(i=Struct->Top->NbPtr+1,tag=Struct->Top->Tab;--i;tag++)
    if(!((*tag)->Analyse(this))) return(false);
  return(true);
}


//---------------------------------------------------------------------------
REDIFFile::~REDIFFile(void)
{
  delete Struct;
}




//---------------------------------------------------------------------------
