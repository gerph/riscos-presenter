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

TARGET ?= present

VPATH = . md4c/src

ALL_TARGETS =	test_border \
				test_colours \
				test_gcontext \
				test_font \
				test_screen \
				test_textblock \
				test_fontfamily \
				test_mdparse \
				test_slidedeck \
				test_slidemd \
				test_sliderender \
				test_image \
				present

OMIT =			\

targetted:
	make ${TARGET},ff8 TARGET=${TARGET}

all:
	for i in ${ALL_TARGETS} ; do make $$i,ff8 TARGET=$$i || exit $$? ; done

clean:
	-rm -f *.o *.bin *,ff8 *.map

CLIBDIR = /Users/charles/projects/RO/riscos64-simple-binaries/clib/export

CC = RISCOS64_RELDIRS=1 ${CLIBDIR}/bin/robuild64 riscos64-cc
AS = RISCOS64_RELDIRS=1 ${CLIBDIR}/bin/robuild64 riscos64-as
LD = RISCOS64_RELDIRS=1 ${CLIBDIR}/bin/robuild64 riscos64-link

# md4c build setup
CFLAGS += -Imd4c/src/

MD_LIB = 	md4c.o \
			entity.o

MD_LIB_UNUSED = \
			md4c-html.o \


OBJS =	${TARGET}.o

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
        fontmapif.o \
        fontfamily.o

endif
ifeq (${TARGET},test_mdparse)
OBJS += ${MD_LIB} \
		mdnames.o \

endif
ifeq (${TARGET},test_slidemd)
OBJS += ${MD_LIB} \
		optlist.o \
		slidedeck.o \
		slidedeck_debug.o \
		slidemd.o \
		file.o \
		mdnames.o

endif

ifeq (${TARGET},test_slidedeck)
OBJS += ${MD_LIB} \
		optlist.o \
		slidedeck.o \
		slidedeck_debug.o \

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
ifeq (${TARGET},test_image)
OBJS += ${MD_LIB} \
		font.o \
		vdu5.o \
		vdushape.o \
		gcontext.o \
		image.o \
		file.o \

endif

ifeq (${TARGET},present)
OBJS += ${MD_LIB} \
			font.o \
			vdu5.o \
			vdushape.o \
			gcontext.o \
			textblock.o \
			fontfamily.o \
			fontmapif.o \
			optlist.o \
			slidedeck.o \
			slidedeck_index.o \
			slidedeck_debug.o \
			position.o \
			slidemd.o \
			file.o \
			sliderender.o \
			colours.o \
			screen.o \
			border.o \
			image.o \
			mdnames.o \
			cursors.o \
			input.o \
			hourglass.o \
			banks.o \

endif


%.o: %.c
	${CC} ${CFLAGS} -c -o $@ $?

%.o: md4c/src/%.c
	${CC} ${CFLAGS} -c -o $@ $?

%.o: %.s
	${AS} ${AFLAGS} -o $@ $?

${TARGET},ff8: ${OBJS}
	${LD} -aif ${OBJS} -lcrt -o $@
