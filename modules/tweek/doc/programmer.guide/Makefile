# $Id$

default: html

docs: html chunk-html pdf
install-docs: install-html install-chunk-html install-pdf

XML_FILES=	book.xml
HTML_FILES=	book.html
PDF_FILES=	book.pdf

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/phpjug
instdir=	tweek/1.0/programmer.guide

prefix=		$(webroot)/$(instdir)
#INSTALL_DIRS=	figures
INSTALL_FILES=	$(webroot)/base_style.css

NEED_DB_IMAGES=	1

book.html: book.xml
book.pdf: book.xml book.fo

#include ../version.mk
include ../../../../docs/docbook.mk
