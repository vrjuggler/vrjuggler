# $Id$

default: html

docs: html chunk-html pdf
install-docs: install-html install-chunk-html install-pdf

docdir=		../../../../doc
NAME=		device.driver.guide
XML_FILES=	$(NAME).xml

XSLT_TOOL=	Saxon

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	gadgeteer/$(PROJECT_NUMBER)/device.driver.guide

prefix=		$(webroot)/$(instdir)
INSTALL_FILES=	$(docdir)/base_style.css

NEED_DB_IMAGES=	1

include ../version.mk
include $(docdir)/docbook.mk
