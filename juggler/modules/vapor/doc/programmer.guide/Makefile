# $Id$

default: html

docs: html chunk-html pdf
install-docs: install-html install-chunk-html install-pdf

NAME=		programmer.guide

XSLT_TOOL=	Saxon

XML_FILES=	$(NAME).xml
HTML_FILES=	$(NAME).html
PDF_FILES=	$(NAME).pdf

#SVG_IMAGES=	
#IMAGES=		$(SVG_IMAGES:%.svg=%.png)

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	vapor/$(PROJECT_NUMBER)/programmer.guide

prefix=		$(webroot)/$(instdir)
#INSTALL_DIRS=	figures
INSTALL_FILES=	$(webroot)/base_style.css

#NEED_DB_IMAGES=	1

$(NAME).html: $(NAME).xml $(IMAGES)
$(NAME).pdf: $(NAME).xml $(NAME).fo $(IMAGES)

CLOBBER_FILES=	$(IMAGES)

include ../version.mk
include ../../../../doc/docbook.mk
