# $Id$

default: html

docs: html chunk-html pdf
install-docs: install-html install-chunk-html install-pdf

XSLT_TOOL=	Saxon

NAME=		clusterjuggler.guide

XML_FILES=	$(NAME).xml
HTML_FILES=	$(NAME).html
PDF_FILES=	$(NAME).pdf

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	gadgeteer/$(PROJECT_NUMBER)/clusterjuggler.guide

prefix=		$(webroot)/$(instdir)
INSTALL_DIRS=	figures
INSTALL_FILES=	$(webroot)/base_style.css

NEED_DB_IMAGES=	1

$(NAME).html: $(NAME).xml
$(NAME).pdf: $(NAME).xml $(NAME).fo

include ../version.mk
include ../../../../doc/docbook.mk
