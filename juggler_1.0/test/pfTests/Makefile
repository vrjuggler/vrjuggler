#!smake -J 1
#-------------------------------------------------------------------#
#-- Makefile for Performer/src/pguide/libpf/C++ directory         --#
#-------------------------------------------------------------------#
#-- RCS version information                                       --#
#--   $Revision$                                           --#
#--   $Date$                                --#
#-------------------------------------------------------------------#

PFROOT ?= $(ROOT)

# needed for motif.c
PFDOMOTIF ?= 1
#if $(PFDOMOTIF)
NEED_MOTIF_LIBS = 1
#endif

# Needed for video program (siriusvtex.c dmvtex.c)
PFDOVIDEO ?= 1
#if $(PFDOVIDEO)
NEED_VIDEO_LIBS = 1
#endif

include $(PFROOT)/usr/share/Performer/src/pfmakedefs

# C++ programs must use CC as linker
LINKERF=$(CXXF)

.BEGIN:
	@if [ $(PFDOMOTIF) = "1" ]; then \
	echo "Linking with motif libraries. setenv PFDOMOTIF 0 to turn off."; \
	else \
	echo "Not linking with motif libraries can't build motif.c. setenv PFDOMOTIF 1 to enable."; \
	fi

#-- targets are the executables
TARGETS	= \
	multichan \
	multipipe \
	multiwin

OBJECTS = $(TARGETS:=.o)


include $(PFROOT)/usr/share/Performer/src/pfmakerules
