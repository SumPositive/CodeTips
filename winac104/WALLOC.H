#ifndef _INC_WALLOC
#define _INC_WALLOC
/*
**  "wAlloc.h"
**
**  ＷｉｎＡｌｌｏｃ : Windows memory Allocator
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**  Walloc.c ヘッダファイル
**
**   (C) Copyright 1992,1993 by Kazumasa (和正)
**                  Produced by MSPO
**                              "CodeTips" Project Fax:06-722-0111
**
*/

#define IDM_ABOUT		100
#define IDM_DEB_1		111
#define IDM_DEB_2		112
#define IDM_DEB_3		113
#define IDM_BENCH_1		121
#define IDM_BENCH_2		122
#define IDM_BENCH_3		123


/*
**  Export Function
*/
long FAR PASCAL MainWndProc( HWND, unsigned, WORD, LONG );
BOOL FAR PASCAL About( HWND, unsigned, WORD, LONG );


extern int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
extern BOOL InitApplication(HANDLE);
extern BOOL InitInstance(HANDLE, int);


/* "wDebug1.c" */
void  wDebug1(HWND);
/* "wDebug2.c" */
void  wDebug2(HWND);
/* "wDebug3.c" */
void  wDebug3(HWND);

//  "wBench1.c"
void  wBench1(HWND);
//  "wBench21.c"
void  wBench2(HWND);
//  "wBench3.c"
void  wBench3(HWND);

/*
**  "wputs.c"
*/
void wputsInit(HWND,HDC, short,short,short);
void wputs(char *);
void wputs_title(int, char *);
void wputt(char *);


//
//  ERROR & EXIT
//
#define Error(msg)\
	{\
		char zError[101];\
		sprintf(zError, "%s(%d) %s",__FILE__,__LINE__,msg);\
		MessageBox(NULL, zError, "Fatal Error", MB_OK|MB_ICONSTOP);\
		FatalExit(-1);\
	}


#ifdef _DEBUG
#define WinAssert(exp)\
	{\
		if(!(exp))\
		{\
			char zWinAssert[101];\
			sprintf(zWinAssert, "%s(%d)", __FILE__, __LINE__);\
			if( MessageBox(NULL, zWinAssert, "WinAlloc: Assertion Error",\
				MB_ABORTRETRYIGNORE|MB_ICONSTOP)==IDABORT ) FatalExit(-1);\
		}\
	}
#else
#define WinAssert(exp);
#endif


#endif
