# $Id$

default: html

docs: html chunk-html pdf
install-docs: install-html install-chunk-html install-pdf

docdir=		../../../../doc
NAME=		getting.started.guide
XML_FILES=	$(NAME).xml

XSLT_TOOL=	Saxon

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	vrjuggler/$(PROJECT_NUMBER)/getting.started.guide

prefix=		$(webroot)/$(instdir)
INSTALL_DIRS=	figures
INSTALL_FILES=	$(docdir)/base_style.css

include ../version.mk
include $(docdir)/docbook.mk
