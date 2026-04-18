/*
**  "vm_bench.c"
**
**  ＶＭＭ＿Ｄｏｓ : Varturl Memory Manager for DOS : 仮想メモリ・マネージャ
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**  ベンチマーク・テスト・サンプル
**
**   (C) Copyright 1989,1993 by Kazumasa (和正)
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
#include "vm_Dos.h"

#if (_MSC_VER >= 700)
	#include <vmemory.h>
	#define MS_VMM
#endif

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


#define  MS_SWAPDEV   (_VM_ALLSWAP)
#define  CT_SWAPDEV   ( VM_ALLSWAP)

static WORD wMAX_HEAP = ( 64 * (1024/DOS_PARAGRAPH));  /* パラグラフ単位 */
static char cPAGE_SIZE =   2;   /* ページサイズ (Kbyte) */

#define  MAX_HANDLES  1000 
#define  MAX_TEST      250 
#define  RAP_TIMES       5   /* 途中経過計測回数 */

#define  ALLOC1          3  /* アロケートサイズ　テスト１ */
#define  ALLOC2         64  /* アロケートサイズ　テスト２ */
#define  ALLOC3        256  /* アロケートサイズ　テスト３ */

#ifdef MS_VMM
static _vmhnd_t  msHandles[ MAX_HANDLES ];  /* Microsoft */
#endif
static VMHAND    ctHandles[ MAX_HANDLES ];  /* CodeTips */


static int Random(int);

#ifndef RAND_MAX
	#define RAND_MAX 0x7fff      /* MS-C 4.0 に捧げる */
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


static int errHandler(int, char *);
static int errHandler( iNo, zMsg )
int iNo;
char *zMsg;
{
	if( zMsg ) puts( zMsg );   /* メッセージがあれば表示する */
	
	return(0);  /* 強制終了 */
}


