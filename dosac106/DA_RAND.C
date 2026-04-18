/*
**  "da_rand.c"
**
**  ＤｏｓＡｌｌｏｃ : DOS memory Allocator
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**  ランダム・ベンチマークテスト・サンプル
**
**   (C) Copyright 1987,1993 by Kazumasa (和正)
**                  Produced by MSPO 
**                              開発部 "CodeTips" Project Fax:06-722-0111
**
*/
#ifdef __BORLANDC__
	#define __MSC
#endif

#ifndef _DEBUG
#define _DEBUG
#endif

#include <assert.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>    /* getch() */
#include <time.h>     /* time(), difftime() */

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


#define  MAX_POINTERS   2000 
#define  MAX_TEST       2500 
#define  RAP_TIMES         5   /* 途中経過計測回数 */

#define  ALLOC_MAX       512   /* 最大アロケートサイズ */
#define  ALLOC_MIN         5   /* 最少アロケートサイズ */
#define  ALLOC_STEP       31   /* 増加ステップ */


static LPVOID  LPV[ MAX_POINTERS ]; 


static int Random(int);     /* MSC4.0 のために */
#ifndef RAND_MAX
	#define RAND_MAX 0x7fff
#endif
static int Random( num )
int num;
{
	int iRet;
	long la;
	
	la = (long)rand() * num;
	iRet = (int)( la / (RAND_MAX+1L) );
	return iRet < 0 ? -iRet : iRet;
}


static int errHandler(int);     /* MSC4.0 のために */
static int errHandler( iNo )
int iNo;
{
	switch( iNo )
	{
		case 0: puts("_d: 致命的エラーです");
				break;
		case 1: puts("_d: ヒープメモリ・オーバーです");
				break;
		case 3: puts("_d: 引数エラーです");
				break;
	}
	return(0); /* 強制中止します */
}


