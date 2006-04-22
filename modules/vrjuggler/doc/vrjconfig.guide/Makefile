# $Id$

default: html

docs: html chunk-html pdf
install-docs: install-html install-chunk-html install-pdf

docdir=		../../../../doc
NAME=		vrjconfig.guide
XML_FILES=	$(NAME).xml

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	vrjuggler/$(PROJECT_NUMBER)/vrjconfig.guide

prefix=		$(webroot)/$(instdir)
#INSTALL_DIRS=	figures
INSTALL_FILES=	$(docdir)/base_style.css

include ../version.mk
include $(docdir)/docbook.mk
