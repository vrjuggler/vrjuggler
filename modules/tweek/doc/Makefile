# $Id$

default: html

docs: html pdf
install-docs: install-html install-pdf

DOXYGEN_FILE=		Tweek.doxy
HTML_DOXYGEN_FILE=	$(DOXYGEN_FILE)
LATEX_DOXYGEN_FILE=	Tweek_latex.doxy
MAN_DOXYGEN_FILE=	$(DOXYGEN_FILE)
HTML_OUTPUT_DIR=	html
LATEX_OUTPUT_DIR=	latex

# Fill these in!!  Together, they form the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	tweek/$(PROJECT_NUMBER)/programmer.reference.cxx

prefix=		$(webroot)/$(instdir)

include ../../../doc/doxygen.mk
include version.mk
