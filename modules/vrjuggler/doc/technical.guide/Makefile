# $Id$

default: pdf

# Define to use OpenJade to render PDF document.
#USE_JADEPROC=1

#XSLT_TOOL=	Xalan
XSLT_TOOL=	Saxon

# TURN off comments
EXTRA_XALAN_FO_PARAMS=		-PARAM show.comments "0"
EXTRA_XALAN_HTML_PARAMS=	-PARAM show.comments "0"
EXTRA_SAXON_FO_PARAMS=		show.comments=0
EXTRA_SAXON_HTML_PARAMS=	show.comments=0

SVG_IMAGES=	figures/AppPulloutCode.svg			\
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
		figures/VjAppHierarchy-PfOgl.svg		\
		figures/VP_APP_sepLine.svg

IMAGES=		$(SVG_IMAGES:%.svg=%.png)

NAME=		technical-overview
MK_PATH=	../../../../doc

#########################################################

TXT_FILES=    $(NAME).txt
HTML_FILES=   $(NAME).html
PDF_FILES=    $(NAME).pdf

$(NAME).txt:  $(NAME).xml $(IMAGES) # README.fo
$(NAME).html: $(NAME).xml $(IMAGES)
$(NAME).pdf:  $(NAME).xml $(NAME).fo $(IMAGES)

include $(MK_PATH)/docbook.mk
