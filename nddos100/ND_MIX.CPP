/*
**  "nd_mix.c"
**
**  ｃｌａｓｓ　ＮｅｗＤｅｌｙ　ｆｏｒ　ＤＯＳ
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**  ミックス・デバッグ・サンプル
**
**   (C) Copyright 1991,1993 by Kazumasa (和正)
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
#include <conio.h>    // getch() 
#include <time.h>     /* time(), difftime() */

#include "NewDely.h"
NEWDELY_INSTANCE          /* NewDely インスタンス */


#define  MAX_POINTERS   1000 
#define  MAX_TEST         50 

#define  ALLOC_MAX       256   /* 最大アロケートサイズ */
#define  ALLOC_STEP       31   /* 増加ステップ */


class NDSet : public NewDely 
{
protected:
	int   iNo;
public:
	NDSet(){ iNo = 0; }
	NDSet( int i ){ iNo = i; }
	int GetNo(){ return iNo; }
};

class NDSet2 : public NDSet
{
protected:
	char  ch[ 105 ];
public:
	NDSet2( int i ){ iNo = i; }
};

NDSet*   NDP[ MAX_POINTERS ];


static int Random( int num )
{
	int iRet;
	long la;
	
	la = (long)rand() * num;
	iRet = (int)( la / (RAND_MAX+1L) );
	return iRet < 0 ? -iRet : iRet;
}


static int errHandler( int iNo )
{
	switch( iNo )
	{
		case 0: puts("致命的エラーです");
				break;
		case 1: puts("ヒープメモリ・オーバーです");
				break;
		case 3: puts("引数エラーです");
				break;
	}
	return(0); /* 強制中止します */
}


int  main()
{
	int  i,j,k;
	
	
	/* TITLE */
	printf("### class NewDely for DOS [ Ver:%.2f ]  - Mixed Debuging -  ###\n",
													NewDelyDos_VERSION );
	
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
		NDP[k] = NULL;
	}
	
	// ＮｅｗＤｅｌｙ インスタンス生成 
	NEWDELY_INITIAL( errHandler );
	
	puts("-WAIT-"); getch();
	
/*===================================================================*/
	
	printf("\no CodeTips NewDely Functions. \n");
	for( i=MAX_TEST-1 ; 0<=i ; i-- ){
		printf(" %4d\r", i);
		for( j=0 ; j<MAX_POINTERS ; j++ )
		{
			if( NDP[ k = Random(MAX_POINTERS) ] != NULL )
			{
				/* CHECK */
				if( NDP[k]->GetNo() != k ){
					printf("%4d => %d \n", k, NDP[k]->GetNo() );
					puts("*** Diffarent Number ! ***  -WAIT-"); getch();
				}
				assert( NDP[k]->GetNo() == k );
				
				#ifdef _DISP
					printf("%4u: Delete  %dbyte\n", k, NDP[k]->size() );
				#endif
				/* Delete */
				delete NDP[k];
				NDP[k] = NULL;
			}
			else
			{
				if( k%2==0 )      NDP[k] = new NDSet(k);
				else if( k%2==1 ) NDP[k] = new NDSet2(k);
				#ifdef _DISP
					printf("%4u: New( %4u ) (%04x:%04x)\n", k, NDP[k]->size(),
										FP_SEG(NDP[k]),FP_OFF(NDP[k]) );
				#endif
				assert( NDP[k] != NULL );
			}
		}
		
		NEWDELY_CHECK(0);  /* オーバーライト */
		
	}
	
	NEWDELY_CHECK(0);  /* オーバーライト */
	
	/* Delete All */
	for( k=0 ; k<MAX_POINTERS ; k++ )
	{
		if( NDP[ k ] != NULL )
		{
			delete NDP[k];
			NDP[k] = NULL;
		}
	}
	
	NEWDELY_CHECK(1);  /* リーク */
	
/*===================================================================*/
	
	printf("\no nd_mix end.\n");
	/*
	** Terminate 
	*/
	return(0);
}


