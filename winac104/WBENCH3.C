/*
**  "wBench3.c"
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
#include <time.h>     // time() 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ALLOC         256  /* アロケートサイズ */
#define MEGA		(DWORD)(1024L * 1024L)



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
** ベンチマーク３：　マックス 
*/
void
wBench3( HWND hWnd )
{
	HDC  hdc;
	short  cxText,cyText;
	time_t  tmStart;
	double  dctTime;
	char    z100[101];
	TEXTMETRIC  tm;
	DWORD  dwTotal, dwTotalNext;
	LPVOID  lp, lpList;
	
	
	hdc = GetDC( hWnd );
	SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
	GetTextMetrics(hdc, &tm);
	cxText = tm.tmAveCharWidth;
	cyText = tm.tmHeight + tm.tmExternalLeading;
	
	wputsInit( hWnd, hdc, cxText, cyText*2, (short)(cyText*1.2) );
	
	// TITLE
	sprintf(z100, "WinAlloc: Benchmark - MAX ALLOC TEST -  [ Ver:%.2f ]",
												WinAlloc_VERSION );
	wputs_title(0, z100 );
	
	wputs_title(1, "(警告) ｴﾝﾊﾝｽﾄﾓｰﾄﾞで、ﾃﾞｨｽｸｽﾜｯﾌﾟが始まると恐ろしく遅い ^^;");
	wputs_title(2, "                            止めるならば今のうちに ！" );
	wputs_title(3, "(注目) エラー表示されたら、[無視]を選択して下さい。" );
	
//===================================================================
	
	wputs("");
	strcpy(z100, "o CodeTips WinAlloc begin.");
	wputs( z100 );
	
	tmStart = time(NULL);
	dwTotal = 0L;
	dwTotalNext = MEGA;
	lpList = NULL;
	while(TRUE)
	{
		if( (lp = _wAlloc( ALLOC )) == NULL ) break;
		dwTotal += ALLOC;
		
		sprintf(z100, "  %8lu", dwTotal);
		wputt( z100 );
		
		/* WRITE */
		sprintf(z100, "%lu : %8luByte", lpList, dwTotal);
		_fstrcpy( lp, z100 );
		lpList = lp;
		
		if( dwTotalNext <= dwTotal ){
			dctTime = difftime( time(NULL), tmStart );
			sprintf(z100, "     %8lu Bytes  Time is %8.1lf sec.", dwTotal, dctTime);
			wputs( z100 );
			dwTotalNext += MEGA;
		}
	}
	
	dctTime = difftime( time(NULL), tmStart );
	sprintf(z100, "Max: %8lu Bytes  Time is %8.1lf sec.", dwTotal, dctTime);
	wputs( z100 );
	
	wputs("Let's Free !");
	while( lpList )
	{
		/* READ */
		_fstrcpy(z100, lpList);
		
		lp = (LPVOID)atol( z100 );
		
		/* FREE */
		_wFree( lpList );
		
		dwTotal -= ALLOC;
		sprintf(z100, " %8lu", dwTotal);
		wputt( z100 );
		
		/* NEXT */
		lpList = lp;
	}
	
	dctTime = difftime( time(NULL), tmStart );
	sprintf(z100, "Total Time is %8.1lf sec.", dctTime);
	wputs( z100 );
	
	_wCheck(1);  /*** (1)メモリリーク・チェック ***/
	
//===================================================================
	
	wputs("");
	strcpy(z100, "o Complete !");
	wputs( z100 );
	
	ReleaseDC( hWnd, hdc );
	
	return;
}


