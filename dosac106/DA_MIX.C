/*
**  "da_mix.c"
**
**  ＤｏｓＡｌｌｏｃ : DOS memory Allocator
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**  ミックス・デバッグ・サンプル
**
**   (C) Copyright 1987,1993 by Kazumasa (和正)
**                  Produced by MSPO
**                              開発部 "CodeTips" Project Fax:06-722-0111
**
*/

#define _DISP            /* 定義すれば、経過表示されます */

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


#define  MAX_POINTERS   1000
#define  MAX_TEST         50 

#define  ALLOC_MAX       512   /* 最大アロケートサイズ */
#define  ALLOC_MIN         5   /* 最少アロケートサイズ */
#define  ALLOC_STEP       31   /* 増加ステップ */


static LPVOID  LPV[ MAX_POINTERS ];


static int Random(int);

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


static int errHandler(int);

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


int  main()
{
	int  i,j,k, iSize;
	char z20[21];
	
	
	/* TITLE */
	printf("### DosAlloc [ Ver:%.2f ]  - Mixed Debuging -  ###\n",
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
	
	printf(" Allocate MIN=%u. STEP=%u. MAX=%u.\n",
									ALLOC_MAX,ALLOC_STEP,ALLOC_MAX );
	
	/* Delete All */
	for( k=0 ; k<MAX_POINTERS ; k++ )
	{
		LPV[k] = NULL;
	}
	
/*===================================================================*/
	
	if( _dInit( errHandler ) != 0 ) Error("_dInit");
	
	printf("\no CodeTips DosAlloc Functions. \n");
	for( i=MAX_TEST-1 ; 0<=i ; i-- ){
		printf(" %4d\r", i);
		for( j=0 ; j<MAX_POINTERS ; j++ )
		{
			if( LPV[ k = Random(MAX_POINTERS) ] != NULL )
			{
				iSize = _dSize( LPV[k] );
				/* READ */
				Fmemcpy( z20, LPV[k], 20 );
				if( atoi(z20) != k ){
					printf("%4d => %s \n", k, z20 );
					puts("*** Diffarent Number ! ***  -WAIT-"); getch();
				}
				
				if( k % 15 == 0 ){
					int  iNewSize;
					if( ALLOC_MAX/2 < iSize ) iNewSize = iSize - ALLOC_STEP;
					else                      iNewSize = iSize + ALLOC_STEP;
					#ifdef _DISP
						printf("%4u: Realloc  %d → %dbyte\n",
													k, iSize, iNewSize );
					#endif
					/* REALLOC */
					LPV[k] = _dRealloc( LPV[k], iNewSize );
					assert( LPV[k] != NULL );
				} else {
					#ifdef _DISP
						printf("%4u: Free  %dbyte\n", k, iSize );
					#endif
					/* FREE */
					_dFree( LPV[k] );
					LPV[k] = NULL;
				}
			}
			else
			{
				WORD w = (WORD)k;
				if( ALLOC_MAX<w ) w %= ALLOC_MAX;
				w = (w / ALLOC_STEP) * ALLOC_STEP;
				if( w<ALLOC_MIN ) w = ALLOC_MIN;
				LPV[k] = _dAlloc( w );
				#ifdef _DISP
					printf("%4u: Alloc   ( %4u )\n", k, w );
				#endif
				assert( LPV[k] != NULL );
				
				/* WRITE */
				sprintf(z20, "%4d", k);
				Fmemcpy(LPV[k], z20, w);
			}
		}
		
		_dCheck( 0 );  /*** BLOCK CHECK ***/
		
	}
	
	_dCheck( 0 );  /*** BLOCK CHECK ***/
	
	/* Delete All */
	for( k=0 ; k<MAX_POINTERS ; k++ )
	{
		if( LPV[ k ] != NULL )
		{
			_dFree( LPV[k] );
			LPV[k] = NULL;
		}
	}
	
	_dCheck( 1 );  /*** LEAK CHECK ***/
	
	_dTerm();
	
/*===================================================================*/
	
	printf("\no da_mix end.\n");
	/*
	** Terminate 
	*/
	return(0);
}


