# $Id$

default: txt

NAME=		README

TXT_FILES=	$(NAME).txt
HTML_FILES=	$(NAME).html
PDF_FILES=	$(NAME).pdf

$(NAME).txt: $(NAME).xml $(NAME).html # $(NAME).fo
$(NAME).html: $(NAME).xml
$(NAME).pdf: $(NAME).xml $(NAME).fo # $(NAME).tex

CLOBBER_FILES=	$(TXT_FILES)

include docbook.mk
