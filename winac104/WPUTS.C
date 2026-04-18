/*
**  "Wputs.c"
**
**  ＷｉｎＡｌｌｏｃ : Windows memory Allocator
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**  Walloc.c サブ・モジュール： 文字列出力関係
**
**  （注意） Ｗｉｎｄｏｗｓプログラミングの手本ではありませんので、念のため。
**
**   (C) Copyright 1992,1993 by Kazumasa (和正)
**                  Produced by MSPO
**                              開発部 "CodeTips" Project Fax:06-722-0111
**
*/

#include "windows.h"		/* すべてのWindowsアプリケーションに必要です */
#include "Walloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static HWND  wputsHwnd = NULL;
static HDC   wputsHdc;
static short wputsX, wputsY, wputsYofs, wputsYbottom;
static RECT  wputsRect;


/*-------------------------------------------------------------------*/
void
wputsInit(
	HWND  hwnd,
	HDC   hdc,
	short x,
	short y,
	short yOfs )
{
	PSTR pz = "マウスボタン　(左)一時停止  (右)中断";
	
	GetClientRect(hwnd, &wputsRect);
	
	SelectObject(hdc, GetStockObject(WHITE_BRUSH)); /* 塗り潰し */
	SelectObject(hdc, GetStockObject(NULL_PEN));    /* 輪郭なし */
	Rectangle(hdc, wputsRect.left, wputsRect.top,
					wputsRect.right, wputsRect.bottom );
	
	TextOut( hdc, 2,2, pz, strlen(pz) );
	
	wputsHwnd = hwnd;
	wputsHdc  = hdc;
	wputsX    = x;
	wputsY    = y + yOfs*3;
	wputsYofs = yOfs;
	
	wputsYbottom = y + ((wputsRect.bottom-y)/yOfs)*yOfs - yOfs;
	
	wputsRect.top += (yOfs * 4);
}


/*-------------------------------------------------------------------*/
void
mouseCheck( void )
{
	MSG   msg;
	BOOL  boLoop = FALSE;
	do {
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if( msg.message == WM_LBUTTONDOWN ) boLoop = TRUE;
			else
			if( msg.message == WM_LBUTTONUP ) boLoop = FALSE;
			else
			if( msg.message == WM_RBUTTONDOWN ) FatalExit(-1);
			
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			
		}
	} while( boLoop );
}


/*-------------------------------------------------------------------*/
void
wputs( char *pz )
{
	if( !wputsHwnd ) return;
	
	mouseCheck();
	
	TextOut( wputsHdc, wputsX,wputsY, pz, strlen(pz) );
	
	if( wputsY < wputsYbottom )
	{
		wputsY += wputsYofs;
	}
	else
	{
		ScrollWindow( wputsHwnd, 0, -wputsYofs, &wputsRect, NULL );
	}
	
	mouseCheck();
}


/*-------------------------------------------------------------------*/
void
wputs_title( int iLine, char *pz )
{
	if( !wputsHwnd ) return;
	
	TextOut( wputsHdc, wputsX, wputsYofs*(1+iLine), pz, strlen(pz) );
	
	mouseCheck();
}


/*-------------------------------------------------------------------*/
void
wputt( char *pz )
{
	if( !wputsHwnd ) return;
	
	TextOut( wputsHdc, wputsX,wputsY, pz, strlen(pz) );
	
	mouseCheck();
}



