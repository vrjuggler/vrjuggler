# $Id$

default: html

docs: html pdf
install-docs: install-html install-pdf

DOXYGEN_FILE=		Gadgeteer.doxy
HTML_DOXYGEN_FILE=	$(DOXYGEN_FILE)
LATEX_DOXYGEN_FILE=	Gadgeteer_latex.doxy
MAN_DOXYGEN_FILE=	$(DOXYGEN_FILE)
HTML_OUTPUT_DIR=	html
LATEX_OUTPUT_DIR=	latex

# Fill these in!!  Together, they form the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	gadgeteer/$(PROJECT_NUMBER)/programmer.reference

prefix=		$(webroot)/$(instdir)

include version.mk
include ../../../doc/doxygen.mk
