# $Id$

default: html

NAME=		porting.guide

XML_FILES=	$(NAME).xml
HTML_FILES=	$(NAME).html
PDF_FILES=	$(NAME).pdf

$(NAME).html: $(NAME).xml
$(NAME).pdf: $(NAME).xml $(NAME).fo

include ../../../../docs/docbook.mk
