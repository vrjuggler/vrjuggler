# $Id$

default: html

docs: html chunk-html pdf
install-docs: install-html install-chunk-html install-pdf

NAME=		technical-overview
XML_FILES=    $(NAME).xml

# Define to use OpenJade to render PDF document.
#USE_JADEPROC=1

#XSLT_TOOL=	Xalan
XSLT_TOOL=	Saxon

# TURN off comments
EXTRA_XALAN_FO_PARAMS=		-PARAM show.comments "0"
EXTRA_XALAN_HTML_PARAMS=	-PARAM show.comments "0"
EXTRA_SAXON_FO_PARAMS=		show.comments=0
EXTRA_SAXON_HTML_PARAMS=	show.comments=0

SVG_IMAGES=	figures/AppHierarchy.svg			\
		figures/AppPulloutCode.svg        		\
		figures/ConfigChunkHandlerHierarchy.svg		\
		figures/ConfigManagerStructure.svg		\
		figures/DependencyCheckingClasses.svg		\
		figures/DeviceFactory.svg			\
		figures/DisplayManagerClasses.svg		\
		figures/FullKernelControlLoop.svg		\
		figures/InputDeviceTypeHierarchy.svg		\
		figures/Kernel_Loop_Call_Sequence.svg		\
		figures/MicrokernelClassDiagram.svg		\
		figures/RuntimeConfigurationSequence.svg	\
		figures/userOglApp_hierarchy.svg		\
		figures/VP_APP_sepLine.svg

IMAGES=		$(SVG_IMAGES:%.svg=%.png)

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	vrjuggler/$(PROJECT_NUMBER)/technical.guide

prefix=		$(webroot)/$(instdir)
INSTALL_DIRS=	figures
INSTALL_FILES=	$(webroot)/base_style.css

$(NAME).txt: $(IMAGES)
$(NAME).html: $(IMAGES)
$(NAME)/index.html: $(IMAGES)
$(NAME).pdf: $(IMAGES)

include ../version.mk
include ../../../../doc/docbook.mk
