/*
**  "nd_max.c"
**
**  ｃｌａｓｓ　ＮｅｗＤｅｌｙ　ｆｏｒ　ＤＯＳ
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**  マキシマム・ベンチマークテスト・サンプル
**
**   (C) Copyright 1991,1993 by Kazumasa (和正)
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

#if  _MSC_VER >= 700
	#include <malloc.h>
#endif
#ifdef __BORLANDC__
	#include <alloc.h>
	#define _fheapmin()  // 
#endif

#include "NewDely.h"
NEWDELY_INSTANCE          /* NewDely インスタンス */


// エラーハンドラ 
static int errHandler( int iNo )
{
	switch( iNo )
	{
		case 0: puts("致命的エラーです");
				break;
		case 1: puts(" [Over] ＭＡＸテスト折り返し");
				return(2); /* 無視します */
	}
	return(0);  /* 強制終了 */
}

#define DUMMY_SIZE   60

//----------------------------------------------[ NorSet ]
class FAR NorSet
{
protected:
	NorSet*  pNext;
	char    cDummy[DUMMY_SIZE];
public:
	NorSet( NorSet* p ){ pNext = p; }
	NorSet*  GetNext(){ return pNext; }
};

//----------------------------------------------[ NDSet ]
class NDSet : public NewDely      // NewDely より派生させる 
{
protected:
	NDSet*  pNext;
	char    cDummy[DUMMY_SIZE];
public:
	NDSet( NDSet* p ){ pNext = p; }
	NDSet*  GetNext(){ return pNext; }
};


//----------------------------------------------[ MAIN ]
int  main()
{
	time_t  tmStart;
	double  dTime;
	DWORD   dwTotal, dwTotalNor;
	NorSet  *lpNor, *lpNorList;
	NDSet   *lpND,  *lpNDList;
	
	
	// ＮｅｗＤｅｌｙ インスタンス生成 
	NEWDELY_INITIAL( errHandler );
	
	/* TITLE */
	printf("class NewDely for DOS [ Ver:%.2f ]  - MAX BENCH MARK TEST - \n",
													NewDelyDos_VERSION );
	
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
	
	printf("  sizeof(NorSet)=%u. sizeof(NDSet)=%u.\n",
							sizeof(NorSet), sizeof(NDSet) );
	
/*===================================================================*/
	
	printf("\no Normal new/delete Functions. \n");
	tmStart = time(NULL);
	dwTotal = 0L;
	lpNorList = NULL;
	while(  (lpNor = new NorSet( lpNorList )) != NULL  )
	{
		lpNorList = lpNor;
		
		dwTotal += sizeof(NorSet);
		printf("\r %8lu", dwTotal);
	}
	
	dTime = difftime( time(NULL), tmStart );
	printf("\r  Max : %8lu Bytes  Time is %8.1lf sec.\n", dwTotal, dTime);
	
	dwTotalNor = dwTotal;
	
	printf("  Let's Free !\n");
	while( lpNorList )
	{
		lpNor = lpNorList->GetNext();
		
		/* FREE */
		delete  lpNorList;
		
		dwTotal -= sizeof(NorSet);
		printf("\r %8lu", dwTotal);
		
		/* NEXT */
		lpNorList = lpNor;
	}
	
	dTime = difftime( time(NULL), tmStart );
	printf("\r  Total Time is %8.1lf sec.\n", dTime);
	
	/*
	**  未使用ヒープメモリをＯＳに解放する
	*/
	_fheapmin();
	
/*===================================================================*/
	
	printf("\no CodeTips NewDely Functions. \n");
	tmStart = time(NULL);
	dwTotal = 0L;
	lpNDList = NULL;
	while(  (lpND = new NDSet( lpNDList )) != NULL  )
	{
		lpNDList = lpND;
		
		dwTotal += sizeof(NDSet);
		
		printf("\r %8lu", dwTotal);
	}
	
	dTime = difftime( time(NULL), tmStart );
	printf("\r  Max : %8lu Bytes  Time is %8.1lf sec.\n", dwTotal, dTime);
	
	if( dwTotalNor < dwTotal ){
		DWORD dw = dwTotal - dwTotalNor;
		printf("  Size: %8lu Bytes VOLUME UP !\n", dw );
	}
	
	NEWDELY_CHECK(0);  /* オーバーライト */
	
	printf("  Let's Free !\n");
	while( lpNDList )
	{
		lpND = lpNDList->GetNext();
		
		/* FREE */
		delete  lpNDList;
		
		dwTotal -= sizeof(NDSet);
		printf("\r %8lu", dwTotal);
		
		/* NEXT */
		lpNDList = lpND;
	}
	
	dTime = difftime( time(NULL), tmStart );
	printf("\r  Total Time is %8.1lf sec.\n", dTime);
	
	NEWDELY_CHECK(1);  /* リーク */
	
/*===================================================================*/
	
	/*
	** Terminate 
	*/
	return(0);
}


