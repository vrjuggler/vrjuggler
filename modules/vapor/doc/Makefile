# $Id$

default: html

docs: html pdf
install-docs: install-html install-ps install-pdf

DOXYGEN_FILE=		VPR.doxy
HTML_DOXYGEN_FILE=	$(DOXYGEN_FILE)
LATEX_DOXYGEN_FILE=	VPR_latex.doxy
MAN_DOXYGEN_FILE=	$(DOXYGEN_FILE)
HTML_OUTPUT_DIR=	html
LATEX_OUTPUT_DIR=	latex

# Fill these in!!  Together, they form the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	vapor/0.17/programmer.reference

prefix=		$(webroot)/$(instdir)

include ../../../doc/doxygen.mk
