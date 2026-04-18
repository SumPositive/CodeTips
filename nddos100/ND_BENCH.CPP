/*
**  "ND_bench.cpp"
**
**  ｃｌａｓｓ　ＮｅｗＤｅｌｙ　ｆｏｒ　ＤＯＳ
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**  ベンチマーク・テスト
**
**   (C) Copyright 1991,1993 by Kazumasa (和正)
**                  Produced by MSPO 
**                              開発部 "CodeTips" Project Fax:06-722-0111
**
*/

#include <assert.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>    // getch() 
#include <time.h>     // time() 

#include "NewDely.h"
NEWDELY_INSTANCE          /* NewDely インスタンス */


// エラーハンドル・ファンクション
void  ErrorExit( char bLevel )
{
	puts("-WAIT-"); getch();
	exit( bLevel );
}


// エラーハンドラ 
static int errHandler( int iNo )
{
	switch( iNo )
	{
		case 0: puts("致命的エラーです");
				break;
		case 1: puts("ヒープメモリ・オーバーです");
				break;
	}
	return(0);  /* 強制終了 */
}


int Random( int num )
{
	long la = long(rand()) * num;
	int ret = int( la / (long(RAND_MAX)+1L) );
	return ret < 0 ? -ret : ret;
}


//----------------------------------------------[ NorSet ]
class FAR NorSet
{
protected:
	int   iNo;
public:
	virtual int GetNo() = 0;
};

class NorSet1 : public NorSet 
{
	char  cDummy[24];
public:
	NorSet1( int i ){ iNo = i; }
	int GetNo(){ return iNo; }
};

class NorSet2 : public NorSet 
{
	char  cDummy2[128];
public:
	NorSet2( int i ){ iNo = i; }
	int GetNo(){ return iNo; }
};


//----------------------------------------------[ NDSet ]
class NDSet : public NewDely      // NewDely より派生させる 
{
protected:
	int   iNo;
public:
	virtual int GetNo() = 0;
};

class NDSet1 : public NDSet
{
	char  cDummy[24];
public:
	NDSet1( int i ){ iNo = i; }
	int GetNo(){ return iNo; }
};

class NDSet2 : public NDSet
{
	char  cDummy2[128];
public:
	NDSet2( int i ){ iNo = i; }
	int GetNo(){ return iNo; }
};


//----------------------------------------------[ const ]
const int  MAX_POINTER = 2000;
const int  MAX_TEST    = 2500;
const int  RAP_TIMES   =    5;   /* 途中経過計測回数 */


NorSet*  NorPointers[ MAX_POINTER ];
NDSet*   NDPointers[ MAX_POINTER ];


//==============================================
int  main()
{
	// TITLE
	printf("class NewDely for DOS [ Ver:%.2f ]  - BENCH MARK TEST - \n",
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
	
	printf("  sizeof(NDSet1)=%u. sizeof(NDSet2)=%u.\n",
							sizeof(NDSet1), sizeof(NDSet2) );
	
	
//===================================================================
	
	printf("\no Empty loop begin.\n");
	
	int  i,j,k, iLap;
	double  dEmpty[RAP_TIMES+1];
	time_t tmStart = time(NULL);
	for( iLap=0, i=MAX_TEST-1 ; 0<=i ; i-- ){
		printf(" %4d\r", i);
		for( j=0 ; j<MAX_POINTER ; j++ ){
			if( NDPointers[ k = Random(MAX_POINTER) ] ){
				if( k%2==0 ) NDPointers[k] = (NDSet*)0;
				else         NDPointers[k] = (NDSet*)0;
			}
			else {
				if( k%2==0 ) NDPointers[k] = (NDSet*)!0;
				else         NDPointers[k] = (NDSet*)!0;
				assert( NDPointers[k] != NULL );
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
	
	// Clear All 
	for( k=0 ; k<MAX_POINTER ; k++ )
	{
		NorPointers[k] = NULL;
		NDPointers[k]  = NULL;
	}
	
//===================================================================
	
	printf("\no Normal new/delete begin.\n");
	
	double  dNorTime[RAP_TIMES+1];
	tmStart = time(NULL);
	for( iLap=0, i=MAX_TEST-1 ; 0<=i ; i-- ){
		printf(" %4d\r", i);
		for( j=0 ; j<MAX_POINTER ; j++ ){
			if( NorPointers[ k = Random(MAX_POINTER) ] ){
				if( NorPointers[k]->GetNo() != k ){
					puts("err: Diffarent No !");
					ErrorExit( 1 );
				}
				delete NorPointers[k];
				NorPointers[k] = NULL;
			}
			else {
				if( k%2==0 ) NorPointers[k] = new NorSet1(k);
				else         NorPointers[k] = new NorSet2(k);
				assert( NorPointers[k] != NULL );
			}
		}
		if( i % (MAX_TEST/RAP_TIMES) == 0 ){
			dNorTime[iLap] = difftime( time(NULL), tmStart );
			printf("   %d. [%4d] LapTime is %8.1lf sec.\n",
									iLap, (-i+MAX_TEST), dNorTime[iLap] );
                                       /* ^^^^^^^^^^^^^Borland C Bug Fix. */
			iLap++;
		}
	}
	
	// Delete All 
	for( k=0 ; k<MAX_POINTER ; k++ )
	{
		if( NorPointers[ k ] )
		{
			delete NorPointers[k];
			NorPointers[k] = NULL;
		}
	}
	
//===================================================================
	
	// ＮｅｗＤｅｌｙ インスタンス生成 
	NEWDELY_INITIAL( errHandler );
	
	printf("\no NewDely new/delete begin.\n");
	
	double  d, dNDTime[RAP_TIMES+1];
	tmStart = time(NULL);
	for( iLap=0, i=MAX_TEST-1 ; 0<=i ; i-- ){
		printf(" %4d\r", i);
		for( j=0 ; j<MAX_POINTER ; j++ ){
			if( NDPointers[ k = Random(MAX_POINTER) ] ){
				if( NDPointers[k]->GetNo() != k ){
					puts("err: Diffarent No !");
					ErrorExit( 1 );
				}
				delete NDPointers[k];
				NDPointers[k] = NULL;
			}
			else {
				if( k%2==0 ) NDPointers[k] = new NDSet1(k);
				else         NDPointers[k] = new NDSet2(k);
				assert( NDPointers[k] != NULL );
			}
		}
		if( i % (MAX_TEST/RAP_TIMES) == 0 ){
			dNDTime[iLap] = difftime( time(NULL), tmStart );
			if( dNorTime[iLap] != dEmpty[iLap] ){
				d = 100.0 * ((dNorTime[iLap]-dEmpty[iLap])
				  - (dNDTime[iLap]-dEmpty[iLap])) / (dNorTime[iLap]-dEmpty[iLap]);
			} else  d = 0.0;
			printf("   %d. [%4d] LapTime is %8.1lf sec. (%6.2lf%%)",
									iLap, (-i+MAX_TEST), dNDTime[iLap], d);
                                       /* ^^^^^^^^^^^^^Borland C Bug Fix. */
			if( 30.0 <= d ) printf(" Great !");
			printf("\n");
			iLap++;
		}
	}
	
	// Delete All 
	for( k=0 ; k<MAX_POINTER ; k++ )
	{
		if( NDPointers[ k ] )
		{
			delete NDPointers[k];
			NDPointers[k] = NULL;
		}
	}
	
	NEWDELY_CHECK(1);  /* リーク */
	
//===================================================================
	
	//
	// Terminate 
	//
	return(0);
}


