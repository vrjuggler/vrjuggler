# $Id$

default: html

docs: html chunk-html pdf
install-docs: install-html install-chunk-html install-pdf

NAME=		build.system

XML_FILES=	$(NAME).xml

XSLT_TOOL=	Saxon

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	docs/juggler.build.system

prefix=		$(webroot)/$(instdir)
INSTALL_FILES=	$(webroot)/base_style.css

NEED_DB_IMAGES=	1

include ../docbook.mk
