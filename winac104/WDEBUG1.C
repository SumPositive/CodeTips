/*
**  "wDebug1.c"
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



/*===================================================================
**
*/
void
wDebug1( HWND hWnd )
{
	int  i;
	HDC  hdc;
	short  cxText,cyText;
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
	sprintf(z100, "<:CodeTips:> WinAlloc Library  [ Ver:%.2f ]",
												WinAlloc_VERSION );
	wputs_title(0, z100 );
	
//===================================================================
	
	strcpy(z100, "o _wAlloc & Write.");
	wputs_title(1, z100 );
	
	for( i=0 ; i<MAX_POINTER ; i++ )
	{
		if( i%2==0 )      LPointers[i] = _wAlloc(   5 );
		else if( i%2==1 ) LPointers[i] = _wAlloc( 256 );
		WinAssert( LPointers[i] != NULL );
		lpw = (WORD FAR *)LPointers[i];
		*lpw = (WORD)i;
		
		sprintf(z100, "LPointers[%d]=%lu=%04x:%04x *lpw=%u.      ",
						i, LPointers[i],
						HIWORD(LPointers[i]), LOWORD(LPointers[i]),
						*lpw );
		wputs( z100 );
	}
	
	_wCheck(0);  /*** (0)メモリオーバーライト・チェック ***/
	
	strcpy(z100, "o back Read.");
	wputs_title(1, z100 );
	
	for( i=MAX_POINTER-1 ; 0<=i ; i-- )
	{
		lpw = (WORD FAR *)LPointers[i];
		
		sprintf(z100, "LPointers[%d]=%lu=%04x:%04x *lpw=%u.      ",
						i, LPointers[i],
						HIWORD(LPointers[i]), LOWORD(LPointers[i]),
						*lpw );
		wputs( z100 );
	}
	
#if TRUE
	{
		int  size;
		char FAR *lpz;
		/*
		**  オーバーライトを発生させる 
		*/
		size = _wSize( LPointers[MAX_POINTER/2] );
		
		lpz = (char FAR *)LPointers[MAX_POINTER/2];
		
//		*(lpz-1)    = (char)0x01;   /* トップ・オーバー */
		*(lpz+size) = (char)0x02;   /* ボトム・オーバー */
	}
#endif
	
	_wCheck(0);  /*** (0)メモリオーバーライト・チェック ***/
	
	strcpy(z100, "o Read & _wFree.");
	wputs_title(1, z100 );
	
	for( i=0 ; i<MAX_POINTER ; i++ )
	{
		lpw = (WORD FAR *)LPointers[i];
		
		sprintf(z100, "LPointers[%d]=%lu=%04x:%04x. *lpw=%u.      ",
						i, LPointers[i],
						HIWORD(LPointers[i]), LOWORD(LPointers[i]),
						*lpw );
		wputs( z100 );
		
		_wFree( LPointers[i] );
		LPointers[i] = NULL;
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