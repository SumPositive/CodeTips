#ifndef _INC_WINALLOC
#define _INC_WINALLOC
/*
**  "WinAlloc.h"
**
**  ＷｉｎＡｌｌｏｃ : Windows memory Allocator
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**
**   (C) Copyright 1990,1993 by Kazumasa (和正)
**                  Produced by MSPO
**                              "CodeTips" Project Fax:06-722-0111
**
*/

/*
**  WinAlloc  Version & Release
*/
#define WinAlloc_VERSION   1.04  /* Version 1.0.4 */


#ifdef _AP_DEBUG
	#define _wAlloc(w)			D_wAlloc(w,__FILE__,__LINE__)
	#define _wCheck(i)			D_wCheck(i,__FILE__,__LINE__)
	#define _wFree(p)			D_wFree(p,__FILE__,__LINE__)
	#define _wFreeAll()			D_wFreeAll(__FILE__,__LINE__)
	#define _wInit(a,b)			D_wInit(a,b,__FILE__,__LINE__)
	#define _wRealloc(p,w)		D_wRealloc(p,w,__FILE__,__LINE__)
	#define _wSize(p)			D_wSize(p,__FILE__,__LINE__)
	#define _wTerm()			D_wTerm(__FILE__,__LINE__)
#else
	#define _wAlloc(w)			R_wAlloc(w)
	#define _wCheck(i)			FALSE
	#define _wFree(p)			R_wFree(p)
	#define _wFreeAll()			R_wFreeAll()
	#define _wInit(a,b)			R_wInit(a,b)
	#define _wRealloc(p,w)		R_wRealloc(p,w)
	#define _wSize(p)			R_wSize(p)
	#define _wTerm()			R_wTerm()
#endif

/*
**  プロトタイプ宣言 
*/
#ifdef _AP_DEBUG
	extern LPVOID    D_wAlloc(WORD,char*,int);
	extern int       D_wCheck(int,char*,int);
	extern void      D_wFree(LPVOID,char*,int);
	extern void      D_wFreeAll(char*,int);
	extern int       D_wInit(LPCATCHBUF,int (*)(int,char*),char*,int);
	extern LPVOID    D_wRealloc(LPVOID,WORD,char*,int);
	extern WORD      D_wSize(LPVOID,char*,int);
	extern void      D_wTerm(char*,int);
#else
	extern LPVOID    R_wAlloc(WORD);
	extern void      R_wFree(LPVOID);
	extern void      R_wFreeAll(void);
	extern int       R_wInit(LPCATCHBUF,int (*)(int,char*));
	extern LPVOID    R_wRealloc(LPVOID,WORD);
	extern WORD      R_wSize(LPVOID);
	extern void      R_wTerm(void);
#endif


#endif
