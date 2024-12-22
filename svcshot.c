
#include "global.h"
#include "winsvc.h"



VOID	GetServiceSnap(LPSVCCONTENT lpSvcContent)
{
	//Open the Service Control Manager
	SC_HANDLE sc = OpenSCManager (NULL,NULL,SC_MANAGER_ENUMERATE_SERVICE);
	ULONG i;

	//Check if OpenSCManager returns NULL. Otherwise proceed
	if (sc != NULL)
	{
		ENUM_SERVICE_STATUS service_data, *lpservice;	   
        BOOL retVal;
		QUERY_SERVICE_CONFIG* g_psc = NULL;
        DWORD bytesNeeded,srvCount,resumeHandle = 0,srvType, srvState;

        srvType = SERVICE_WIN32 | SERVICE_DRIVER;
        srvState = SERVICE_STATE_ALL;

		//Call EnumServicesStatus using the handle returned by OpenSCManager
		retVal = EnumServicesStatus(sc,srvType,srvState,&service_data,sizeof(service_data),&bytesNeeded,&srvCount,&resumeHandle);

		//Check if EnumServicesStatus needs more memory space
		if ((retVal == FALSE) || GetLastError() == ERROR_MORE_DATA) 
		{
			DWORD dwBytes = bytesNeeded + sizeof(ENUM_SERVICE_STATUS);
			lpservice = MYALLOC(dwBytes);
			EnumServicesStatus (sc,srvType,srvState,lpservice,dwBytes, &bytesNeeded,&srvCount,&resumeHandle);
		}

		for(i=0;i<srvCount;i++)
		{
			SC_HANDLE hService = OpenService(sc, lpservice[i].lpServiceName, SERVICE_ALL_ACCESS);
                
			if (hService) {
				QUERY_SERVICE_CONFIG sc;
				DWORD dwBytesNeeded = 0;
                
				// Try to get information about the query
				BOOL bRetVal = QueryServiceConfig(hService, &sc, sizeof(QUERY_SERVICE_CONFIG), &dwBytesNeeded);
                
				if (!bRetVal) {
					DWORD retVal = GetLastError();

					if (ERROR_INSUFFICIENT_BUFFER == retVal) {

						DWORD dwBytes = sizeof(QUERY_SERVICE_CONFIG) + dwBytesNeeded;
						g_psc = MYALLOC(dwBytes);
                        
						bRetVal = QueryServiceConfig(hService, g_psc, dwBytes,  &dwBytesNeeded);
                        
						if (!bRetVal) {
                   
									MYFREE(g_psc);
									g_psc = NULL;
									break;
						}
                        
					}
					
					
					printf("%s, %s, %s, %d, %d, %d, %d, %s\n", 
								lpservice[i].lpServiceName, 
								lpservice[i].lpDisplayName, 
								g_psc->lpBinaryPathName, 
								lpservice[i].ServiceStatus.dwCurrentState, 
								g_psc->dwStartType, 
								g_psc->dwServiceType, 
								g_psc->dwTagId, 
								g_psc->lpServiceStartName);
							MYFREE(g_psc);
						}
					}


              }
			  MYFREE(lpservice);
         }

         //Close the SC_HANLDE returned by OpenSCManager
         CloseServiceHandle(sc);    


	return ;
}

VOID SaveServiceSnap(){

	return ;
}
