# $Id$

default: html

docs: html chunk-html pdf
install-docs: install-html install-chunk-html install-pdf

docdir=		../../../../doc
NAME=		programmer.guide

XSLT_TOOL=	Saxon

XML_FILES=	$(NAME).xml

#SVG_IMAGES=	
#IMAGES=		$(SVG_IMAGES:%.svg=%.png)

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	vapor/$(PROJECT_NUMBER)/programmer.guide

prefix=		$(webroot)/$(instdir)
#INSTALL_DIRS=	figures
INSTALL_FILES=	$(docdir)/base_style.css

#NEED_DB_IMAGES=	1

#$(NAME).html: $(IMAGES)
#$(NAME)/index.html: $(IMAGES)
#$(NAME).pdf: $(IMAGES)

#CLOBBER_FILES=	$(IMAGES)

include ../version.mk
include $(docdir)/docbook.mk
