# $Id$

default: html

XML_FILES=	book.xml
HTML_FILES=	book.html
PDF_FILES=	book.pdf

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/phpjug
instdir=	vrjuggler/1.0/juggler.team.guide

prefix=		$(webroot)/$(instdir)
INSTALL_FILES=	$(webroot)/base_style.css

NEED_DB_IMAGES=	1

book.html: book.xml
book.pdf: book.xml book.fo

include ../docbook.mk
