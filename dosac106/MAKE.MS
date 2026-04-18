#
# サンプルＭＡＫＥＦＩＬＥ
# make.MS                         Microsoft NMAKE 仕様
#
# その他の MAKE を御使用の場合など、これを参考にして作成して下さい。
#
# DosAlloc
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
DEBUG=1   # Debug [0]OFF  [1]ON 

#--------------------------------------------------------------------
# Compiler Option:

CFLAGS=/c /A$(MODEL) /W3 /Ze /D_DOS 
LFLAGS=/NOI /E /ST:2048 

!if "$(TYPE)"=="P"
LIBS=$(MODEL)P_DAC.lib     #（製品開発版）
!else
LIBS=$(MODEL)A_DAC.lib     #（個人学習版）
!endif

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

all:  da_rand.exe  da_max.exe  da_bench.exe  da_mix.exe


# --------------------------------------------------
.c.exe:
	cl $(CFLAGS)  $*.c
	link $(LFLAGS) $*,$*,NUL,$(LIBS);

da_rand.exe:  da_rand.c

da_bench.exe:  da_bench.c

da_max.exe:  da_max.c

da_mix.exe:  da_mix.c


# --- end ---
