# $Id$

default: html

XML_FILES=	book.xml
HTML_FILES=	book.html
PDF_FILES=	book.pdf

book.html: book.xml
book.pdf: book.xml book.fo # book.tex

include ../docbook.mk
