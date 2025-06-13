

#include "global.h"


//Some strings use to write to HTML or TEXT file,need [] to use with sizeof()
char str_CR[]="\r\n"; //{0x0d,0x0a,0x00};
char txt_line[]		="\r\n----------------------------------\r\n";
char htm_BR[]		="<BR>";
char htm_HTMLbegin[]="<HTML>\r\n";
char htm_HTMLover[]	="</HTML>";
char htm_HEADbegin[]="<HEAD>\r\n";
char htm_HEADover[]	="</HEAD>\r\n";
char htm_Td1Begin[]	="<TR><TD BGCOLOR=669999 ALIGN=LEFT><FONT COLOR=WHITE><B>";
char htm_Td2Begin[]	="<TR><TD NOWRAP><FONT COLOR=BLACK>";
char htm_Td1Over[]	="</B></FONT></TD></TR>\r\n";
char htm_Td2Over[]	="</FONT></TD></TR>\r\n";
//color idea got from HANDLE(Youri) at wgapatcher.ru :) 1.8
char htm_style[]	="<STYLE TYPE=\"text/css\">td{font-family:\"Tahoma\";font-size:9pt}\
tr{font-size:9pt}body{font-size:9pt}\
.o{background:#E0F0E0}.n{background:#FFFFFF}</STYLE>\r\n";  //1.8.2 from e0e0e0 to e0f0e0 by Charles
char htm_BodyBegin[]="<BODY BGCOLOR=FFFFFF TEXT=000000 LINK=C8C8C8>\r\n";
char htm_BodyOver[]	="</BODY>\r\n";
char htm_TableBegin[]="<TABLE BORDER=0 WIDTH=480>\r\n";
char htm_TableOver[]="</TABLE>\r\n";
char htm_s1[]="<span class=o>";
char htm_s2[]="<span class=n>";
char htm_s3[]="</span>\r\n";
//char htm_website[]="<FONT COLOR=C8C8C8>Bug reports to:<A HREF=\"http://regshot.yeah.net/\">regshot.yeah.net</FONT></A>";

//------------------------------------------------------------
//Several routines to write to output file
//------------------------------------------------------------
VOID	WriteHead(u_char * lpstr,DWORD count,BOOL isHTML)
{
	unsigned char lpcount[8];
	sprintf(lpcount,"%d",count);
	if(isHTML==TRUE)
	{
		WriteFile(hFile,htm_BR,sizeof(htm_BR)-1,&NBW,NULL);
		WriteFile(hFile,htm_TableBegin,sizeof(htm_TableBegin)-1,&NBW,NULL);
		WriteFile(hFile,htm_Td1Begin,sizeof(htm_Td1Begin)-1,&NBW,NULL);
	}
	else
		WriteFile(hFile,txt_line,sizeof(txt_line)-1,&NBW,NULL);
	WriteFile(hFile,lpstr,strlen(lpstr),&NBW,NULL);
	WriteFile(hFile,lpcount,strlen(lpcount),&NBW,NULL);
	if(isHTML==TRUE)
	{
		WriteFile(hFile,htm_Td1Over,sizeof(htm_Td1Over)-1,&NBW,NULL);
		WriteFile(hFile,htm_TableOver,sizeof(htm_TableOver)-1,&NBW,NULL);
	}
	else
		WriteFile(hFile,txt_line,sizeof(txt_line)-1,&NBW,NULL);
}

