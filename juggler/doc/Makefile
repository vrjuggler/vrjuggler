# $Id$

default: txt

TXT_FILES=	README.txt
HTML_FILES=	README.html
PDF_FILES=	README.pdf

README.txt: README.xml # README.fo
README.html: README.xml
README.pdf: README.xml README.fo # README.tex

include docbook.mk
