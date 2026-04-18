#ifndef _INC_VM_DOS
#define _INC_VM_DOS
/*
**  "vm_Dos.h"
**
**  ＶＭＭ＿Ｄｏｓ : Varturl Memory Manager for DOS : 仮想メモリ・マネージャ
**  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
**
**   (C) Copyright 1989,1993 by Kazumasa (和正)
**                  Produced by MSPO
**                              "CodeTips" Project Fax:06-722-0111
**
*/

/*
**  VM_DOS Version & Release
*/
#define VM_DOS_VERSION   1.33   /* Version 1.3.3 */


#define  DOS_PARAGRAPH  16   /* パラグラフ単位 (byte) */

#ifndef TRUE
#define TRUE     1
#endif

#ifndef FALSE
#define FALSE    0
#endif

#ifndef WORD
typedef  unsigned int  WORD;
#endif

#ifndef DWORD
typedef  unsigned long  DWORD;
#endif

#ifdef _MSC_VER
	#if (_MSC_VER < 600)
		#ifndef void
			#define void   unsigned char
		#endif
	#endif
#endif

#if (_MSC_VER >= 700)
    #define FAR     __far
#else
    #define FAR       far
#endif

/*
**  VMM HANDLE
*/
typedef unsigned long VMHAND;
#define VM_NULL   ((VMHAND)0)

/*
**  vm_init:
*/
#define VM_ALLDOS   (0)
#define VM_XMS       0x0001
#define VM_EMS       0x0002
#define VM_DISK      0x0004
#define VM_ALLSWAP  (VM_XMS|VM_EMS|VM_DISK)

/*
**  vm_lock, vm_unlock, vm_unlockAll:
*/
#define VM_CLEAN    0
#define VM_DIRTY    1


#ifdef _AP_DEBUG
	#define vm_alloc(w)			Dvm_alloc(w,__FILE__,__LINE__)
	#define vm_allocFix(w)		Dvm_allocFix(w,__FILE__,__LINE__)
	#define vm_check(i)			Dvm_check(i,__FILE__,__LINE__)
	#define vm_compaction()		Dvm_compaction(__FILE__,__LINE__)
	#define vm_free(h)			Dvm_free(h,__FILE__,__LINE__)
	#define vm_freeAll()		Dvm_freeAll(__FILE__,__LINE__)
	#define vm_freeQuick(h)		Dvm_freeQuick(h,__FILE__,__LINE__)
	#define vm_init(a,b,c,d,e)	Dvm_init(a,b,c,d,e,__FILE__,__LINE__)
	#define vm_load(h,f)		Dvm_load(h,f,__FILE__,__LINE__)
	#define vm_lock(h)			Dvm_lock(h,__FILE__,__LINE__)
	#define vm_realloc(h,w)		Dvm_realloc(h,w,__FILE__,__LINE__)
	#define vm_size(h)			Dvm_size(h,__FILE__,__LINE__)
	#define vm_term()			Dvm_term(__FILE__,__LINE__)
	#define vm_unlock(h,f)		Dvm_unlock(h,f,__FILE__,__LINE__)
	#define vm_unlockAll(f)		Dvm_unlockAll(f,__FILE__,__LINE__)
#else
	#define vm_alloc(w)			Rvm_alloc(w)
	#define vm_allocFix(w)		Rvm_allocFix(w)
	#define vm_compaction()		Rvm_compaction()
	#define vm_check(i)			FALSE
	#define vm_free(h)			Rvm_free(h)
	#define vm_freeAll()		Rvm_freeAll()
	#define vm_freeQuick(h)		Rvm_freeQuick(h)
	#define vm_init(a,b,c,d,e)	Rvm_init(a,b,c,d,e)
	#define vm_load(h,f)		Rvm_load(h,f)
	#define vm_lock(h)			Rvm_lock(h)
	#define vm_realloc(h,w)		Rvm_realloc(h,w)
	#define vm_size(h)			Rvm_size(h)
	#define vm_term()			Rvm_term()
	#define vm_unlock(h,f)		Rvm_unlock(h,f)
	#define vm_unlockAll(f)		Rvm_unlockAll(f)
#endif

/*
**  プロトタイプ宣言
*/
#ifdef _AP_DEBUG
	extern VMHAND Dvm_alloc(WORD,char*,int);
	extern VMHAND Dvm_allocFix(WORD,char*,int);
	extern void   Dvm_compaction(char*,int);
	extern int    Dvm_check(int,char*,int);
	extern void   Dvm_free(VMHAND,char*,int);
	extern void   Dvm_freeAll(char*,int);
	extern void   Dvm_freeQuick(VMHAND,char*,int);
	extern int    Dvm_init(WORD,WORD,WORD,char,int (*)(int,char*),char*,int);
	extern void  *Dvm_load(VMHAND,int,char*,int);
	extern void  *Dvm_lock(VMHAND,char*,int);
	extern VMHAND Dvm_realloc(VMHAND,WORD,char*,int);
	extern WORD   Dvm_size(VMHAND,char*,int);
	extern void   Dvm_term(char*,int);
	extern void   Dvm_unlock(VMHAND,int,char*,int);
	extern void   Dvm_unlockAll(int,char*,int);
#else
	extern VMHAND Rvm_alloc(WORD);
	extern VMHAND Rvm_allocFix(WORD);
	extern void   Rvm_compaction(void);
	extern void   Rvm_free(VMHAND);
	extern void   Rvm_freeAll(void);
	extern void   Rvm_freeQuick(VMHAND);
	extern int    Rvm_init(WORD,WORD,WORD,char,int (*)(int,char*));
	extern void  *Rvm_load(VMHAND, int);
	extern void  *Rvm_lock(VMHAND);
	extern VMHAND Rvm_realloc(VMHAND,WORD);
	extern WORD   Rvm_size(VMHAND);
	extern void   Rvm_term(void);
	extern void   Rvm_unlock(VMHAND, int);
	extern void   Rvm_unlockAll(int);
#endif


#if (_MSC_VER >= 500) || defined(__BORLANDC__) || defined(__TURBOC__)
	extern void Fmemset( void FAR *, int, unsigned );
	extern void Fmemcpy( void FAR *, const void FAR *, unsigned );
	extern void Fmemmove( void FAR *, const void FAR *, unsigned );
#else
	extern void Fmemset( char FAR *, int, unsigned );
	extern void Fmemcpy( char FAR *, char FAR *, unsigned );
	extern void Fmemmove( char FAR *, char FAR *, unsigned );
#endif


#endif
