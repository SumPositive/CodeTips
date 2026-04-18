/*
**  "wBench1.c"
**
**  ＷｉｎＡｌｌｏｃ : Windows memory Allocator
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**  Walloc.c サブ・モジュール
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
#include <time.h>     // time(), difftime 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_TEST      500
#define RAP_TIMES       5   /* 途中経過計測回数 */
#define MAX_POINTER  2000
#define ALLOC1          5  /* アロケートサイズ　テスト１ */
#define ALLOC2        128  /* アロケートサイズ　テスト２ */

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
** ベンチマーク１：　ランダム Alloc & Free
*/
void
wBench1( HWND hWnd )
{
	int  i,j,k, iLap;
	HDC  hdc;
	short  cxText,cyText;
	time_t  tmStart;
	double  d, dEmpty[RAP_TIMES+1], dTapi[RAP_TIMES+1], dTwa[RAP_TIMES+1];
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
	sprintf(z100, "WinAlloc: Benchmark - RANDOM ALLOC & FREE TEST -  [ Ver:%.2f ]",
												WinAlloc_VERSION );
	wputs_title(0, z100 );
	
//===================================================================
	
	strcpy(z100, "o Empty loop begin.");
	wputs( z100 );
	
	tmStart = time(NULL);
	for( iLap=0, i=MAX_TEST-1 ; 0<=i ; i-- )
	{
		sprintf(z100, " %4d ", i);
		wputt( z100 );
		for( j=0 ; j<MAX_POINTER ; j++ )
		{
			if( LPointers[ k = Random(MAX_POINTER) ] )
			{
				LPointers[k] = NULL;
			}
			else {
				if( k%2==0 ) LPointers[k] = (LPVOID)1;
				else         LPointers[k] = (LPVOID)1;
				WinAssert( LPointers[k] != NULL );
			}
		}
		if( i % (MAX_TEST/RAP_TIMES) == 0 ){
			dEmpty[iLap] = difftime( time(NULL), tmStart );
			sprintf(z100, "   %d. [%4d] LapTime is %8.1lf sec.",
									iLap, (-i+MAX_TEST), dEmpty[iLap] );
			wputs( z100 );
			iLap++;
		}
	}
	
	// Delete All 
	for( k=0 ; k<MAX_POINTER ; k++ )
	{
		if( LPointers[ k ] )
		{
			LPointers[k] = NULL;
		}
	}
	
//===================================================================
	
	wputs("");
	strcpy(z100, "o API GlobalAlloc begin.");
	wputs( z100 );
	
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
#if FALSE
				sprintf(z100, "  FREE: LPointers[%4d]=%lu=%04x:%04x *lpw=%4u.     ",
								k, LPointers[k],
								FP_SEG(LPointers[k]), FP_OFF(LPointers[k]),
								*lpw );
				wputs( z100 );
#endif
				if( k != (int)(*lpw) ){
					sprintf(z100, "i=%d. j=%d. LPointers[%d] != [%u]", i,j,k,*lpw);
					Error(z100);
				}
				
				GlobalFree( (GLOBALHANDLE)HIWORD((LONG)LPointers[k]) );
				LPointers[k] = NULL;
			}
			else {
				if( k%2==0 ){
					LPointers[k] = (LPVOID)MAKELONG( 0, GlobalAlloc( GPTR, ALLOC1 ) );
				} else {
					LPointers[k] = (LPVOID)MAKELONG( 0, GlobalAlloc( GPTR, ALLOC2 ) );
				}
				if( LPointers[k] == NULL ) Error("GlobalAlloc");
				
				lpw = (WORD FAR *)LPointers[k];
				*lpw = (WORD)k;
#if FALSE
				sprintf(z100, " ALLOC: LPointers[%4d]=%lu=%04x:%04x *lpw=%4u.     ",
								k, LPointers[k],
								FP_SEG(LPointers[k]), FP_OFF(LPointers[k]),
								*lpw );
				wputs( z100 );
#endif
			}
		}
		if( i % (MAX_TEST/RAP_TIMES) == 0 ){
			dTapi[iLap] = difftime( time(NULL), tmStart );
			if( dTapi[iLap] != dEmpty[iLap] ){
				d = 100.0 * ((dTapi[iLap]-dEmpty[iLap])
				  - (dTapi[iLap]-dEmpty[iLap])) / (dTapi[iLap]-dEmpty[iLap]);
			} else d = 0.0;
#ifdef __BORLANDC__
			sprintf(z100, "   %d. [%4d] LapTime is %8.1lf sec. (%6.2lf%%)",
									iLap, (-i+MAX_TEST), dTapi[iLap], d);
#else
			sprintf(z100, "   %d. [%4d] LapTime is %8.1lf sec. (%6.2lf%%)",
									iLap, MAX_TEST-i, dTapi[iLap], d);
#endif
			wputs( z100 );
			iLap++;
		}
	}
	
	// Delete All 
	for( k=0 ; k<MAX_POINTER ; k++ )
	{
		if( LPointers[ k ] )
		{
			GlobalFree( (GLOBALHANDLE)HIWORD((LONG)LPointers[k]) );
			LPointers[k] = NULL;
		}
	}
	
//===================================================================
	
	wputs("");
	strcpy(z100, "o CodeTips WinAlloc begin.");
	wputs( z100 );
	
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
#if FALSE
				sprintf(z100, "  FREE: LPointers[%4d]=%lu=%04x:%04x *lpw=%4u.     ",
								k, LPointers[k],
								FP_SEG(LPointers[k]), FP_OFF(LPointers[k]),
								*lpw );
				wputs( z100 );
#endif
				if( k != (int)(*lpw) ){
					sprintf(z100, "i=%d. j=%d.\nLPointers[%d]=%lu=%04x:%04x *lpw=%u.",
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
				if( k%2==0 ) LPointers[k] = _wAlloc( ALLOC1 );
				else         LPointers[k] = _wAlloc( ALLOC2 );
				if( LPointers[k] == NULL ) Error("_wAlloc");
				lpw = (WORD FAR *)LPointers[k];
				*lpw = (WORD)k;
#if FALSE
				sprintf(z100, "WALLOC: LPointers[%4d]=%lu=%04x:%04x *lpw=%4u.     ",
								k, LPointers[k],
								FP_SEG(LPointers[k]), FP_OFF(LPointers[k]),
								*lpw );
				wputs( z100 );
#endif
			}
		}
		if( i % (MAX_TEST/RAP_TIMES) == 0 ){
			dTwa[iLap] = difftime( time(NULL), tmStart );
			if( dTapi[iLap] != dEmpty[iLap] ){
				d = 100.0 * ((dTapi[iLap]-dEmpty[iLap])
				  - (dTwa[iLap]-dEmpty[iLap])) / (dTapi[iLap]-dEmpty[iLap]);
			} else d = 0.0;
#ifdef __BORLANDC__
			sprintf(z100, "   %d. [%4d] LapTime is %8.1lf sec. (%6.2lf%%)",
									iLap, (-i+MAX_TEST), dTwa[iLap], d);
#else
			sprintf(z100, "   %d. [%4d] LapTime is %8.1lf sec. (%6.2lf%%)",
									iLap, MAX_TEST-i, dTwa[iLap], d);
#endif
			if( 30.0 <= d ) strcat(z100, " Great !");
			else if( d <= -30.0 ) strcat(z100, " 使途不適.");
			else if( d <= 0.0 ) strcat(z100, " Failure.");
			wputs( z100 );
			iLap++;
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
	wputs( z100 );
	
	ReleaseDC( hWnd, hdc );
	
	return;
}


