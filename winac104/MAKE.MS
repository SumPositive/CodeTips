#
# サンプルＭＡＫＥＦＩＬＥ
# make.MS                         Microsoft NMAKE 仕様
#
# WinAlloc
#
# その他の MAKE を御使用の場合など、これを参考にして作成して下さい。
#
# 使用法： NMAKE make.MS
#
# 環境変数： INCLUDE = インクルードファイル・パス
#                LIB = ライブラリファイル・パス
#
#--------------------------------------------------------------------
# MODEL & TYPE:

TYPE=A    # Compile Type    [ Pro | Ama ]
MODEL=C   # Compile Model   [ Large | Compact | Medium | Small ]
DEBUG=0   # Debug [0]OFF  [1]ON 

#--------------------------------------------------------------------
# Compiler Option:

CFLAGS=/c /DWINVER=0x0300 /A$(MODEL) /Zp /G2w
LFLAGS=/NOD

LIBS=$(MODEL)$(TYPE)_WAC.lib 

!if $(DEBUG)==1
!message [--- apprication Debug ---]
CFLAGS=$(CFLAGS) /D_AP_DEBUG /Od 
LFLAGS=$(LFLAGS)
!elseif "$(TYPE)"=="R"
!message [--- apprication Release ---]
CFLAGS=$(CFLAGS) /DNDEBUG /Otelg /Gs
LFLAGS=$(LFLAGS)
!endif

LIBS=$(LIBS) libw $(MODEL)libcew

OBJS=wAlloc.obj wPuts.obj wDebug1.obj wDebug2.obj wDebug3.obj \
		wBench1.obj wBench2.obj wBench3.obj

#--------------------------------------------------------------------
# Command:

all:    wAlloc.exe  


# --------------------------------------------------
wAlloc.exe:  $(OBJS) wAlloc.def wAlloc.res
	link @<<
	$(LFLAGS) $(OBJS),
	wAlloc.exe,
	NUL,
	$(LIBS),
	wAlloc.def;
<<
	rc -T  wAlloc.res


# --------------------------------------------------
.c.obj:
	cl $(CFLAGS)  $*.c

wAlloc.obj:  wAlloc.c  WinAlloc.h

wPuts.obj:  wPuts.c  WinAlloc.h

wDebug1.obj:  wDebug1.c  WinAlloc.h

wDebug2.obj:  wDebug2.c  WinAlloc.h

wDebug3.obj:  wDebug3.c  WinAlloc.h

wBench1.obj:  wBench1.c  WinAlloc.h

wBench2.obj:  wBench2.c  WinAlloc.h

wBench3.obj:  wBench3.c  WinAlloc.h


# --------------------------------------------------
.rc.res:
    rc -r $*.rc

wAlloc.res:  wAlloc.rc wAlloc.h


# --- end ---
