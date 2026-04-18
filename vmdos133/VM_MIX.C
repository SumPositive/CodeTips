/*
**  "vm_mix.c"
**
**  ＶＭＭ＿Ｄｏｓ : Varturl Memory Manager for DOS : 仮想メモリ・マネージャ
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**  ミックス・デバッグ・サンプル
**
**   (C) Copyright 1989,1993 by Kazumasa (和正)
**                  Produced by MSPO
**                              開発部 "CodeTips" Project Fax:06-722-0111
**
*/

/* #define _DISP */            /* 定義すれば、経過表示されます */

#if defined(__BORLANDC__)
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
#include "vm_Dos.h"

#if defined(__TURBOC__)
//	extern unsigned _stklen = ( 12U * 1024U );
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


#define  CT_SWAPDEV    ( VM_EMS | VM_XMS )
/* #define  CT_SWAPDEV   ( VM_EMS |  VM_DISK) */
/* #define  CT_SWAPDEV   ( VM_DISK) */

#define  MAX_HEAP      (128 * (1024/DOS_PARAGRAPH))  /* パラグラフ単位 */
#define  MAX_HANDLES   1000
#define  MAX_SIZE       512   /* 最大アロケートサイズ */
#define  PAGE_SIZE        2   /* ページサイズ (Kbyte) */


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
	
	if( iNo<50 ) return(2);  /* 無視 */
	
	return(0);  /* 強制終了 */
}


int main( argc, argv )
int  argc;
char *argv[];
{
	int  i,j,k, iSize;
	char *pz;
	WORD wMAX_TEST;
	
	
	/* TITLE */
	printf("### vm_Dos Mixed Debuging. [ Ver:%.2f ] ###\n",
													VM_DOS_VERSION );
	
	if( argc!=2 || atoi(argv[1])<=0 ){
		puts("使用法: vm_mix <リピート数>");
		puts("        vm_mix  10");
		return(0);
	}
	
	wMAX_TEST = atoi(argv[1]);
	
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
	
	printf(" HEAP=%uKByte  PAGE=%uK  SIZE=%uByte  TEST=%u回ﾘﾋﾟｰﾄ\n",
										(WORD)MAX_HEAP/(1024/DOS_PARAGRAPH),
										(WORD)PAGE_SIZE,
										(WORD)MAX_SIZE,
										wMAX_TEST );
	
	
	/* Delete All */
	for( k=0 ; k<MAX_HANDLES ; k++ )
	{
		ctHandles[k] = VM_NULL;
	}
	
/*===================================================================*/
	
	if( vm_init( 0, MAX_HEAP, CT_SWAPDEV, PAGE_SIZE, errHandler ) != 0 )
		Error("vm_init");
	
	printf("\no CodeTips vm_Dos Variable & Fixed Mode Functions. \n");
	for( i=wMAX_TEST-1 ; 0<=i ; i-- ){
		printf(" %4d\r", i);
		for( j=0 ; j<MAX_HANDLES ; j++ )
		{
			if( ctHandles[ k = Random(MAX_HANDLES) ] != VM_NULL )
			{
				/* READ */
				if( (pz = (char *)vm_load(ctHandles[k],VM_CLEAN)) == NULL ){
					Error("vm_load()");
				}
				iSize = vm_size( ctHandles[k] );
				if( atoi(pz) != k ){
					printf("%4d => %s \n", k, pz );
					puts("*** Diffarent Number ! ***  -WAIT-"); getch();
				}
				
				if( iSize % 15 != 0  &&  iSize % 16 == 0 ){
					int  iNewSize;
					if( 128 < iSize ) iNewSize = iSize - 21;
					else              iNewSize = iSize + 21;
					#ifdef _DISP
						printf("%4u: Realloc  %d → %dbyte\n",
													k, iSize, iNewSize );
					#endif
					/* REALLOC */
					ctHandles[k] = vm_realloc( ctHandles[k], iNewSize );
					assert( ctHandles[k] != VM_NULL );
				} else {
					#ifdef _DISP
						printf("%4u: Free  %dbyte\n", k, iSize );
					#endif
					/* FREE */
					vm_free( ctHandles[k] );
					ctHandles[k] = VM_NULL;
				}
			}
			else
			{
				WORD w = k;
				if( MAX_SIZE<w ) w %= MAX_SIZE;
				if( w<5 ) w = 5;   /*** 以下で、5byte書き込みするため ***/
				if( w % 15 == 0 ){
					ctHandles[k] = vm_allocFix( w );  /*** Fixed mode ***/
					#ifdef _DISP
						printf("%4u: AllocFix( %4u )\n", k, w );
					#endif
				} else {
					ctHandles[k] = vm_alloc( w );
					#ifdef _DISP
						printf("%4u: Alloc   ( %4u )\n", k, w );
					#endif
				}
				assert( ctHandles[k] != VM_NULL );
				/* WRITE */
				if( (pz = (char *)vm_load(ctHandles[k],VM_DIRTY)) == NULL ){
					Error("vm_load()");
				}
				sprintf(pz, "%4d", k);
			}
		}
		
		vm_check( 0 );  /*** CHECK ***/
		
	}
	
	vm_check( 1 );  /*** CHECK ***/
	
	/* Delete All */
	vm_freeAll();
	
	vm_check( 2 );  /*** CHECK ***/
	
	vm_term();
	
/*===================================================================*/
	
	printf("\no vm_mix end.\n");
	/*
	** Terminate 
	*/
	return(0);
}