//------------------------------------------------------------
VOID	WritePart(LPCOMRESULT lpcomhead,BOOL isHTML,BOOL usecolor)
{
	DWORD n,nLen,i;
	LPBYTE lpstr;
	LPCOMRESULT lp;
	if(isHTML)
	{
		WriteFile(hFile,htm_TableBegin,sizeof(htm_TableBegin)-1,&NBW,NULL);
		WriteFile(hFile,htm_Td2Begin,sizeof(htm_Td2Begin)-1,&NBW,NULL);
	}

	for(i=0,lp=lpcomhead;lp!=NULL;i++,lp=lp->lpnextresult)
	{
		nLen=strlen(lp->lpresult);
		lpstr=lp->lpresult;
		if(isHTML)
		{ //1.8.0
		if(usecolor&&i%2==0)
			WriteFile(hFile,htm_s1,sizeof(htm_s1)-1,&NBW,NULL);
		else
			WriteFile(hFile,htm_s2,sizeof(htm_s2)-1,&NBW,NULL);
		}
		
		for(n=0;nLen>0;)
		{
			nLen<HTMLWRAPLENGTH? (n=nLen):(n=HTMLWRAPLENGTH);

			WriteFile(hFile,lpstr,n,&NBW,NULL);
			lpstr=lpstr+n;
			nLen=nLen-n;
			//WriteFile(hFile,lp->lpresult,strlen(lp->lpresult),&NBW,NULL);
			if(isHTML)
				WriteFile(hFile,htm_BR,sizeof(htm_BR)-1,&NBW,NULL);
			//else
			//	WriteFile(hFile,str_CR,sizeof(str_CR)-1,&NBW,NULL);
			// for some reason,txt don't wrap anymore since 1.50e,check below!
		}
		if(isHTML)
		{
			if(usecolor)
				WriteFile(hFile,htm_s3,sizeof(htm_s3)-1,&NBW,NULL);
		}
		else
			WriteFile(hFile,str_CR,sizeof(str_CR)-1,&NBW,NULL); //this!


	}
	if(isHTML)
	{
		WriteFile(hFile,htm_Td2Over,sizeof(htm_Td2Over)-1,&NBW,NULL);
		WriteFile(hFile,htm_TableOver,sizeof(htm_TableOver)-1,&NBW,NULL);
	}

}
//------------------------------------------------------------
VOID	WriteTitle(LPSTR lph,LPSTR lpb,BOOL isHTML)
{
	if(isHTML)
	{
		WriteFile(hFile,htm_TableBegin,sizeof(htm_TableBegin)-1,&NBW,NULL);
		WriteFile(hFile,htm_Td1Begin,sizeof(htm_Td1Begin)-1,&NBW,NULL);
	}
	WriteFile(hFile,lph,strlen(lph),&NBW,NULL);
	WriteFile(hFile,lpb,strlen(lpb),&NBW,NULL);
	if(isHTML)
	{
		WriteFile(hFile,htm_Td1Over,sizeof(htm_Td1Over)-1,&NBW,NULL);
		WriteFile(hFile,htm_TableOver,sizeof(htm_TableOver)-1,&NBW,NULL);
	}
	else
		WriteFile(hFile,str_CR,sizeof(str_CR)-1,&NBW,NULL);
}
// 1.8.0
VOID WriteHtmlbegin(void)
{
	WriteFile(hFile,htm_HTMLbegin,sizeof(htm_HTMLbegin)-1,&NBW,NULL);
	WriteFile(hFile,htm_HEADbegin,sizeof(htm_HEADbegin)-1,&NBW,NULL);
	WriteFile(hFile,htm_style,sizeof(htm_style)-1,&NBW,NULL);
	WriteFile(hFile,htm_HEADover,sizeof(htm_HEADover)-1,&NBW,NULL);
	WriteFile(hFile,htm_BodyBegin,sizeof(htm_BodyBegin)-1,&NBW,NULL);
}
VOID WriteHtmlover(void)
{
	//WriteFile(hFile,htm_website,sizeof(htm_website)-1,&NBW,NULL); //omit at 1.8
	WriteFile(hFile,htm_BodyOver,sizeof(htm_BodyOver)-1,&NBW,NULL);
	WriteFile(hFile,htm_HTMLover,sizeof(htm_HTMLover)-1,&NBW,NULL);
}
VOID WriteHtmlbr(void)
{
	WriteFile(hFile,htm_BR,sizeof(htm_BR)-1,&NBW,NULL);
}