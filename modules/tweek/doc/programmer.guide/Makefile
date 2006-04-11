# $Id$

default: html

docs: html chunk-html pdf
install-docs: install-html install-chunk-html install-pdf

XML_FILES=	programmer.guide.xml
HTML_FILES=	programmer.guide.html
PDF_FILES=	programmer.guide.pdf

XSLT_TOOL=	Saxon

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	tweek/$(PROJECT_NUMBER)/programmer.guide

prefix=		$(webroot)/$(instdir)
#INSTALL_DIRS=	figures
INSTALL_FILES=	$(webroot)/base_style.css

NEED_DB_IMAGES=	1

programmer.guide.html: programmer.guide.xml
programmer.guide.pdf: programmer.guide.xml programmer.guide.fo

include ../version.mk
include ../../../../doc/docbook.mk
