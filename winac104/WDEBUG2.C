/*
**  "wDebug2.c"
**
**  ＷｉｎＡｌｌｏｃ : Windows memory Allocator
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**  wAlloc.c サブ・モジュール
**
**  （注意） Ｗｉｎｄｏｗｓプログラミングの手本ではありませんので、念のため。
**
**   (C) Copyright 1992,1993 by Kazumasa (和正)
**                  Produced by MSPO
**                              開発部 "CodeTips" Project Fax:06-722-0111
**
*/

#include "windows.h"
#include "WinAlloc.h"
#include "wAlloc.h"

#include <dos.h>
#include <time.h>     // time() 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define MAX_TEST       10
#define MAX_POINTER   256
#define ALLOC1          3  /* アロケートサイズ　テスト１ >=sizeof(WORD) */
#define ALLOC2        128  /* アロケートサイズ　テスト２ >=sizeof(WORD) */

static LPVOID  LPointers[ MAX_POINTER ];



/*-------------------------------------------------------------------*/
static int
Random( int num )
{
	int iRet;
	long la;
	
	la = (long)rand() * num;
	iRet = (int)( la / (RAND_MAX+1L) );
	return iRet < 0 ? -iRet : iRet;
}


/*===================================================================
**
*/
void
wDebug2( HWND hWnd )
{
	int  i,j,k, iLap;
	HDC  hdc;
	short  cxText,cyText;
	time_t  tmStart;
	WORD FAR *lpw;
	char  z100[101];
	TEXTMETRIC  tm;
	
	
	hdc = GetDC( hWnd );
	SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
	GetTextMetrics(hdc, &tm);
	cxText = tm.tmAveCharWidth;
	cyText = tm.tmHeight + tm.tmExternalLeading;
	
	wputsInit( hWnd, hdc, cxText, cyText*2, (short)(cyText*1.2) );
	
	// TITLE
	sprintf(z100, " Windows ALLOC Library  [ Ver:%.2f ]",
												WinAlloc_VERSION );
	wputs_title(0, z100 );
	
//===================================================================
	
	wputs("");
	strcpy(z100, "o CodeTips WinAlloc begin.");
	wputs_title(1, z100 );
	
	tmStart = time(NULL);
	for( iLap=0, i=MAX_TEST-1 ; 0<=i ; i-- )
	{
		sprintf(z100, " %4d ", i);
		wputt( z100 );
		for( j=0 ; j<MAX_POINTER ; j++ )
		{
			if( LPointers[ k = Random(MAX_POINTER) ] )
			{
				lpw = (WORD FAR *)LPointers[k];
#if TRUE
				sprintf(z100, "  FREE: LPointers[%4d]=%lu=%04x:%04x *lpw=%4u.     ",
								k, LPointers[k],
								FP_SEG(LPointers[k]), FP_OFF(LPointers[k]),
								*lpw );
				wputs( z100 );
#endif
				if( k != (int)(*lpw) ){
					sprintf(z100, "i=%d. j=%d.\nLPointers[%d]=%lu=%04x:%04x *lpw=%u.     ",
								i,j,
								k, LPointers[k],
								FP_SEG(LPointers[k]), FP_OFF(LPointers[k]),
								*lpw );
					Error(z100);
				}
				
				_wFree( LPointers[k] );
				LPointers[k] = NULL;
			}
			else {
				if( k%2==0 )      LPointers[k] = _wAlloc( ALLOC1 );
				else if( k%2==1 ) LPointers[k] = _wAlloc( ALLOC2 );
				if( LPointers[k] == NULL ) Error("_wAlloc");
				lpw = (WORD FAR *)LPointers[k];
				*lpw = (WORD)k;
#if TRUE
				sprintf(z100, " ALLOC: LPointers[%4d]=%lu=%04x:%04x *lpw=%4u.     ",
								k, LPointers[k],
								FP_SEG(LPointers[k]), FP_OFF(LPointers[k]),
								*lpw );
				wputs( z100 );
#endif
			}
		}
	}
	
	// Delete All 
	for( k=0 ; k<MAX_POINTER ; k++ )
	{
		if( LPointers[ k ] )
		{
			_wFree( LPointers[k] );
			LPointers[k] = NULL;
		}
	}
	
	_wCheck(1);  /*** (1)さらにメモリリーク・チェック ***/
	
//===================================================================
	
	wputs("");
	strcpy(z100, "o Complete !");
	wputs_title(1, z100 );
	
	ReleaseDC( hWnd, hdc );
	
	return;
}


