# Makefile for building bin for aarch64
#
# Code compiles with:
#
#	__riscos
#	__riscos64
#   __aarch64__
#
# Norcroft defines not set:
#	__acorn
#	__arm

TARGET = present64

USE_FUNC_SIGNATURE ?= 1

DIS = ~/projects/RO/pyromaniac/utils/riscos-dumpi --arm64
CROSS_ROOT = ${shell echo $$CROSS_ROOT}

DEFAULT_GOAL = ${TARGET},ff8

# Remove the flags that might make code think it's compiling for linux system.
CFLAGS = -U__linux -U__linux__ -U__unix__ -U__unix -Ulinux -Uunix -U__gnu_linux__

# Add the definitions to indicate that we're compiling for RISC OS
CFLAGS += -D__riscos -D__riscos64

# Allow us to build without assuming the standard library is present
CFLAGS += -nostdlib -ffreestanding -march=armv8-a+nofp

# Options to allow function signatures to appear RISC OS-like
ifeq (${USE_FUNC_SIGNATURE},1)
CFLAGS += -fpatchable-function-entry=20,20
endif

# Optimisation options
CFLAGS += -O1

# Options for this build
CFLAGS += 

all: ${DEFAULT_GOAL}

shell: dockcross-linux-arm64
	./dockcross-linux-arm64 bash

dockcross-linux-arm64:
	docker run --rm dockcross/linux-arm64:20240529-0dade71 > dockcross-linux-arm64
	chmod +x dockcross-linux-arm64

clean:
	-rm -f *.o *.bin *,ff8 *.map

ifeq (${CROSS_ROOT},)
# If we're outside the docker container, re-run ourselves inside the container
ifneq ($(filter-out all shell dockcross-linux-arm64 clean,${MAKECMDGOALS}),)
# The command wasn't one of our invocation commands above
.PHONY: ${MAKECMDGOALS}
${MAKECMDGOALS}: dockcross-linux-arm64
	cd .. ; aarch64/dockcross-linux-arm64 bash -c "cd aarch64 && make ${MAKECMDGOALS} TARGET=${TARGET}"
else
.PHONY: ${DEFAULT_GOAL}
${DEFAULT_GOAL}: dockcross-linux-arm64
	cd .. ; aarch64/dockcross-linux-arm64 bash -c "cd aarch64 && make ${DEFAULT_GOAL} TARGET=${TARGET}"
endif

else

RT_OBJS = 	malloc.o \
			clib.o \
			clib-kernel.o

OBJS = 	${RT_OBJS}

ifeq (${TARGET},border)
OBJS += font.o \
		vdu5.o \
		vdushape.o \
		gcontext.o \
		border.o \
		test_border.o
endif

%.o: %.c
	aarch64-unknown-linux-gnu-gcc ${CFLAGS} -c -o $@ $?

${TARGET}.bin: start.o swis.o link.lnk ${OBJS}
	aarch64-unknown-linux-gnu-ld start.o swis.o ${OBJS} -T link.lnk -o $@

${TARGET}.map: start.o swis.o link.lnk ${OBJS}
	aarch64-unknown-linux-gnu-ld start.o swis.o ${OBJS} -T link.lnk -Map $@ -o /dev/null

ifeq (${USE_FUNC_SIGNATURE},1)
${TARGET},ff8: ${TARGET}.bin ${TARGET}.map
	aarch64-unknown-linux-gnu-objcopy -O binary -j .text ${TARGET}.bin $@
	python riscos_symbols.py ${TARGET}.map ${TARGET},ff8
else
${TARGET},ff8: ${TARGET}.bin
	aarch64-unknown-linux-gnu-objcopy -O binary -j .text ${TARGET}.bin $@
endif

start.o: start.s
	aarch64-unknown-linux-gnu-gcc ${CFLAGS} -c -o $@ $?

endif
