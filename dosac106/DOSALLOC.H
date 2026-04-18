#ifndef _INC_DOSALLOC
#define _INC_DOSALLOC
/*
**  "DosAlloc.h"
**
**  ＤｏｓＡｌｌｏｃ : DOS memory Allocator
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**
**   (C) Copyright 1987,1993 by Kazumasa (和正)
**                  Produced by MSPO
**                              "CodeTips" Project Fax:06-722-0111
**
*/

/*
**  DosAlloc  Version & Release
*/
#define DosAlloc_VERSION   1.06   /* Version 1.0.6 */


#ifndef TRUE
#define TRUE     1
#endif

#ifndef FALSE
#define FALSE    0
#endif

#ifndef BOOL
typedef  unsigned      BOOL;
#endif

#ifndef WORD
typedef  unsigned int  WORD;
#endif

#ifndef DWORD
typedef  unsigned long  DWORD;
#endif

#ifndef LPVOID
	#if (_MSC_VER >= 500) || defined(__BORLANDC__)
		typedef void _far*      LPVOID;
	#else
		typedef char far*       LPVOID;
	#endif
#endif


#ifdef _AP_DEBUG
	#define _dAlloc(w)			D_dAlloc(w,__FILE__,__LINE__)
	#define _dCheck(i)			D_dCheck(i,__FILE__,__LINE__)
	#define _dFree(p)			D_dFree(p,__FILE__,__LINE__)
	#define _dInit(a)			D_dInit(a,__FILE__,__LINE__)
	#define _dRealloc(p,w)		D_dRealloc(p,w,__FILE__,__LINE__)
	#define _dSize(p)			D_dSize(p,__FILE__,__LINE__)
	#define _dTerm()			D_dTerm(__FILE__,__LINE__)
#else
	#define _dAlloc(w)			R_dAlloc(w)
	#define _dCheck(i)			FALSE
	#define _dFree(p)			R_dFree(p)
	#define _dInit(a)			R_dInit(a)
	#define _dRealloc(p,w)		R_dRealloc(p,w)
	#define _dSize(p)			R_dSize(p)
	#define _dTerm()			R_dTerm()
#endif

/*
**  プロトタイプ宣言 
*/
#ifdef _AP_DEBUG
	extern LPVOID    D_dAlloc(WORD,char*,int);
	extern int       D_dCheck(int,char*,int);
	extern void      D_dFree(LPVOID,char*,int);
	extern int       D_dInit(int (*)(int),char*,int);
	extern LPVOID    D_dRealloc(LPVOID,WORD,char*,int);
	extern WORD      D_dSize(LPVOID,char*,int);
	extern void      D_dTerm(char*,int);
#else
	extern LPVOID    R_dAlloc(WORD);
	extern void      R_dFree(LPVOID);
	extern int       R_dInit(int (*)(int));
	extern LPVOID    R_dRealloc(LPVOID,WORD);
	extern WORD      R_dSize(LPVOID);
	extern void      R_dTerm(void);
#endif


/* Far_mem */
#if (_MSC_VER >= 500) || defined(__BORLANDC__)
	extern void Fmemset( void _far *, int, unsigned );
	extern void Fmemcpy( void _far *, const void _far *, unsigned );
#else
	extern void Fmemset( char far *, int, unsigned );
	extern void Fmemcpy( char far *, char far *, unsigned );
#endif


#endif
