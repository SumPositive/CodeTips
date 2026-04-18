/*
**  "vm_max.c"
**
**  ＶＭＭ＿Ｄｏｓ : Varturl Memory Manager for DOS : 仮想メモリ・マネージャ
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**  マキシマム・ベンチマークテスト・サンプル
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

extern WORD XMS_maxBlockSize( void );

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


#define  MS_SWAPDEV   (_VM_EMS)
#define  CT_SWAPDEV   ( VM_EMS)

#define  MAX_HEAP     (128 * (1024/DOS_PARAGRAPH))  /* パラグラフ単位 */
#define  PAGE_SIZE       2   /* ページサイズ (Kbyte) */

#define  ALLOC1       256  /* アロケートサイズ　１ */

#define  MEGA		(DWORD)(1024L * 1024L)


static int errHandler(int, char *);
static int errHandler( iNo, zMsg )
int iNo;
char *zMsg;
{
	if( iNo==0 )  /* Out of memory. */
	{
		return(2); /* 無視 （継続します）*/
		/* この場合、vm_alloc(),vm_allocFix()の戻値をチェック
		   しなければなりませぬ */
	}
	
	if( zMsg ) puts( zMsg );   /* メッセージがあれば表示する */
	
	return(0);  /* 強制終了 */
}


int  main()
{
	time_t  tmStart;
	double  dmsTime, dctTime;
	DWORD  dwTotal, dwTotalNext;
	char   *pz, z80[81];
	VMHAND  vh, vhList;
#ifdef MS_VMM
	char far *lpz;
	_vmhnd_t  mh, mhList;
#endif
	
	
	
	/* TITLE */
	printf("### vm_Dos Maximam Benchmark Test. [ Ver:%.2f ] ###\n",
													VM_DOS_VERSION );
	
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
	#elif defined(__BORLANDC__) || defined(__TURBOC__)
		#if defined(__BORLANDC__)
			printf("### Borland C: %x ", __BORLANDC__);
		#else
			printf("### Turbo C: %x ", __TURBOC__);
		#endif
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
	
	printf(" HEAP=%uKByte  PAGE=%uK  \n",
										(WORD)MAX_HEAP/(1024/DOS_PARAGRAPH),
										(WORD)PAGE_SIZE );
	
	
/*===================================================================*/
#ifdef MS_VMM
	
	if( _vheapinit( 0, MAX_HEAP, MS_SWAPDEV )==0 )
		Error("_vheapinit");
	
	printf("\no Microsoft _v Functions. \n");
	tmStart = time(NULL);
	dwTotal = 0L;
	dwTotalNext = MEGA;
	mhList = _VM_NULL;
	while(TRUE)
	{
		if( (mh = _vmalloc( ALLOC1 )) == _VM_NULL ) break;
		dwTotal += ALLOC1;
		
		printf("\r %8lu", dwTotal);
		
		/* WRITE  常にFARポインタが返されます */
		if( (lpz = (char far *)_vload(mh,_VM_DIRTY)) == NULL ){
			Error("_vload()");
		}
		
#ifdef _NEAR_
		sprintf(z80, "%lu : %8luByte", mhList, dwTotal);
		_fstrcpy( lpz, z80 );
#else
		sprintf(lpz, "%lu : %8luByte", mhList, dwTotal);
#endif
		
		mhList = mh;
		
		if( dwTotalNext <= dwTotal ){
			dmsTime = difftime( time(NULL), tmStart );
			printf("Bytes  Time is %8.1lf sec.\n", dmsTime);
			dwTotalNext += MEGA;
		}
	}
	
	dmsTime = difftime( time(NULL), tmStart );
	printf("\r Max: %8lu Bytes  Time is %8.1lf sec.\n", dwTotal, dmsTime);
	
	printf(" Let's Free !\n");
	while( mhList )
	{
		/* READ  常にFARポインタが返されます */
		if( (lpz = (char far *)_vload(mhList,_VM_CLEAN)) == NULL ){
			Error("_vload()");
		}
		
#ifdef _NEAR_
		_fstrcpy( z80, lpz );   /* NEARモデルのために */
		mh = atol( z80 );
#else
		mh = atol( lpz );
#endif
		
		/* FREE */
		_vfree(mhList);
		
		dwTotal -= ALLOC1;
		printf("\r %8lu", dwTotal);
		
		/* NEXT */
		mhList = mh;
	}
	
	dmsTime = difftime( time(NULL), tmStart );
	printf("\r Total Time is %8.1lf sec.\n", dmsTime);
	
	_vheapterm();
	
#endif
/*===================================================================*/
	
	if( vm_init( 0, MAX_HEAP, CT_SWAPDEV, PAGE_SIZE, errHandler ) != 0 )
		Error("vm_init");
	
	printf("\no CodeTips vm_Dos Variable Mode Functions. \n");
	tmStart = time(NULL);
	dwTotal = 0L;
	dwTotalNext = MEGA;
	vhList = VM_NULL;
	while(TRUE)
	{
		if( (vh = vm_alloc( ALLOC1 )) == VM_NULL ) break;
		dwTotal += ALLOC1;
		
		printf("\r %8lu", dwTotal);
		
		/* WRITE   ﾒﾓﾘﾓﾃﾞﾙに従ったポインタが返されます */
		if( (pz = (char *)vm_load(vh,VM_DIRTY)) == NULL ){
			Error("vm_load()");
		}
		sprintf(pz, "%lu : %8luByte", vhList, dwTotal);
		vhList = vh;
		
		if( dwTotalNext <= dwTotal ){
			dctTime = difftime( time(NULL), tmStart );
			printf("Bytes  Time is %8.1lf sec.\n", dctTime);
			dwTotalNext += MEGA;
		}
	}
	
	dctTime = difftime( time(NULL), tmStart );
	printf("\r Max: %8lu Bytes  Time is %8.1lf sec.\n", dwTotal, dctTime);
	
	printf(" Let's Free !\n");
	while( vhList )
	{
		/* READ  ﾒﾓﾘﾓﾃﾞﾙに従ったポインタが返されます */
		if( (pz = (char *)vm_load(vhList,VM_CLEAN)) == NULL ){
			Error("vm_load()");
		}
		
		vh = atol( pz );
		
		/* FREE */
		vm_free(vhList);
		
		dwTotal -= ALLOC1;
		printf("\r %8lu", dwTotal);
		
		/* NEXT */
		vhList = vh;
	}
	
	dctTime = difftime( time(NULL), tmStart );
	printf("\r Total Time is %8.1lf sec.\n", dctTime);
	
	vm_term();
	
/*===================================================================*/
	
	if( vm_init( 0, MAX_HEAP, CT_SWAPDEV, PAGE_SIZE, errHandler ) != 0 )
		Error("vm_init");
	
	printf("\no CodeTips vm_Dos Fixed Mode Functions. \n");
	tmStart = time(NULL);
	dwTotal = 0L;
	dwTotalNext = MEGA;
	vhList = VM_NULL;
	while(TRUE)
	{
		if( (vh = vm_allocFix( ALLOC1 )) == VM_NULL ) break;
		dwTotal += ALLOC1;
		
		printf("\r %8lu", dwTotal);
		
		/* WRITE */
		if( (pz = (char *)vm_load(vh,VM_DIRTY)) == NULL ){
			Error("vm_load()");
		}
		sprintf(pz, "%lu : %8luByte", vhList, dwTotal);
		vhList = vh;
		
		if( dwTotalNext <= dwTotal ){
			dctTime = difftime( time(NULL), tmStart );
			printf("Bytes  Time is %8.1lf sec.\n", dctTime);
			dwTotalNext += MEGA;
		}
	}
	
	dctTime = difftime( time(NULL), tmStart );
	printf("\r Max: %8lu Bytes  Time is %8.1lf sec.\n", dwTotal, dctTime);
	
	printf(" Let's Free !\n");
	while( vhList )
	{
		/* READ */
		if( (pz = (char *)vm_load(vhList,VM_CLEAN)) == NULL ){
			Error("vm_load()");
		}
		
		vh = atol( pz );
		
		/* FREE */
		vm_free(vhList);
		
		dwTotal -= ALLOC1;
		printf("\r %8lu", dwTotal);
		
		/* NEXT */
		vhList = vh;
	}
	
	dctTime = difftime( time(NULL), tmStart );
	printf("\r Total Time is %8.1lf sec.\n", dctTime);
	
	vm_term();
	
/*===================================================================*/
	
	/* Terminate */
	return(0);
}


