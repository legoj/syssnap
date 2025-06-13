/*
    Copyright 1999-2003,2007 TiANWEi

    This file is part of Regshot.

    Regshot is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Regshot is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Regshot; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "global.h"

extern u_char * lan_error;


//-------------------------------------------------------------
// Show error message
//-------------------------------------------------------------
VOID	ErrMsg(char * note)
{
	MessageBox(hWnd,note,lan_error,MB_ICONHAND);
}

//-------------------------------------------------------------
//Routine to debug
//-------------------------------------------------------------
#ifdef  DEBUGLOG
extern char * str_CR;
VOID	DebugLog(LPSTR filename,LPSTR lpstr,HWND hDlg,BOOL bisCR)
{
	DWORD	length;
	DWORD	nPos;
	
	hFile = CreateFile(filename,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if( hFile == INVALID_HANDLE_VALUE) 
		ErrMsg(lan_errorcreatefile);
	else
	{
		nPos=SetFilePointer(hFile,0,NULL,FILE_END);
		if(nPos==0xFFFFFFFF)
			ErrMsg(lan_errormovefp);
		else
		{
		
			length=strlen(lpstr);
			WriteFile(hFile,lpstr,length,&NBW,NULL);
			if(NBW!=length)
			{
				//ErrMsg(lan_errorwritefile);
				
			}
			if(bisCR==TRUE)
			WriteFile(hFile,str_CR,sizeof(str_CR)-1,&NBW,NULL);
		}
	}
	CloseHandle(hFile);
}
#endif


//------------------------------------------------------------
//routine to replace invalid chars in comment fields
//------------------------------------------------------------
BOOL ReplaceInValidFileName(LPSTR lpf)
{
	char lpInvalid[]="\\/:*?\"<>|"; //1.8.2
	DWORD	i,j,nLen;
	BOOL	bLegal=FALSE;
	nLen=strlen(lpf);
	for(i=0;i<nLen;i++)
	{
		for(j=0;j<sizeof(lpInvalid)-1;j++) //changed at 1.8.2 from 9 to sizeof()-1
		{
			if (*(lpf+i)==*(lpInvalid+j))
				*(lpf+i)='-'; //0x2D; check for invalid chars and replace it (return FALSE;)
			else
				if(*(lpf+i)!=0x20&&*(lpf+i)!=0x09) //At least one non-space,non-tab char needed!
					bLegal=TRUE;

		}
	}
	return bLegal;
}

//--------------------------------------------------
// Find lp's position in lpMaster (like At(),but not limit to str)
//--------------------------------------------------
LPSTR	AtPos(LPSTR lpMaster,LPSTR lp,DWORD size)
{
	DWORD	i,j,nsizelp;
	nsizelp=strlen(lp);
	if (size<=nsizelp||nsizelp<1)
		return NULL;
	
	for(i=0;i<size-nsizelp;i++)
	{
		for(j=0;j<nsizelp;j++)
		{
			if(*(lp+j)!=*(lpMaster+i+j))
				break;
		}
		//_asm int 3;
		if(j==nsizelp)
			return lpMaster+i+nsizelp;
	}
	return NULL;
		
}

				

