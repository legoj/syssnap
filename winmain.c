

#include "global.h"

char *str_prgname="SysSnap"; //tfx ³ÌÐò±êÌâ
char *str_aboutme="SysSnap is based on Regshot code.\n\n\
Search google for RegShot info.\n\n";
LPSTR	REGSHOTINI			="syssnap.ini"; //tfx
LPSTR	REGSHOTLANGUAGEFILE	="language.ini";

extern u_char * lan_menuclearallshots;  // Be careful of extern ref! must be the same when declare them,otherwise pointer would mis-point
extern u_char * lan_menuclearshot1;     //and I can not use sizeof to get real array size in extern ref
extern u_char * lan_menuclearshot2;
extern u_char * lan_about;
extern LPSTR str_DefaultLanguage;
extern LPSTR str_Original;
extern BOOL bINCLFILEVER;



// this new function Added by Youri in 1.8.2, for expand path in browse dialog
int CALLBACK SelectBrowseFolder(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
  if (uMsg == BFFM_INITIALIZED)
  {
    SendMessage(hWnd, BFFM_SETSELECTION, 1, lpData);
  }
  return 0;
}

//--------------------------------------------------
//Main Dialog Proc
//--------------------------------------------------
BOOL	CALLBACK	DialogProc(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{					    
	UINT	nLengthofStr;


	switch(message)
	{
	case	WM_INITDIALOG:
			


			SendDlgItemMessage(hDlg,IDC_EDITCOMMENT,EM_SETLIMITTEXT,(WPARAM)COMMENTLENGTH,(LPARAM)0);
			SendDlgItemMessage(hDlg,IDC_EDITPATH,EM_SETLIMITTEXT,(WPARAM)MAX_PATH,(LPARAM)0);
			SendDlgItemMessage(hDlg,IDC_EDITDIR,EM_SETLIMITTEXT,(WPARAM)EXTDIRLEN,(LPARAM)0);
			lpExtDir=MYALLOC0(EXTDIRLEN+2);
			lpOutputpath=MYALLOC0(MAX_PATH+5);
			lpCurrentLanguage=MYALLOC0(SIZEOF_SINGLE_LANGUAGENAME);
			lpKeyName=MYALLOC0(MAX_PATH+1);
			lpMESSAGE=MYALLOC0(128);
			lpStartDir=MYALLOC0(MAX_PATH+1);
			lpWindowsDirName=MYALLOC0(MAX_PATH+5);
			lpTempPath=MYALLOC0(MAX_PATH+2);
			lpComputerName1=MYALLOC0(COMPUTERNAMELEN);
			lpComputerName2=MYALLOC0(COMPUTERNAMELEN);
			lpUserName1=MYALLOC0(COMPUTERNAMELEN);
			lpUserName2=MYALLOC0(COMPUTERNAMELEN);
			lpSystemtime1=MYALLOC0(sizeof(SYSTEMTIME));
			lpSystemtime2=MYALLOC0(sizeof(SYSTEMTIME));
			
			lpCurrentTranslator=str_Original;
			nLengthofStr=0;
			GetWindowsDirectory(lpWindowsDirName,MAX_PATH);
			
			//JGS: set the default dir path to system drive
			lpSysDrive=MYALLOC0(4);
			strncpy(lpSysDrive, lpWindowsDirName, 3);
			lpWindowsDirName=lpSysDrive;


			nLengthofStr=strlen(lpWindowsDirName);
			if (nLengthofStr>0&&*(lpWindowsDirName+nLengthofStr-1)=='\\')
				*(lpWindowsDirName+nLengthofStr-1)=0x00;
			
			//JGS: change output dir to current directory see below
			//GetTempPath(MAX_PATH,lpTempPath);

			
			//_asm int 3;
			GetCurrentDirectory(MAX_PATH,lpStartDir); //fixed at 1.8.2 former version use getcommandline()

			lpTempPath = lpStartDir;

			lpIni=MYALLOC0(MAX_PATH*2);
			strcpy(lpIni,lpStartDir);
			if (*(lpIni+strlen(lpIni)-1)!='\\') // 1.8.2
				strcat(lpIni,"\\");
			strcat(lpIni,REGSHOTLANGUAGEFILE);

			lpFreeStrings=MYALLOC(SIZEOF_FREESTRINGS);
			ldwTempStrings=MYALLOC0(4*60); //max is 60 strings

			//if(GetLanguageType(hDlg))
			//	GetLanguageStrings(hDlg);
			//else
				GetDefaultStrings();
			
			SendMessage(hDlg,WM_COMMAND,(WPARAM)IDC_CHECKDIR,(LPARAM)0);

			lpLastSaveDir=lpOutputpath;
			lpLastOpenDir=lpOutputpath;
			
			lpRegshotIni=MYALLOC0(3*MAX_PATH);
			strcpy(lpRegshotIni,lpStartDir);
			if (*(lpRegshotIni+strlen(lpRegshotIni)-1)!='\\')
				strcat(lpRegshotIni,"\\");
			strcat(lpRegshotIni,REGSHOTINI);

			GetSnapRegs(hDlg); //tfx
			
			return TRUE;
			
	case	WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case	IDC_1STSHOT:
					/*CreateShotPopupMenu();
					is1=TRUE;
					GetWindowRect(GetDlgItem(hDlg,IDC_1STSHOT),&rect);
					TrackPopupMenu(hMenu,TPM_LEFTALIGN|TPM_LEFTBUTTON,rect.left+10,rect.top+10,0,hDlg,NULL);
					DestroyMenu(hMenu);
					*/
					SnapAction(hDlg,TRUE, IDC_1STSHOT);
					return(TRUE);
			case	IDC_2NDSHOT:
					/*CreateShotPopupMenu();
					is1=FALSE;
					GetWindowRect(GetDlgItem(hDlg,IDC_2NDSHOT),&rect);
					TrackPopupMenu(hMenu,TPM_LEFTALIGN|TPM_LEFTBUTTON,rect.left+10,rect.top+10,0,hDlg,NULL);
					DestroyMenu(hMenu);
					*/
					SnapAction(hDlg, FALSE, IDC_2NDSHOT);
					return(TRUE);

			case	IDM_SHOTONLY:
					if(is1)
					{
						is1LoadFromHive=FALSE;
						Shot1();
					}
					else
					{
						is2LoadFromHive=FALSE;
						Shot2();
					}

					return(TRUE);
			case	IDM_SHOTSAVE:
					if(is1)
					{
						is1LoadFromHive=FALSE;
						Shot1();
						SaveHive(lpHeadLocalMachine1,lpHeadUsers1,lpHeadFile1,lpComputerName1,lpUserName1,lpSystemtime1); //I might use a struct in future!
					}
					else
					{
						is2LoadFromHive=FALSE;
						Shot2();
						SaveHive(lpHeadLocalMachine2,lpHeadUsers2,lpHeadFile2,lpComputerName2,lpUserName2,lpSystemtime2);
					}

					return(TRUE);

			case	IDM_LOAD:
					if(is1)
						is1LoadFromHive=LoadHive(&lpHeadLocalMachine1,&lpHeadUsers1,&lpHeadFile1,&lpTempHive1);
					else
						is2LoadFromHive=LoadHive(&lpHeadLocalMachine2,&lpHeadUsers2,&lpHeadFile2,&lpTempHive2);

					//if(is1LoadFromHive||is2LoadFromHive)
					//	SendMessage(GetDlgItem(hWnd,IDC_CHECKDIR),BM_SETCHECK,(WPARAM)0x00,(LPARAM)0);

					return(TRUE);
					
			//case	IDC_SAVEREG:
					//SaveRegistry(lpHeadLocalMachine1,lpHeadUsers1);
			//		return(TRUE);
			
			case	IDC_COMPARE:
					CreateComparePopupMenu();
					GetWindowRect(GetDlgItem(hDlg,IDC_COMPARE),&rect);
					TrackPopupMenu(hMenu,TPM_LEFTALIGN|TPM_LEFTBUTTON,rect.left+10,rect.top+10,0,hDlg,NULL);
					DestroyMenu(hMenu);

					return(TRUE);
			
			//JGS: case IDM_CMPTOTEXT and IDM_CMPTOHTML just differs in format in CompareShots
			case	IDM_CMPTOTEXT:
					CompareAction(hDlg,FALSE);

					/*EnableWindow(GetDlgItem(hDlg,IDC_COMPARE),FALSE);
					UI_BeforeClear();
					CompareShots(FALSE);
					ShowWindow(GetDlgItem(hDlg,IDC_PBCOMPARE),SW_HIDE);
					EnableWindow(GetDlgItem(hDlg,IDC_CLEAR1),TRUE);
					SetFocus(GetDlgItem(hDlg,IDC_CLEAR1));
					SendMessage(hDlg,DM_SETDEFID,(WPARAM)IDC_CLEAR1,(LPARAM)0);
					SetCursor(hSaveCursor);					
					MessageBeep(0xffffffff);
					*/
					return(TRUE);

			case	IDM_CMPTOHTML:
					CompareAction(hDlg,TRUE);
					return(TRUE);

			case	IDC_CLEAR1:
					hMenuClear=CreatePopupMenu();
					AppendMenu(hMenuClear,MF_STRING,IDM_CLEARALLSHOTS,lan_menuclearallshots);
					AppendMenu(hMenuClear,MF_MENUBARBREAK,IDM_BREAK,NULL);
					AppendMenu(hMenuClear,MF_STRING,IDM_CLEARSHOT1,lan_menuclearshot1);
					AppendMenu(hMenuClear,MF_STRING,IDM_CLEARSHOT2,lan_menuclearshot2);
					//AppendMenu(hMenuClear,MF_STRING,IDM_CLEARRESULT,"Clear compare result");
					SetMenuDefaultItem(hMenuClear,IDM_CLEARALLSHOTS,FALSE);
			
					

						if(lpHeadLocalMachine1!=NULL)
							EnableMenuItem(hMenuClear,IDM_CLEARSHOT1,MF_BYCOMMAND|MF_ENABLED);
						else
							EnableMenuItem(hMenuClear,IDM_CLEARSHOT1,MF_BYCOMMAND|MF_GRAYED);

						if(lpHeadLocalMachine2!=NULL)
							EnableMenuItem(hMenuClear,IDM_CLEARSHOT2,MF_BYCOMMAND|MF_ENABLED);
						else
							EnableMenuItem(hMenuClear,IDM_CLEARSHOT2,MF_BYCOMMAND|MF_GRAYED);

					GetWindowRect(GetDlgItem(hDlg,IDC_CLEAR1),&rect);
					TrackPopupMenu(hMenuClear,TPM_LEFTALIGN|TPM_LEFTBUTTON,rect.left+10,rect.top+10,0,hDlg,NULL);
					DestroyMenu(hMenuClear);
					return(TRUE);
			case	IDM_CLEARALLSHOTS:
					UI_BeforeClear();
					FreeAllKeyContent1();  //Note!! if loadfromhive and contains file,we should let lpHeadFile to NULL
					FreeAllKeyContent2();
					FreeAllCompareResults();
					
					FreeAllFileHead(lpHeadFile1);
					FreeAllFileHead(lpHeadFile2);

					lpHeadFile1=NULL;
					lpHeadFile2=NULL;
					UI_AfterClear();
					EnableWindow(GetDlgItem(hWnd,IDC_CLEAR1),FALSE);
					return(TRUE);
			case	IDM_CLEARSHOT1:
					UI_BeforeClear();
					FreeAllKeyContent1();
					FreeAllCompareResults();
					FreeAllFileHead(lpHeadFile1);
					lpHeadFile1=NULL;
					ClearKeyMatchTag(lpHeadLocalMachine2); //we clear shot2's tag
					ClearKeyMatchTag(lpHeadUsers2);
					ClearHeadFileMatchTag(lpHeadFile2);
					UI_AfterClear();
					return(TRUE);
			case	IDM_CLEARSHOT2:
					UI_BeforeClear();
					FreeAllKeyContent2();
					FreeAllCompareResults();
					FreeAllFileHead(lpHeadFile2);
					lpHeadFile2=NULL;
					ClearKeyMatchTag(lpHeadLocalMachine1); //we clear shot1's tag
					ClearKeyMatchTag(lpHeadUsers1);
					ClearHeadFileMatchTag(lpHeadFile1);
					UI_AfterClear();
					return(TRUE);


			case	IDC_CHECKDIR:
					if(SendMessage(GetDlgItem(hDlg,IDC_CHECKDIR),BM_GETCHECK,(WPARAM)0,(LPARAM)0)==1)
					{
						EnableWindow(GetDlgItem(hDlg,IDC_EDITDIR),TRUE);
						EnableWindow(GetDlgItem(hDlg,IDC_BROWSE1),TRUE);
						EnableWindow(GetDlgItem(hDlg,IDC_FILEVERSION),TRUE);
					}
					else
					{
						EnableWindow(GetDlgItem(hDlg,IDC_EDITDIR),FALSE);
						EnableWindow(GetDlgItem(hDlg,IDC_BROWSE1),FALSE);
						EnableWindow(GetDlgItem(hDlg,IDC_FILEVERSION),FALSE);
					}
					return(TRUE);
			case	IDC_CANCEL1:
			case	IDCANCEL:

					SetSnapRegs(hDlg);//tfx
					PostQuitMessage(0);
					return(TRUE);

			case	IDC_BROWSE1:
				{

					LPITEMIDLIST lpidlist;
					DWORD	nWholeLen;
					BrowseInfo1.hwndOwner=hDlg;
					BrowseInfo1.pszDisplayName=MYALLOC0(MAX_PATH+1);
					//BrowseInfo1.lpszTitle="Select:";
					BrowseInfo1.ulFlags =0; //3lines added in 1.8.2
					BrowseInfo1.lpfn = NULL;
					BrowseInfo1.lParam = 0;

					lpidlist=SHBrowseForFolder(&BrowseInfo1);
					if (lpidlist!=NULL)
					{
						SHGetPathFromIDList(lpidlist,BrowseInfo1.pszDisplayName);
						nLengthofStr = GetDlgItemText(hDlg,IDC_EDITDIR,lpExtDir,EXTDIRLEN+2);
						nWholeLen=nLengthofStr+strlen(BrowseInfo1.pszDisplayName);
						
						if (nWholeLen<EXTDIRLEN+1)
						{
							strcat(lpExtDir,";");
							strcat(lpExtDir,BrowseInfo1.pszDisplayName);

						}
						else
							strcpy(lpExtDir,BrowseInfo1.pszDisplayName);
						
						SetDlgItemText(hDlg,IDC_EDITDIR,lpExtDir);
						MYFREE(lpidlist);
					}
					
					MYFREE(BrowseInfo1.pszDisplayName);
				}
				return(TRUE);
					
			case	IDC_BROWSE2:
				{

					LPITEMIDLIST lpidlist;
					BrowseInfo1.hwndOwner=hDlg;
					BrowseInfo1.pszDisplayName=MYALLOC0(MAX_PATH+1);
					//BrowseInfo1.lpszTitle="Select:";

					//-----------------
					// Added by Youri in 1.8.2 ,Thanks!
					// if you add this code, the browse dialog will be expand path and have button "Create Folder"
					BrowseInfo1.ulFlags |= 0x0040; //BIF_NEWDIALOGSTYLE;    // button "Create Folder" and resizable
					BrowseInfo1.lpfn = SelectBrowseFolder;        // function for expand path
					BrowseInfo1.lParam = (LPARAM)BrowseInfo1.pszDisplayName;
					// Initilize selection path
					GetDlgItemText(hDlg,IDC_EDITPATH,BrowseInfo1.pszDisplayName,MAX_PATH);
					//-----------------

					lpidlist=SHBrowseForFolder(&BrowseInfo1);
					if (lpidlist!=NULL)
					{
						SHGetPathFromIDList(lpidlist,BrowseInfo1.pszDisplayName);
						SetDlgItemText(hDlg,IDC_EDITPATH,BrowseInfo1.pszDisplayName);
						MYFREE(lpidlist);
					}
					
					MYFREE(BrowseInfo1.pszDisplayName);
				}
				return(TRUE);
			case IDC_FILEVERSION:
				bINCLFILEVER = SendMessage(GetDlgItem(hDlg,IDC_FILEVERSION),BM_GETCHECK,(WPARAM)0,(LPARAM)0)==1;
				return(TRUE); 
			//case	IDC_COMBOLANGUAGE:
			//		GetLanguageStrings(hDlg);
			//		return(TRUE);
					
			/*case	IDC_ABOUT:
					{	
					LPSTR	lpAboutBox;
					//_asm int 3;
					lpAboutBox=MYALLOC0(SIZEOF_ABOUTBOX);
					//it is silly that when wsprintf encounter a NULL strings, it will write the whole string to NULL!
					sprintf(lpAboutBox,"%s%s%s%s%s%s",str_aboutme,"[",(strlen(lpCurrentLanguage)==0)?str_DefaultLanguage:lpCurrentLanguage,"]"," by:",lpCurrentTranslator);
					MessageBox(hDlg,lpAboutBox,lan_about,MB_OK);
					MYFREE(lpAboutBox);
					return(TRUE);
					}
					*/
			}

	}
	return(FALSE);
}



