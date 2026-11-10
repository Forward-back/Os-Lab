NAME := kernel
SRCS := kernel.c
export MODULE  := L0
export AM_HOME := $(PWD)/../abstract-machine
ifeq ($(ARCH),)
export ARCH := x86_64-qemu
endif

log:
	make -nB \
		| grep -ve '^\(\#\|echo\|mkdir\|make\)' \
		| sed "s#$(AM_HOME)#\AM#g" \
		| sed "s#$(PWD)#.#g" \
		> $(BUILD_DIR)/compile.log

# Ubuntu 24.04 (GCC 13) 对 AM x86 平台代码误报 -Warray-bounds，
# 且框架的 -Werror 会将其升级为错误。只放行这一个警告。
CFLAGS += -Wno-array-bounds
export CFLAGS

include $(AM_HOME)/Makefile
include ../oslabs.mk
image: git
