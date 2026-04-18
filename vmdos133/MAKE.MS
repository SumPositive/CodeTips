#
# サンプルＭＡＫＥＦＩＬＥ
# make.MS                         Microsoft NMAKE 仕様
#
# その他の MAKE を御使用の場合など、これを参考にして作成して下さい。
#
# VMM_DOS 
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

CFLAGS=/c /A$(MODEL) /W2 /Ze /D_DOS 
LFLAGS=/NOI /E /ST:2048 

LIBS=$(MODEL)$(TYPE)_VMM.lib 

!if $(DEBUG)==1
!message [--- apprication Debug ---]
CFLAGS=$(CFLAGS) /D_AP_DEBUG /Od 
LFLAGS=$(LFLAGS)
!else
!message [--- apprication Release ---]
CFLAGS=$(CFLAGS) /DNDEBUG /Ox
LFLAGS=$(LFLAGS)
!endif


#--------------------------------------------------------------------
# Command:

all:  vm_rand.exe  vm_max.exe  vm_bench.exe  vm_mix.exe


# --------------------------------------------------
.c.exe:
	cl $(CFLAGS)  $*.c
	link $(LFLAGS) $*,$*,NUL,$(LIBS);

vm_rand.exe:  vm_rand.c

vm_bench.exe:  vm_bench.c

vm_max.exe:  vm_max.c

vm_mix.exe:  vm_mix.c


# --- end ---
