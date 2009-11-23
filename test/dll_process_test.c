#include <stdio.h>
#include <windows.h>
#include <tchar.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
    switch(fdwReason)
    {
        case    DLL_PROCESS_ATTACH:
			MessageBox(NULL,_T("Proc att"),_T(""),MB_OK);
            break;

        case    DLL_PROCESS_DETACH:
			MessageBox(NULL,_T("Proc det"),_T(""),MB_OK);
            break;

        case    DLL_THREAD_ATTACH:
			MessageBox(NULL,_T("Thread att"),_T(""),MB_OK);
            break;

        case    DLL_THREAD_DETACH:
			MessageBox(NULL,_T("Thread detach"),_T(""),MB_OK);
            break;
    }
    return  TRUE;
}

int HogePIGetInfo(){
	MessageBox(NULL,_T("HogePIGetInfo"),_T(""),MB_OK);
	return 0;
}