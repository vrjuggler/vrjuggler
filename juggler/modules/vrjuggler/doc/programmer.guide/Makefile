# $Id$

default: html

NAME=		guide

XML_FILES=	$(NAME).xml
HTML_FILES=	$(NAME).html
PDF_FILES=	$(NAME).pdf

SVG_IMAGES=	figures/Kernel_Loop_Call_Sequence.svg		\
		figures/userOglApp_hierarchy.svg		\
		figures/userPfApp_hierarchy.svg
IMAGES=		$(SVG_IMAGES:%.svg=%.png)

$(NAME).html: $(NAME).xml $(IMAGES)
$(NAME).pdf: $(NAME).xml $(NAME).fo $(IMAGES)

include ../../../../docs/docbook.mk