//////////////////////////////////////////////////////////////////
int		PASCAL WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
					   LPSTR lpszCmdLine,INT nCmdShow)
{
		
			
	hHeap=GetProcessHeap(); //1.8.2
	hWnd=CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,(WNDPROC)DialogProc);
	
	//JGS: this doesn't show up the 
	//SetClassLong(hWnd,GCL_HICONSM,(LONG)LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON2)));
    SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON2)));
         

	SetWindowText(hWnd, str_prgname); 
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);		   

	while(GetMessage(&msg,NULL,(WPARAM)NULL,(LPARAM)NULL))
	{
		if(!IsDialogMessage(hWnd,&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return(msg.wParam);
}

VOID SnapAction(HWND hDlg, BOOL isSnap1, unsigned short ctrlID){
	CreateShotPopupMenu();
	is1=isSnap1;
	GetWindowRect(GetDlgItem(hDlg,ctrlID),&rect);
	TrackPopupMenu(hMenu,TPM_LEFTALIGN|TPM_LEFTBUTTON,rect.left+10,rect.top+10,0,hDlg,NULL);
	DestroyMenu(hMenu);
}
VOID CompareAction(HWND hDlg, BOOL isHTML){
	EnableWindow(GetDlgItem(hDlg,IDC_COMPARE),FALSE);
	UI_BeforeClear();
	CompareShots(isHTML);
	ShowWindow(GetDlgItem(hDlg,IDC_PBCOMPARE),SW_HIDE);
	EnableWindow(GetDlgItem(hDlg,IDC_CLEAR1),TRUE);
	SetFocus(GetDlgItem(hDlg,IDC_CLEAR1));
	SendMessage(hDlg,DM_SETDEFID,(WPARAM)IDC_CLEAR1,(LPARAM)0);
	SetCursor(hSaveCursor);					
	MessageBeep(0xffffffff);
}