int main( argc, argv )
int  argc;
char *argv[];
{
	int  i,j,k, iLap;
	double  d, dEmpty[RAP_TIMES+1], dmsTime[RAP_TIMES+1], dctTime[RAP_TIMES+1];
	time_t  tmStart;
	
	
	/* TITLE */
	printf("### vm_Dos Benchmark Test. [ Ver:%.2f ] ###\n",
													VM_DOS_VERSION );
	if( argc!=3 ){
		puts("使用法: vm_bench <占有ヒープサイズ(Kbyte)> <ページサイズ(Kbyte)>");
		puts("                 < 0=全て                > < 1|2|4|8|16 >");
		puts("        vm_bench   0 2");
		puts("        vm_bench 128 2");
		puts("        vm_bench  64 2");
		puts("        vm_bench  16 2");
		return(0);
	}
	
	wMAX_HEAP = ( atoi(argv[1]) * (1024/DOS_PARAGRAPH));  /* パラグラフ単位 */
	cPAGE_SIZE = (char)atoi(argv[2]);   /* ページサイズ (Kbyte) */
	
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
	#elif defined(__BORLANDC__) || defined(__TURBOC__)
		#if defined(__BORLANDC__)
			printf("### Borland C: %x ", __BORLANDC__);
		#else
			printf("### Turbo C: %x ", __TURBOC__);
		#endif
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
	
	printf(" HEAP=%uKByte  PAGE=%uK  TEST=%u回ﾘﾋﾟｰﾄ\n",
									(WORD)wMAX_HEAP/(1024/DOS_PARAGRAPH),
									(WORD)cPAGE_SIZE,
									(WORD)MAX_TEST );
	
	
	/* Delete All */
	for( k=0 ; k<MAX_HANDLES ; k++ )
	{
#ifdef MS_VMM
		msHandles[k] = _VM_NULL;
#endif
		ctHandles[k] = VM_NULL;
	}
	
/*===================================================================*/
	
	printf("\no Empty loop begin.\n");
	tmStart = time(NULL);
	for( iLap=0, i=MAX_TEST-1 ; 0<=i ; i-- ){
		printf(" %4d\r", i);
		for( j=0 ; j<MAX_HANDLES ; j++ ){
			if( ctHandles[ k = Random(MAX_HANDLES) ] ){
				/* NULL */
				ctHandles[k] = VM_NULL;
			}
			else {
				if( k%3==0 )      ctHandles[k] = 1;
				else if( k%3==1 ) ctHandles[k] = 1;
				else              ctHandles[k] = 1;
				assert( ctHandles[k] == 1 );
			}
		}
		if( i % (MAX_TEST/RAP_TIMES) == 0 ){
			dEmpty[iLap] = difftime( time(NULL), tmStart );
			printf("   %d. [%4d] LapTime is %8.1lf sec.\n",
									iLap, (-i+MAX_TEST), dEmpty[iLap] );
                                       /* ^^^^^^^^^^^^^Borland C Bug Fix. */
#ifndef MS_VMM
			dmsTime[iLap] = 999.0;  /* Divid by 0 を発生させないように */
#endif
			iLap++;
		}
	}
	
	/* Delete All */
	for( k=0 ; k<MAX_HANDLES ; k++ )
	{
		if( ctHandles[ k ] != VM_NULL )
		{
			ctHandles[k] = VM_NULL;
		}
	}
	
/*===================================================================*/
#ifdef MS_VMM
	
	if( _vheapinit( 0, wMAX_HEAP, MS_SWAPDEV )==0 )
		Error("_vheapinit");
	
	printf("\no Microsoft _v Functions. \n");
	tmStart = time(NULL);
	for( iLap=0, i=MAX_TEST-1 ; 0<=i ; i-- ){
		printf(" %4d\r", i);
		for( j=0 ; j<MAX_HANDLES ; j++ ){
			if( msHandles[ k = Random(MAX_HANDLES) ] != _VM_NULL ){
				_vfree( msHandles[k] );
				msHandles[k] = _VM_NULL;
			}
			else {
				if( k%3==0 )      msHandles[k] = _vmalloc( ALLOC1 );
				else if( k%3==1 ) msHandles[k] = _vmalloc( ALLOC2 );
				else              msHandles[k] = _vmalloc( ALLOC3 );
				assert( msHandles[k] != _VM_NULL );
			}
		}
		if( i % (MAX_TEST/RAP_TIMES) == 0 ){
			dmsTime[iLap] = difftime( time(NULL), tmStart );
			d = 100.0 * ((dmsTime[iLap]-dEmpty[iLap])
			  - (dmsTime[iLap]-dEmpty[iLap])) / (dmsTime[iLap]-dEmpty[iLap]);
			printf("   %d. [%4d] LapTime is %8.1lf sec. (%6.2lf%%)",
									iLap, (MAX_TEST-i), dmsTime[iLap], d);
			if( 30.0 <= d ) printf(" Great !");
			printf("\n");
			iLap++;
		}
	}
	
	/* Delete All */
	for( k=0 ; k<MAX_HANDLES ; k++ )
	{
		if( msHandles[ k ] != _VM_NULL )
		{
			_vfree( msHandles[k] );
			msHandles[k] = _VM_NULL;
		}
	}
	
	_vheapterm();
	
#endif
/*===================================================================*/
	
	if( vm_init( 0, wMAX_HEAP, CT_SWAPDEV, cPAGE_SIZE, errHandler ) != 0 )
		Error("vm_init");
	
	printf("\no CodeTips vm_Dos Variable Mode Functions. \n");
	tmStart = time(NULL);
	for( iLap=0, i=MAX_TEST-1 ; 0<=i ; i-- ){
		printf(" %4d\r", i);
		for( j=0 ; j<MAX_HANDLES ; j++ ){
			if( ctHandles[ k = Random(MAX_HANDLES) ] != VM_NULL ){
				vm_free( ctHandles[k] );
				ctHandles[k] = VM_NULL;
			}
			else {
				if( k%3==0 )      ctHandles[k] = vm_alloc( ALLOC1 );
				else if( k%3==1 ) ctHandles[k] = vm_alloc( ALLOC2 );
				else              ctHandles[k] = vm_alloc( ALLOC3 );
				assert( ctHandles[k] != VM_NULL );
			}
		}
		if( i % (MAX_TEST/RAP_TIMES) == 0 ){
			dctTime[iLap] = difftime( time(NULL), tmStart );
			d = 100.0 * ((dmsTime[iLap]-dEmpty[iLap])
			  - (dctTime[iLap]-dEmpty[iLap])) / (dmsTime[iLap]-dEmpty[iLap]);
#ifdef MS_VMM
			printf("   %d. [%4d] LapTime is %8.1lf sec. (%6.2lf%%)",
									iLap, MAX_TEST-i, dctTime[iLap], d);
			if( 30.0 <= d ) printf(" Great !");
#else
			printf("   %d. [%4d] LapTime is %8.1lf sec.           ",
									iLap, (-i+MAX_TEST), dctTime[iLap]);
                                       /* ^^^^^^^^^^^^^Borland C Bug Fix. */
			if( 30.0 <= d ) printf("        ");
#endif
			printf("\n");
			iLap++;
		}
	}
	
	/* Delete All */
	for( k=0 ; k<MAX_HANDLES ; k++ )
	{
		if( ctHandles[ k ] != VM_NULL )
		{
			vm_free( ctHandles[k] );
			ctHandles[k] = VM_NULL;
		}
	}
	
	vm_term();
	
/*===================================================================*/
	
	if( vm_init( 0, wMAX_HEAP, CT_SWAPDEV, cPAGE_SIZE, errHandler ) != 0 )
		Error("vm_init");
	
	printf("\no CodeTips vm_Dos Fixed Mode Functions. \n");
	tmStart = time(NULL);
	for( iLap=0, i=MAX_TEST-1 ; 0<=i ; i-- ){
		printf(" %4d\r", i);
		for( j=0 ; j<MAX_HANDLES ; j++ ){
			if( ctHandles[ k = Random(MAX_HANDLES) ] != VM_NULL ){
				vm_free( ctHandles[k] );
				ctHandles[k] = VM_NULL;
			}
			else {
				if( k%3==0 )      ctHandles[k] = vm_allocFix( ALLOC1 );
				else if( k%3==1 ) ctHandles[k] = vm_allocFix( ALLOC2 );
				else              ctHandles[k] = vm_allocFix( ALLOC3 );
				assert( ctHandles[k] != VM_NULL );
			}
		}
		if( i % (MAX_TEST/RAP_TIMES) == 0 ){
			dctTime[iLap] = difftime( time(NULL), tmStart );
			d = 100.0 * ((dmsTime[iLap]-dEmpty[iLap])
			  - (dctTime[iLap]-dEmpty[iLap])) / (dmsTime[iLap]-dEmpty[iLap]);
#ifdef MS_VMM
			printf("   %d. [%4d] LapTime is %8.1lf sec. (%6.2lf%%)",
									iLap, MAX_TEST-i, dctTime[iLap], d);
			if( 30.0 <= d ) printf(" Great !");
#else
			printf("   %d. [%4d] LapTime is %8.1lf sec.           ",
									iLap, (-i+MAX_TEST), dctTime[iLap]);
                                       /* ^^^^^^^^^^^^^Borland C Bug Fix. */
			if( 30.0 <= d ) printf("        ");
#endif
			printf("\n");
			iLap++;
		}
	}
	
	/* Delete All */
	for( k=0 ; k<MAX_HANDLES ; k++ )
	{
		if( ctHandles[ k ] != VM_NULL )
		{
			vm_free( ctHandles[k] );
			ctHandles[k] = VM_NULL;
		}
	}
	
	vm_term();
	
/*===================================================================*/
	
	/* Terminate */
	return(0);
}


