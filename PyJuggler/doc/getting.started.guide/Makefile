# $Id$

default: html

docs: html chunk-html pdf
install-docs: install-html install-chunk-html install-pdf

NAME=		getting.started.guide

XSLT_TOOL=	Saxon

XML_FILES=	$(NAME).xml
HTML_FILES=	$(NAME).html
PDF_FILES=	$(NAME).pdf

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	pyjuggler/$(PROJECT_NUMBER)/getting.started.guide

prefix=		$(webroot)/$(instdir)
INSTALL_FILES=	$(webroot)/base_style.css

$(NAME).html: $(NAME).xml
$(NAME).pdf: $(NAME).xml $(NAME).fo

include ../version.mk
include ../docbook.mk
