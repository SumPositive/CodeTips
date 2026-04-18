/*
**  "wDebug3.c"
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

#define _DISP             /* 定義すれば、経過表示されます */

#include "windows.h"
#include "WinAlloc.h"
#include "wAlloc.h"

#include <dos.h>
#include <time.h>     // time() 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define MAX_TEST       10
#define MAX_POINTER   512

#define ALLOC_MIN       5   /* 最少アロケートサイズ  文字列書き込みのため */
#define ALLOC_MAX     256   /* 最大アロケートサイズ */
#define ALLOC_STEP      7   /* サイズ・ステップ */

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
wDebug3( HWND hWnd )
{
	int  i,j,k;
	HDC  hdc;
	short  cxText,cyText;
	char  z100[101];
	TEXTMETRIC  tm;
	
	
	hdc = GetDC( hWnd );
	SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
	GetTextMetrics(hdc, &tm);
	cxText = tm.tmAveCharWidth;
	cyText = tm.tmHeight + tm.tmExternalLeading;
	
	wputsInit( hWnd, hdc, cxText, cyText*2, (short)(cyText*1.2) );
	
	// TITLE
	sprintf(z100, "WinAlloc: Debug - MIX ALL TEST -  [ Ver:%.2f ]",
												WinAlloc_VERSION );
	wputs_title(0, z100 );
	
	
//===================================================================
	
	// Clear 
	for( k=0 ; k<MAX_POINTER ; k++ )
	{
		if( LPointers[ k ] )
		{
			LPointers[k] = NULL;
		}
	}
	
//===================================================================
	
	wputs("");
	strcpy(z100, "o CodeTips WinAlloc begin.");
	wputs_title(1, z100 );
	
	for( i=MAX_TEST-1 ; 0<=i ; i-- ){
		sprintf(z100, " %4d ", i);
		wputt( z100 );
		for( j=0 ; j<MAX_POINTER ; j++ )
		{
			if( LPointers[ k = Random(MAX_POINTER) ] )
			{
				int iSize;
				
				/* READ */
				_fstrcpy( z100, (LPSTR)LPointers[k] );
				/* SIZE */
				iSize = _wSize( LPointers[k] );
				
				if( atoi(z100) != k ){
					char z80[81];
					sprintf(z80, "%4d => %s.", k, z100 );
					wputs( z80 );
					Error("*** Diffarent Number ! ***");
				}
				
				if( iSize % (ALLOC_STEP*3) == 0 ){
					int  iNewSize;
					if( 128 < iSize ) iNewSize = iSize - 21;
					else              iNewSize = iSize + 21;
					#ifdef _DISP
						sprintf(z100, "%4u: Realloc  %d → %dbyte",
													k, iSize, iNewSize );
						wputs( z100 );
					#endif
					/* REALLOC */
					LPointers[k] = _wRealloc( LPointers[k], iNewSize );
					if( LPointers[k] == NULL ) Error("_wRealloc");
				} else {
					#ifdef _DISP
						sprintf(z100, "%4u: Free  %dbyte", k, iSize );
						wputs( z100 );
					#endif
					/* FREE */
					_wFree( LPointers[k] );
					LPointers[k] = NULL;
				}
			}
			else
			{
				WORD w = (WORD)k;
				if( ALLOC_MAX<w ) w %= ALLOC_MAX;
				w = (w / ALLOC_STEP) * ALLOC_STEP;
				if( w<ALLOC_MIN ) w = ALLOC_MIN;
				LPointers[k] = _wAlloc( w );
				#ifdef _DISP
					sprintf(z100, "%4u: Alloc   ( %4u )", k, w );
					wputs( z100 );
				#endif
				if( LPointers[k] == NULL ) Error("_wAlloc");
				/* WRITE */
				sprintf(z100, "%4d", k);
				_fstrcpy( (LPSTR)LPointers[k], z100 );
			}
		}
		
		_wCheck(0);  /*** CHECK ***/
		
	}
	
	_wCheck(0);  /*** CHECK ***/
	
	// Delete All 
#if FALSE
	for( k=0 ; k<MAX_POINTER ; k++ )
	{
		if( LPointers[ k ] )
		{
			_wFree( LPointers[k] );
			LPointers[k] = NULL;
		}
	}
#else
	_wFreeAll();  /* メモリブロックの一斉高速解放  [New: v1.03] */
#endif
	
	_wCheck(1);  /*** (1)さらにメモリリーク・チェック ***/
	
//===================================================================
	
	wputs("");
	strcpy(z100, "o Complete !");
	wputs_title(1, z100 );
	
	ReleaseDC( hWnd, hdc );
	
	return;
}


