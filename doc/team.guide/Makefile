# $Id$

default: html

docs: html chunk-html pdf
install-docs: install-html install-chunk-html install-pdf

NAME=		team.guide

XML_FILES=	$(NAME).xml
HTML_FILES=	$(NAME).html
PDF_FILES=	$(NAME).pdf

XSLT_TOOL=	Saxon

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	docs/juggler.team.guide

prefix=		$(webroot)/$(instdir)
INSTALL_FILES=	$(webroot)/base_style.css

NEED_DB_IMAGES=	1

$(NAME).html: $(NAME).xml
$(NAME).pdf: $(NAME).xml $(NAME).fo

include ../docbook.mk
