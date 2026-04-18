#
# サンプルＭＡＫＥＦＩＬＥ
# make.MS                         Microsoft NMAKE 仕様
#
# その他の MAKE を御使用の場合など、これを参考にして作成して下さい。
#
# class NewDely for DOS
#
# 使用法： NMAKE make.MS
#
# 環境変数： INCLUDE = インクルードファイル・パス
#                LIB = ライブラリファイル・パス
#
#--------------------------------------------------------------------
# MODEL & TYPE:

TYPE=F    # Compile Type    [ Pro | Ama | Free ]
MODEL=S   # Compile Model   [ Large | Compact | Medium | Small ]
DEBUG=0   # Debug [0]OFF  [1]ON 

#--------------------------------------------------------------------
# Compiler Option:

CFLAGS=/c /A$(MODEL) /W3 /Ze /D_DOS 
LFLAGS=/NOI /E /ST:2048 

LIBS=$(MODEL)$(TYPE)_NDD.lib 

!if $(DEBUG)==1
!message [--- apprication Debug ---]
CFLAGS=$(CFLAGS) /D_AP_DEBUG /Od 
LFLAGS=$(LFLAGS)
!else
!message [--- apprication Release ---]
CFLAGS=$(CFLAGS) /DNDEBUG /Ox /Gs
LFLAGS=$(LFLAGS)
!endif


#--------------------------------------------------------------------
# Command:

all:  nd_max.exe  nd_bench.exe  nd_mix.exe


# --------------------------------------------------
.cpp.exe:
	cl $(CFLAGS)  $*.cpp
	link $(LFLAGS) $*,$*,NUL,$(LIBS);

nd_bench.exe:  nd_bench.cpp

nd_max.exe:  nd_max.cpp

nd_mix.exe:  nd_mix.cpp


# --- end ---
