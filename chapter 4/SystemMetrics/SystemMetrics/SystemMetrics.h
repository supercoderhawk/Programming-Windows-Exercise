/*------------------------------------------------

SystemMetrics.h
	System Metrics display structure and array
	code and compile it with  Visual studio 2008
------------------------------------------------*/
#ifndef _SYSTEMMETRICS_H_
#define _SYSTEMMETRICS_H_

#ifndef _WINDOWS_
#include <windows.h>
#endif

#ifndef _INC_TCHAR
#include <tchar.h>
#endif

#define NULLINES ((int)(sizeof(sysmetrics)/sizeof(sysmetrics[0])))

struct
{
	int iIndex;
	TCHAR* szLabel;
	TCHAR* szDesc;
}

sysmetrics[] = 
{
	SM_CXSCREEN,	TEXT("SM_CXSCREEN"),
					TEXT("Screen width in pixels")
};

#endif
