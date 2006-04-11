# $Id$

default: html

docs: html pdf
install-docs: install-html install-pdf

XSLT_TOOL=      Saxon

NAME=		porting.guide

XML_FILES=	$(NAME).xml
HTML_FILES=	$(NAME).html
PDF_FILES=	$(NAME).pdf

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	vrjuggler/$(PROJECT_NUMBER)/migration.guide

prefix=		$(webroot)/$(instdir)
#INSTALL_DIRS=	figures
INSTALL_FILES=	$(webroot)/base_style.css

$(NAME).html: $(NAME).xml
$(NAME).pdf: $(NAME).xml $(NAME).fo

include ../version.mk
include ../../../../doc/docbook.mk