int main()
{
	int  i,j,k, iLap;
	double  d, dEmpty[RAP_TIMES+1], dmsTime[RAP_TIMES+1], dctTime[RAP_TIMES+1];
	time_t  tmStart;
	
	
	/* TITLE */
	printf("### DosAlloc [ Ver:%.2f ]  - Random Benchmark Test -  ###\n",
													DosAlloc_VERSION );
	
	#if defined(_MSC_VER)
		printf("### Microsoft %d ", _MSC_VER);
		#if defined M_I86SM
			printf("[ Small ]");
		#elif defined M_I86MM
			printf("[ Medium ]");
		#elif defined M_I86CM
			printf("[ Compact ]");
		#elif defined M_I86LM
			printf("[ Large ]");
		#endif
	#elif defined(__BORLANDC__)
		printf("### Borland %x ", __BORLANDC__);
		#if defined __SMALL__
			printf("[ Small ]");
		#elif defined __MEDIUM__
			printf("[ Medium ]");
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
	
	printf(" Allocate MIN=%u. STEP=%u. MAX=%u.\n",
									ALLOC_MAX,ALLOC_STEP,ALLOC_MAX );
	
	
	/* Delete All */
	for( k=0 ; k<MAX_POINTERS ; k++ )
	{
		LPV[k] = NULL;
	}
	
/*===================================================================*/
	
	printf("\no Empty loop begin.\n");
	tmStart = time(NULL);
	for( iLap=0, i=MAX_TEST-1 ; 0<=i ; i-- ){
		printf(" %4d\r", i);
		for( j=0 ; j<MAX_POINTERS ; j++ ){
			if( LPV[ k = Random(MAX_POINTERS) ] ){
				/* NULL */
				LPV[k] = NULL;
			}
			else {
				WORD w = (WORD)k;
				if( ALLOC_MAX<w ) w %= ALLOC_MAX;
				w = (w / ALLOC_STEP) * ALLOC_STEP;
				if( w<ALLOC_MIN ) w = ALLOC_MIN;
				LPV[k] = (LPVOID)!0;
				assert( LPV[k] != NULL );
			}
		}
		if( i % (MAX_TEST/RAP_TIMES) == 0 ){
			dEmpty[iLap] = difftime( time(NULL), tmStart );
			printf("   %d. [%4d] LapTime is %8.1lf sec.\n",
									iLap, (-i+MAX_TEST), dEmpty[iLap] );
                                       /* ^^^^^^^^^^^^^Borland C Bug Fix. */
			iLap++;
		}
	}
	
	/* Delete All */
	for( k=0 ; k<MAX_POINTERS ; k++ )
	{
		if( LPV[ k ] != NULL )
		{
			LPV[k] = NULL;
		}
	}
	
/*===================================================================*/
#if (_MSC_VER >= 600) || defined(__BORLANDC__)
	
	printf("\no Normal FAR_malloc Functions. \n");
	tmStart = time(NULL);
	for( iLap=0, i=MAX_TEST-1 ; 0<=i ; i-- ){
		printf(" %4d\r", i);
		for( j=0 ; j<MAX_POINTERS ; j++ ){
			if( LPV[ k = Random(MAX_POINTERS) ] != NULL ){
				/* FREE */
				_ffree( LPV[k] );
				LPV[k] = NULL;
			}
			else {
				WORD w = (WORD)k;
				if( ALLOC_MAX<w ) w %= ALLOC_MAX;
				w = (w / ALLOC_STEP) * ALLOC_STEP;
				if( w<ALLOC_MIN ) w = ALLOC_MIN;
				LPV[k] = (LPVOID)_fmalloc( w );
				assert( LPV[k] != NULL );
			}
		}
		if( i % (MAX_TEST/RAP_TIMES) == 0 ){
			dmsTime[iLap] = difftime( time(NULL), tmStart );
			if( dmsTime[iLap] != dEmpty[iLap] ){
				d = 100.0 * ((dmsTime[iLap]-dEmpty[iLap])
				  - (dmsTime[iLap]-dEmpty[iLap])) / (dmsTime[iLap]-dEmpty[iLap]);
			} else d = 0.0;
			printf("   %d. [%4d] LapTime is %8.1lf sec. (%6.2lf%%)",
									iLap, (-i+MAX_TEST), dmsTime[iLap], d);
                                       /* ^^^^^^^^^^^^^Borland C Bug Fix. */
			if( 30.0 <= d ) printf(" Great !");
			printf("\n");
			iLap++;
		}
	}
	
	/* Delete All */
	for( k=0 ; k<MAX_POINTERS ; k++ )
	{
		if( LPV[ k ] != NULL )
		{
			_ffree( LPV[k] );
			LPV[k] = NULL;
		}
	}
	
	/*
	**  未使用ヒープメモリをＯＳに解放する
	*/
	_fheapmin();
	
	
#endif
/*===================================================================*/
	
	if( _dInit( errHandler ) != 0 ) Error("_dInit");
	
	printf("\no CodeTips DosAlloc Functions. \n");
	tmStart = time(NULL);
	for( iLap=0, i=MAX_TEST-1 ; 0<=i ; i-- ){
		printf(" %4d\r", i);
		for( j=0 ; j<MAX_POINTERS ; j++ )
		{
			if( LPV[ k = Random(MAX_POINTERS) ] != NULL )
			{
				/* FREE */
				_dFree( LPV[k] );
				LPV[k] = NULL;
			}
			else
			{
				WORD w = (WORD)k;
				if( ALLOC_MAX<w ) w %= ALLOC_MAX;
				w = (w / ALLOC_STEP) * ALLOC_STEP;
				if( w<ALLOC_MIN ) w = ALLOC_MIN;
				LPV[k] = _dAlloc( w );
				assert( LPV[k] != NULL );
			}
		}
		if( i % (MAX_TEST/RAP_TIMES) == 0 ){
			dctTime[iLap] = difftime( time(NULL), tmStart );
			if( dmsTime[iLap] != dEmpty[iLap] ){
				d = 100.0 * ((dmsTime[iLap]-dEmpty[iLap])
				  - (dctTime[iLap]-dEmpty[iLap])) / (dmsTime[iLap]-dEmpty[iLap]);
			} else d = 0.0;
			printf("   %d. [%4d] LapTime is %8.1lf sec. (%6.2lf%%)",
									iLap, (-i+MAX_TEST), dctTime[iLap], d);
                                       /* ^^^^^^^^^^^^^Borland C Bug Fix. */
			if( 30.0 <= d ) printf(" Great !");
			printf("\n");
			iLap++;
		}
	}
	
	/* Delete All */
	for( k=0 ; k<MAX_POINTERS ; k++ )
	{
		if( LPV[ k ] != NULL )
		{
			_dFree( LPV[k] );
			LPV[k] = NULL;
		}
	}
	
	_dTerm();
	
/*===================================================================*/
	
	/*
	** Terminate 
	*/
	return(0);
}


