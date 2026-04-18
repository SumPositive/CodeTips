/*
**  "Walloc.c"
**
**  ＷｉｎＡｌｌｏｃ : Windows memory Allocator
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**  サンプル・リスト   複数起動可能です
**
**  （注意） WM_PAINT:処理をしていませんので、ウインドゥ変化時の再描画は行わ
**           れません。 必要最小限の表示処理のみ行っています。
**           Ｗｉｎｄｏｗｓプログラミングの手本ではありませんので、念のため。
**
**   (C) Copyright 1992,1993 by Kazumasa (和正)
**                  Produced by MSPO
**                              開発部 "CodeTips" Project Fax:06-722-0111
**
*/

#include <windows.h>
#include "WinAlloc.h"
#include "wAlloc.h"


/*** WinAlloc< 0 > Windows実行環境バッファ宣言 ***/
CATCHBUF  CatchBuf;     /* _wInitに渡すため */

HANDLE hInst;           /* 現在のインスタンス */


/*===================================================================
**WinMain:
*/
int PASCAL
WinMain(
    HANDLE hInstance ,
    HANDLE hPrevInstance ,
    LPSTR lpCmdLine ,
    int nCmdShow )
{
    MSG msg;
	
    /*** WinAlloc< 1 > Windows動作モードを調べる ***/
    if( !(GetWinFlags() & WF_PMODE) ) /* プロテクトモードではない */
        return FALSE;  /* Error */
	
    /*** WinAlloc< 2 > Windows実行環境を得る ***/
    if( Catch( (LPCATCHBUF)&CatchBuf )==(-1) ) return FALSE;  /* Error */
	
	
    if(!hPrevInstance){
	    if(!InitApplication(hInstance)) return FALSE;
	}
	
    if (!InitInstance(hInstance, nCmdShow)) return FALSE;
	
    while( GetMessage(&msg, NULL, NULL, NULL) )
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	
    return msg.wParam;
}


/*===================================================================
**InitApplication:
*/
BOOL
InitApplication( HANDLE hInstance )
{
    WNDCLASS  wc;
	
    wc.style         = NULL;
    wc.lpfnWndProc   = MainWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH); 
    wc.lpszMenuName  = "WallocMenu";
    wc.lpszClassName = "WallocWClass";
	
    return RegisterClass(&wc);
}


/*-------------------------------------------------------------------
**WinAlloc: エラーハンドラ
*/
int
errHandler( int iErrNo, char *zMessage )
{
    if( iErrNo==0 )  /* 0: Out of memory. メモリ・オーバーです */
    {
		MessageBox(NULL, "Out of memory.\n 続行します",
						 "WinAlloc",
						 MB_ICONHAND|MB_SYSTEMMODAL);
   	    return(2);  /* 無視します */
        /* _wAlloc, _wRealloc の戻値が、NULL になります */
	}
	
    if( zMessage )   /* メッセージがあれば表示する */
    {
        MessageBox(NULL, zMessage, "WinAlloc ERROR",
                                    MB_ICONHAND|MB_SYSTEMMODAL);
    }
	
	return(0);  /* 強制終了 */
}


/*===================================================================
**InitInstance:
*/
BOOL
InitInstance(
    HANDLE  hInstance ,
    int     nCmdShow )
{
    HWND            hWnd;
	
    hInst = hInstance;
	
    hWnd = CreateWindow(	"WallocWClass",
        					"WinAlloc Benchmark",
					        WS_OVERLAPPEDWINDOW,
					        CW_USEDEFAULT,
					        CW_USEDEFAULT,
					        CW_USEDEFAULT,
					        CW_USEDEFAULT,
					        NULL,
					        NULL,
					        hInstance,
					        NULL );
	
    if( !hWnd ) return FALSE;
	
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    /*** WinAlloc< 3 > WinAllocを初期化する ***/
    if( !_wInit( (LPCATCHBUF)&CatchBuf, errHandler ) ) return FALSE;
    
    return TRUE;
}


/********************************************************************
**MainWndProc:
*/
long FAR PASCAL
MainWndProc(
	HWND hWnd ,
	unsigned message ,
	WORD wParam ,
	LONG lParam )
{
    FARPROC lpProcAbout;
    
    
    switch( message )
    {
        case WM_COMMAND:
            switch( wParam )
            {
                case IDM_ABOUT:
                    lpProcAbout = MakeProcInstance(About, hInst);
                    DialogBox(hInst, "AboutBox", hWnd, lpProcAbout);
                    FreeProcInstance(lpProcAbout);
                    break;
                
                case IDM_DEB_1:  /* デバッグ１：シーケンシャル */
                    wDebug1(hWnd);
                    return DefWindowProc(hWnd, message, wParam, lParam);
                    break;
                
                case IDM_DEB_2:  /* デバッグ２：ランダム */
                    wDebug2(hWnd);
                    return DefWindowProc(hWnd, message, wParam, lParam);
                    break;
                
                case IDM_DEB_3:  /* デバッグ３：ミックス */
                    wDebug3(hWnd);
                    return DefWindowProc(hWnd, message, wParam, lParam);
                    break;
                
                case IDM_BENCH_1:  /* ベンチマーク１：ランダム */
                    wBench1(hWnd);
                    return DefWindowProc(hWnd, message, wParam, lParam);
                    break;
                
                case IDM_BENCH_2:  /* ベンチマーク２：ランダム＆ミックス */
                    wBench2(hWnd);
                    return DefWindowProc(hWnd, message, wParam, lParam);
                    break;
                
                case IDM_BENCH_3:  /* ベンチマーク３：マックス */
                    wBench3(hWnd);
                    return DefWindowProc(hWnd, message, wParam, lParam);
                    break;
                
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        
        case WM_DESTROY:
            /*** WinAlloc< 4 > WinAllocを終了する ***/
            _wTerm();
            PostQuitMessage(0);
            break;
        
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return NULL;
}


/********************************************************************
**About:
*/
BOOL FAR PASCAL
About(
	HWND hDlg ,
	unsigned message ,
	WORD wParam ,
	LONG lParam )
{
    switch (message)
    {
	    case WM_INITDIALOG:
    	    return TRUE;
		
	    case WM_COMMAND:
    	    if (wParam == IDOK || wParam == IDCANCEL)
    	    {
		        EndDialog(hDlg, TRUE);
        		return TRUE;
	        }
        	break;
    }
    return FALSE;
}



