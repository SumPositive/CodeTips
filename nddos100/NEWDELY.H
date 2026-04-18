#ifndef _INC_NEWDELY
#define _INC_NEWDELY
/*
**  "NewDely.h"
**
**  ｃｌａｓｓ　ＮｅｗＤｅｌｙ　ｆｏｒ　ＤＯＳ
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**
**   (C) Copyright 1991,1993 by Kazumasa (和正)
**                  Produced by MSPO
**                              開発部 "CodeTips" Project Fax:06-722-0111
**
*/

// NewDelyDos  Version & Release Number. 
const float  NewDelyDos_VERSION = 1.00;


#ifdef _MSC_VER
    #define FAR     __far
#endif
#ifdef __BORLANDC__
    #define FAR       far
#endif

#ifndef TRUE
#define TRUE     1
#endif

#ifndef FALSE
#define FALSE    0
#endif

#ifndef BOOL
typedef  unsigned      BOOL;
#endif

#ifndef WORD
typedef  unsigned int  WORD;
#endif

#ifndef DWORD
typedef  unsigned long  DWORD;
#endif

#ifndef LPVOID
typedef void FAR*      LPVOID;
#endif



//===================================================================
// ＮｅｗＤｅｌｙ 初期化クラス
//===================================================================
#ifdef _AP_DEBUG
    class D_NewDely_Init
    {
        void _ndInit( int (*)(int) );
    public:
        // コンストラクタ
        D_NewDely_Init(){ _ndInit(NULL); }
        D_NewDely_Init( int (*f)(int) ){ _ndInit(f); }
        // ディストラクタ
        ~D_NewDely_Init();
        // Check
        int check( int, char*, int );
    };
    extern D_NewDely_Init*      gpNewDelyInstance;
    #define NEWDELY_INSTANCE    D_NewDely_Init*  gpNewDelyInstance = NULL;
    #define NEWDELY_INITIAL(f)  D_NewDely_Init  NewDelyInstance(f);\
                                gpNewDelyInstance = &NewDelyInstance;
    #define NEWDELY_CHECK(a)    gpNewDelyInstance->check(a,__FILE__,__LINE__)
#else
    class R_NewDely_Init
    {
        void _ndInit( int (*)(int) );
    public:
        // コンストラクタ
        R_NewDely_Init(){ _ndInit(NULL); }
        R_NewDely_Init( int (*f)(int) ){ _ndInit(f); }
        // ディストラクタ
        ~R_NewDely_Init();
    };
    extern R_NewDely_Init*      gpNewDelyInstance;
    #define NEWDELY_INSTANCE    R_NewDely_Init*  gpNewDelyInstance = NULL;
    #define NEWDELY_INITIAL(f)  R_NewDely_Init  NewDelyInstance(f);\
                                gpNewDelyInstance = &NewDelyInstance;
    #define NEWDELY_CHECK(a) 
#endif


//===================================================================
// ＮｅｗＤｅｌｙ 基本クラス
//===================================================================
#ifdef _AP_DEBUG
    class FAR D_NewDely
    {
    public:
        // New 
        LPVOID operator new( size_t );
        // Delete 
        void  operator delete( LPVOID );
        // Size
        size_t  size( void );
    };
    #define NewDely   D_NewDely
#else
    class FAR R_NewDely
    {
    public:
        // New 
        LPVOID operator new( size_t );
        // Delete 
        void  operator delete( LPVOID );
        // Size
        size_t  size( void );
    };
    #define NewDely   R_NewDely
#endif



#endif
