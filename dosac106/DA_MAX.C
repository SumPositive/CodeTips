/*
**  "da_max.c"
**
**  ＤｏｓＡｌｌｏｃ : DOS memory Allocator
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**  マキシマム・ベンチマークテスト・サンプル
**
**   (C) Copyright 1987,1993 by Kazumasa (和正)
**                  Produced by MSPO 
**                              開発部 "CodeTips" Project Fax:06-722-0111
**
*/
#ifndef _DEBUG
#define _DEBUG
#endif

#include <assert.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>    /* getch() */
#include <time.h>     /* time() */

#if  _MSC_VER >= 600
	#include <malloc.h>
#endif
#ifdef __BORLANDC__
	#include <alloc.h>
	#define _fmalloc     farmalloc
	#define _ffree       farfree
	#define _fheapmin()  // 
#endif

#include "DosAlloc.h"

extern WORD XMS_maxBlockSize( void );

/*
**  ERROR & EXIT
*/
#define Error(msg)\
	{\
		char szBuffer[80];\
		sprintf(szBuffer, "ERR: %s(%d) %s", __FILE__, __LINE__, msg);\
		perror( szBuffer );\
		exit(-1);\
	}


#define  ALLOC1       64  /* アロケートサイズ　１ */


static int errHandler(int);

static int errHandler( iNo )
int iNo;
{
	switch( iNo )
	{
		case 0: puts("_d: 致命的エラーです");
				return(0); /* 強制中止します */
		case 3: puts("_d: 引数エラーです");
				return(0); /* 強制中止します */
		
		case 1: puts(" [Over] ＭＡＸテスト折り返し");
				break;
		case 2: puts("_d: 補助メモリ・オーバーです");
				break;
	}
	return(2); /* 無視します */
}


int  main()
{
	time_t  tmStart;
	double  dTime;
	DWORD   dwTotal, dwTotalNor;
	char    z80[81];
	char far *lpzList;
	char far *lpz;
	
	
	/* TITLE */
	printf("### DosAlloc [ Ver:%.2f ]  - Maximam Benchmark Test -  ###\n",
													DosAlloc_VERSION );
	
	#if defined(_MSC_VER)
		printf("### Microsoft %d ", _MSC_VER);
		#if defined M_I86SM
			printf("[ Small ]");
			#define  _NEAR_
		#elif defined M_I86MM
			printf("[ Medium ]");
			#define  _NEAR_
		#elif defined M_I86CM
			printf("[ Compact ]");
		#elif defined M_I86LM
			printf("[ Large ]");
		#endif
	#elif defined(__BORLANDC__)
		printf("### Borland %x ", __BORLANDC__);
		#if defined __SMALL__
			printf("[ Small ]");
			#define  _NEAR_
		#elif defined __MEDIUM__
			printf("[ Medium ]");
			#define  _NEAR_
		#elif defined __COMPACT__
			printf("[ Compact ]");
		#elif defined __LARGE__
			printf("[ Large ]");
		#endif
	#endif
	
	#if (_MSC_VER < 600) && !defined(__BORLANDC__)
		printf("\n Microsoft C 6.0 以上でコンパイルして下さい \n");
		exit(0);
	#endif
	
	printf(" AllocateSize=%u.\n", ALLOC1 );
	
/*===================================================================*/
#if (_MSC_VER >= 600) || defined(__BORLANDC__)
	
	printf("\no Normal FAR_malloc Functions. \n");
	tmStart = time(NULL);
	dwTotal = 0L;
	lpzList = NULL;
	while(TRUE)
	{
		if( (lpz = (char far *)_fmalloc( ALLOC1 )) == NULL ) break;
		dwTotal += ALLOC1;
		
		printf("\r %8lu", dwTotal);
		
#ifdef _NEAR_
		sprintf(z80, "%lu : %8luByte", (unsigned long)lpzList, dwTotal);
		Fmemcpy( lpz, z80, ALLOC1 );
#else
		sprintf(lpz, "%lu : %8luByte", (unsigned long)lpzList, dwTotal);
#endif
		
		lpzList = lpz;
		
	}
	
	dTime = difftime( time(NULL), tmStart );
	printf("\r  Max : %8lu Bytes  Time is %8.1lf sec.\n", dwTotal, dTime);
	
	dwTotalNor = dwTotal;
	
	printf("  Let's Free !\n");
	while( lpzList )
	{
#ifdef _NEAR_
		Fmemcpy( z80, lpzList, ALLOC1 );   /* NEARモデルのために */
		lpz = (char far *)atol( z80 );
#else
		lpz = (char far *)atol( lpzList );
#endif
		
		/* FREE */
		_ffree( lpzList );
		
		dwTotal -= ALLOC1;
		printf("\r %8lu", dwTotal);
		
		/* NEXT */
		lpzList = lpz;
	}
	
	dTime = difftime( time(NULL), tmStart );
	printf("\r  Total Time is %8.1lf sec.\n", dTime);
	
	/*
	**  未使用ヒープメモリをＯＳに解放する
	*/
	_fheapmin();
	
#endif
/*===================================================================*/
	
	if( _dInit( errHandler ) != 0 ) Error("_dInit");
	
	printf("\no CodeTips DosAlloc Functions. \n");
	tmStart = time(NULL);
	dwTotal = 0L;
	lpzList = NULL;
	while(TRUE)
	{
		if( (lpz = (char far *)_dAlloc( ALLOC1 )) == NULL ) break;
		dwTotal += ALLOC1;
		
		printf("\r %8lu", dwTotal);
		
#ifdef _NEAR_
		sprintf(z80, "%lu : %8luByte", (unsigned long)lpzList, dwTotal);
		Fmemcpy( lpz, z80, ALLOC1 );
#else
		sprintf(lpz, "%lu : %8luByte", (unsigned long)lpzList, dwTotal);
#endif
		
		lpzList = lpz;
		
	}
	
	dTime = difftime( time(NULL), tmStart );
	printf("\r  Max : %8lu Bytes  Time is %8.1lf sec.\n", dwTotal, dTime);
	
	if( dwTotalNor < dwTotal ){
		DWORD dw = dwTotal - dwTotalNor;
		printf("  Size: %8lu Bytes VOLUME UP !\n", dw );
	}
	
	printf("  Let's Free !\n");
	while( lpzList )
	{
#ifdef _NEAR_
		Fmemcpy( z80, lpzList, ALLOC1 );   /* NEARモデルのために */
		lpz = (char far *)atol( z80 );
#else
		lpz = (char far *)atol( lpzList );
#endif
		
		/* FREE */
		_dFree( lpzList );
		
		dwTotal -= ALLOC1;
		printf("\r %8lu", dwTotal);
		
		/* NEXT */
		lpzList = lpz;
	}
	
	dTime = difftime( time(NULL), tmStart );
	printf("\r  Total Time is %8.1lf sec.\n", dTime);
	
	_dTerm();
	
/*===================================================================*/
	
	/*
	** Terminate 
	*/
	return(0);
}


