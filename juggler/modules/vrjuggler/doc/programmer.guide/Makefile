# $Id$

default: html

XML_FILES=	guide.xml
HTML_FILES=	guide.html
PDF_FILES=	guide.pdf

guide.html: guide.xml
guide.pdf: guide.xml guide.fo # guide.tex

include ../../../../docs/docbook.mk
