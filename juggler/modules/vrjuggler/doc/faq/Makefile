# $Id$

default: html

XML_FILES=	faq.xml
HTML_FILES=	faq.html
PDF_FILES=	faq.pdf

faq.html: faq.xml
faq.pdf: faq.xml faq.fo # faq.tex

include ../../../../docs/docbook.mk
