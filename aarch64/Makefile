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

TARGET ?= present64

USE_FUNC_SIGNATURE ?= 1

DIS = ~/projects/RO/pyromaniac/utils/riscos-dumpi --arm64
CROSS_ROOT = ${shell echo $$CROSS_ROOT}

ALL_TARGETS =	test_border \
				test_colours \
				test_gcontext \
				test_font \
				test_screen \
				test_textblock \
				test_fontfamily \
				test_mdparse \
				test_slidemd \
				test_sliderender \

OMIT =			\


# Remove the flags that might make code think it's compiling for linux system.
CFLAGS = -U__linux -U__linux__ -U__unix__ -U__unix -Ulinux -Uunix -U__gnu_linux__

# Add the definitions to indicate that we're compiling for RISC OS
CFLAGS += -D__riscos -D__riscos64

# Allow us to build without assuming the standard library is present
CFLAGS += -nostdlib -ffreestanding -march=armv8-a
#CFLAGS += -nostdlib -ffreestanding -march=armv8-a+nofp

# Add the exports directory to those things we'll build with
CFLAGS += -Iriscos_headers/C/ -Iriscos_headers/Lib/

# Options to allow function signatures to appear RISC OS-like
ifeq (${USE_FUNC_SIGNATURE},1)
CFLAGS += -fpatchable-function-entry=20,20
endif

# Optimisation options
CFLAGS += -O1

# Options for this build
CFLAGS += 

targetted: local_headers
	make ${TARGET},ff8 TARGET=${TARGET}

all: local_headers
	for i in ${ALL_TARGETS} ; do make $$i,ff8 TARGET=$$i || exit $$? ; done

local_headers:
	mkdir -p riscos_headers/C/
	for hdr in ${CEXPORT_DIR}/h/* ; do cp "$$hdr" "riscos_headers/C/$$(basename "$$hdr").h" ; done
	#for hdrdir in ${LIB_DIR}/* ; do mkdir -p "riscos_headers/Lib/$$(basename "$$hdrdir")" ; for hdr in "$$hdrdir/h"/* ; do cp "$$hdr" "riscos_headers/Lib/$$(basename "$$hdrdir")/$$(basename "$$hdr").h" ; done ; done

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
	cd .. ; aarch64/dockcross-linux-arm64 -- bash -c "cd aarch64 && make ${MAKECMDGOALS} TARGET=${TARGET}"
else
.PHONY: ${DEFAULT_GOAL}
${DEFAULT_GOAL}: dockcross-linux-arm64
	cd .. ; aarch64/dockcross-linux-arm64 -- bash -c "cd aarch64 && make TARGET=${TARGET}"
endif

else

RT_OBJS = 	malloc.o \
			clib.o \
			clib-kernel.o \
			strtol.o \
			strtod.o

# md4c build setup
VPATH = md4c/src/riscos/aarch64
CFLAGS += -Imd4c/src/riscos/aarch64

MD_LIB = 	md4c/src/riscos/aarch64/md4c.o \
			md4c/src/riscos/aarch64/entity.o

MD_LIB_UNUSED = \
			md4c/src/riscos/aarch64/md4c-html.o \


OBJS =	${RT_OBJS} \
		${TARGET}.o

ifeq (${TARGET},test_border)
OBJS += font.o \
		vdu5.o \
		vdushape.o \
		gcontext.o \
		border.o

endif
ifeq (${TARGET},test_colours)
OBJS += font.o \
		vdu5.o \
		vdushape.o \
		gcontext.o \
		colours.o

endif
ifeq (${TARGET},test_gcontext)
OBJS += font.o \
		vdu5.o \
		vdushape.o \
		gcontext.o

endif
ifeq (${TARGET},test_font)
OBJS += font.o \
		gcontext.o \
		vdushape.o \
		vdu5.o

endif
ifeq (${TARGET},test_screen)
OBJS +=	screen.o \

endif
ifeq (${TARGET},test_textblock)
OBJS += font.o \
        vdu5.o \
        vdushape.o \
        gcontext.o \
        textblock.o

endif
ifeq (${TARGET},test_fontfamily)
OBJS += font.o \
        vdu5.o \
        vdushape.o \
        gcontext.o \
        textblock.o \
        str.o \
        fontmapif.o \
        fontfamily.o

endif
ifeq (${TARGET},test_mdparse)
OBJS += ${MD_LIB} \
		mdnames.o \

endif
ifeq (${TARGET},test_slidemd)
OBJS += ${MD_LIB} \
		str.o \
		optlist.o \
		slidedeck.o \
		slidedeck_debug.o \
		slidemd.o \
		file.o \
		mdnames.o

endif
ifeq (${TARGET},test_sliderender)
OBJS += ${MD_LIB} \
		font.o \
		vdu5.o \
		vdushape.o \
		gcontext.o \
		textblock.o \
		fontfamily.o \
		fontmapif.o \
		str.o \
		optlist.o \
		slidedeck.o \
		slidedeck_debug.o \
		position.o \
		slidemd.o \
		file.o \
		sliderender.o \
		colours.o \
		screen.o \
		border.o \
		image.o \
		mdnames.o

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
