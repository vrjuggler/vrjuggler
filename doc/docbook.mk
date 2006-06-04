# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2006 by Iowa State University
#
# Original Authors:
#   Allen Bierbaum, Christopher Just,
#   Patrick Hartling, Kevin Meinert,
#   Carolina Cruz-Neira, Albert Baker
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# This file provides all the settings and targets needed to generate HTML and
# PDF documents from DocBook XML and SGML input.  It requires GNU make 3.78 or
# newer.
#
# This makefile uses many paths that are specific to the computer systems at
# the Virtual Reality Applications Center, but it can be customized for local
# installations.  Editing the version numbers and base directory paths below
# should be sufficient.
# -----------------------------------------------------------------------------

.SUFFIXES: .html .xml .pdf .tex .fo .txt

# Software and stylesheet versions.
DOCBOOK_XSL_VERSION?=	1.70.1
XALAN_VERSION?=		2_7_0
SAXON_VERSION?=		6.5.5
FOP_VERSION?=		0.20.5
BATIK_VERSION?=		1.5.1

ifeq ($(FOP_VERSION), 0.20.5)
FOP_SCRIPT=	fop.sh
else
FOP_SCRIPT=	fop
endif

# Installation paths.
DOCBOOK_ROOT?=	/home/vr/Juggler/docbook
BATIK_ROOT?=	$(DOCBOOK_ROOT)/batik-$(BATIK_VERSION)
SGML_ROOT?=	/usr/share/sgml/docbook
TEX_DIR?=	$(DOCBOOK_ROOT)/TeX
TEX_BINDIR?=	$(TEX_DIR)/bin/i386-linux

# Application paths.
DVIPDF?=	dvipdf
DVIPS?=		dvips
FOP?=		sh $(DOCBOOK_ROOT)/fop-$(FOP_VERSION)/$(FOP_SCRIPT)
HTML2TXT?=	html2text
HTML2TXTOPTS?=	-ascii -nobs -style pretty -width 76 -rcfile html2text.rc
HTML2TXTFILE?=	file:$<
JADE?=		openjade -V tex-backend
JADEPROC?=	$(DOCBOOK_ROOT)/jadeproc.pl
JADETEX?=	$(TEX_BINDIR)/jadetex
PDFJADETEX?=	$(TEX_BINDIR)/pdfjadetex
PDFLATEX?=	$(TEX_BINDIR)/pdflatex
PDFTEX?=	$(TEX_BINDIR)/pdftex
PDFXMLTEX?=	$(TEX_BINDIR)/pdfxmltex
RASTERIZER?=	BATIK_ROOT="$(BATIK_ROOT)" $(BATIK_ROOT)/svgrasterizer
RM=		rm -f
SAXON_DIR?=	$(DOCBOOK_ROOT)/saxon-$(SAXON_VERSION)
SAXON?=		$(SAXON_DIR)/saxon.sh
TEX_ENV?=	PATH=$(TEX_BINDIR):$(PATH) VARTEXMF=$(TEX_DIR)/texmf-var
XALAN_DIR?=	$(DOCBOOK_ROOT)/xalan-j_$(XALAN_VERSION)
XALAN?=		$(XALAN_DIR)/bin/xalan.sh
XEP?=		sh $(DOCBOOK_ROOT)/XEP/run.sh
XSLTPROC?=	/usr/bin/xsltproc

FO_TOOL?=	FOP
XSLT_TOOL?=	Xalan

recursive_copy=	tar --exclude .svn --exclude CVS -chvf - $(1) | tar -C $(2) -xpf -

# Use one of the following depending on what will be processing the generated
# FO.  The default is to use FOP.  XEP or Passive TeX can be used instead by
# defining $(USE_XEP) or $(USE_PASSIVE_TEX) respectively.
ifeq ($(FO_TOOL), FOP)
ifeq ($(FOP_VERSION), 0.20.5)
   XALAN_FO_PARAMS=	-PARAM fop.extensions "1" -PARAM alignment "start"
   SAXON_FO_PARAMS=	fop.extensions=1 alignment="start"
else
   XALAN_FO_PARAMS=	-PARAM fop1.extensions "1" -PARAM alignment "start"
   SAXON_FO_PARAMS=	fop1.extensions=1 alignment="start"
endif
endif
ifeq ($(FO_TOOL), XEP)
   XALAN_FO_PARAMS=	-PARAM xep.extensions "1"
   SAXON_FO_PARAMS=	xep.extensions=1
endif
ifeq ($(FO_TOOL), PASSIVE_TEX)
   XALAN_FO_PARAMS=	-PARAM passivetex.extensions "1"		\
			-PARAM tex.math.in.alt "latex"
   SAXON_FO_PARAMS=	passivetex.extensions=1 tex.math.in.alt=latex
endif

SAXON_HTML_PARAMS=	html.stylesheet=base_style.css
XALAN_HTML_PARAMS=	-PARAM html.stylesheet "base_style.css"

XALAN_TXT_PARAMS=	-PARAM page.margin.bottom "0in"	\
			-PARAM page.margin.inner "0in"	\
			-PARAM page.margin.outer "0in"	\
			-PARAM page.margin.top "0in"	\
			-PARAM paper.type "A5"

DB_SGML_DTD?=	$(DOCBOOK_ROOT)/docbook-sgml-4.1.dtd
DSSSL_DIR?=	$(DOCBOOK_ROOT)/docbook-dsssl-1.76
XSL_DIR=	$(DOCBOOK_ROOT)/docbook-xsl-$(DOCBOOK_XSL_VERSION)

HTML_XSL?=		$(XSL_DIR)/html/docbook.xsl
CHUNK_HTML_XSL?=	$(XSL_DIR)/html/chunk.xsl
FO_XSL?=		$(XSL_DIR)/fo/docbook.xsl

ENV=		DOCBOOK_XSL=$(XSL_DIR) DOCBOOK_ROOT=$(DOCBOOK_ROOT)	\
		SAXON_DIR=$(SAXON_DIR) XALAN_DIR=$(XALAN_DIR)		\
		DOCBOOK_XSL_VERSION=$(DOCBOOK_XSL_VERSION)		\
		SAXON_VERSION=$(SAXON_VERSION)

ifdef NEED_DB_IMAGES
LINK_DEPS=	images
endif

HTML_FILES=		$(XML_FILES:.xml=.html)
CHUNK_HTML_FILES=	$(XML_FILES:.xml=/index.html)
FO_FILES=		$(XML_FILES:.xml=.fo)
PDF_FILES=		$(XML_FILES:.xml=.pdf)
TXT_FILES=		$(XML_FILES:.xml=.txt)

txt: $(TXT_FILES)
html: $(LINK_DEPS) $(HTML_FILES)
chunk-html: $(CHUNK_HTML_FILES)
pdf: $(LINK_DEPS) $(PDF_FILES)

# The method for specifying a path to the images that come with the DocBook
# XSL stylesheets sucks.  It requires a path relative to the current directory,
# and we do not necessarily know how to get to $(XSL_DIR) relative to the
# current directory.  This hack with a symlink works around that problem.
images:
	-rm -f ./images
	ln -s $(XSL_DIR)/images ./

install-txt: $(TXT_FILES)
ifndef prefix
	$(error "No prefix set!")
else
	if [ ! -d "$(prefix)" ]; then mkdir -p $(prefix); fi
	cp $(TXT_FILES) $(prefix)/
endif

install-html: $(LINK_DEPS) $(HTML_FILES)
ifndef prefix
	$(error "No prefix set!")
else
	if [ ! -d "$(prefix)" ]; then mkdir -p $(prefix); fi
	cp $(HTML_FILES) $(prefix)/
ifdef INSTALL_FILES
	cp $(INSTALL_FILES) $(prefix)/
endif
ifdef INSTALL_DIRS
	$(call recursive_copy, $(INSTALL_DIRS), $(prefix))
endif
ifdef NEED_DB_IMAGES
	$(call recursive_copy, images, $(prefix))
endif
endif

install-chunk-html:
ifndef prefix
	$(error "No prefix set!")
else
	if [ ! -d "$(prefix)" ]; then mkdir -p $(prefix); fi
	for file in $(XML_FILES) ; do \
            dir=`echo $$file | sed -e 's/\.xml//'` ; \
            $(call recursive_copy, $$dir, $(prefix)) ; \
            if [ ! -z "$(INSTALL_FILES)" ]; then \
                cp $(INSTALL_FILES) $(prefix)/$$dir ; \
            fi ; \
            if [ ! -z "$(NEED_DB_IMAGES)" ]; then \
                $(call recursive_copy, images, $(prefix)/$$dir) ; \
            fi ; \
            if [ ! -z "$(INSTALL_DIRS)" ]; then \
                $(call recursive_copy, $(INSTALL_DIRS), $(prefix)/$$dir) ; \
            fi ; \
          done
endif

install-pdf: $(PDF_FILES)
ifndef prefix
	$(error "No prefix set!")
else
	cp $(PDF_FILES) $(prefix)/
endif

install install-all:
	$(MAKE) install-html
	$(MAKE) install-chunk-html
	$(MAKE) install-pdf

# Image conversions -----------------------------------------------------------

%.png: %.svg
	$(RASTERIZER) -bg 255.255.255.255 -m png $<

%.jpg %.jpeg: %.svg
	$(RASTERIZER) -q .75 -bg 255.255.255.255 -m jpg $<

# Basic XSL conversions -------------------------------------------------------

%.html: %.xml
ifeq ($(XSLT_TOOL), Xalan)
	$(ENV) $(XALAN) -in $< -xsl $(HTML_XSL) -out $@		\
          $(XALAN_HTML_PARAMS) $(EXTRA_XALAN_HTML_PARAMS)
else
	$(ENV) $(SAXON) -i $< -xsl $(HTML_XSL) -o $@		\
          $(SAXON_HTML_PARAMS) $(EXTRA_SAXON_HTML_PARAMS)
endif

%/index.html: %.xml
	for file in $(XML_FILES) ; do \
            dir=`echo $$file | sed -e 's/\.xml//'` ; \
            if [ ! -d $$dir ] ; then mkdir $$dir ; fi ; \
            if [ ! -z "$(INSTALL_FILES)" ]; then \
                cp $(INSTALL_FILES) $$dir ; \
            fi ; \
            if [ ! -z "$(INSTALL_DIRS)" ]; then \
                $(call recursive_copy, $(INSTALL_DIRS), $$dir) ; \
            fi ; \
            cur_dir=`pwd` ; \
            cd $$dir ; \
            $(ENV) $(SAXON) -i $$cur_dir/$$file -xsl $(CHUNK_HTML_XSL) \
              $(SAXON_HTML_PARAMS) $(EXTRA_SAXON_HTML_PARAMS) ; \
            cd $$cur_dir ; \
          done

%.fo: %.xml
ifeq ($(XSLT_TOOL), Xalan)
	$(ENV) $(XALAN) -in $< -xsl $(FO_XSL) -out $@		\
          $(XALAN_FO_PARAMS) $(EXTRA_XALAN_FO_PARAMS)
else
	$(ENV) $(SAXON) -i $< -xsl $(FO_XSL) -o $@		\
          $(SAXON_FO_PARAMS) $(EXTRA_SAXON_FO_PARAMS)
endif

%.txt: %.html
	$(HTML2TXT) $(HTML2TXTOPTS) $(EXTRA_HTML2TXTOPTS) $(HTML2TXTFILE) > $@

#%.txt: %.xml
#ifeq ($(XSLT_TOOL), Xalan)
#	$(ENV) $(XALAN) -in $< -xsl $(FO_XSL) -out $@		\
#          $(XALAN_TXT_PARAMS) $(EXTRA_XALAN_TXT_PARAMS)
#else
#	$(ENV) $(SAXON) -i $< -xsl $(FO_XSL) -o $@		\
#          $(SAXON_TXT_PARAMS) $(EXTRA_SAXON_TXT_PARAMS)
#endif
#	$(FOP) -fo $< -txt $@

# Generate a PDF file from an FO file using FOP.
ifeq ($(FO_TOOL), FOP)
$(PDF_FILES): $(FO_FILES)
$(TXT_FILES): $(FO_FILES)

%.pdf: %.fo
	$(FOP) $< $@

%.txt: %.fo
	$(FOP) -fo $< -txt $@
endif

# Generate a PDF file from an XML file using FOP.
#%.pdf: %.xml
#	$(FOP) -xml $< -xsl $(FO_XSL) -pdf $@

# -----------------------------------------------------------------------------

# Jade/JadeTex conversions ----------------------------------------------------

# Generate a TeX file using Jade.
ifdef USE_JADEPROC
%.pdf: %.xml
	$(TEX_ENV) $(JADEPROC) -i $< -o $@ -d $(DB_SGML_DTD)		\
          -s $(DSSSL_DIR)/print/docbook.dsl
         
else
%.tex: %.xml
	$(JADE) -t tex -d $(DSSSL_DIR)/print/docbook.dsl $<

# $(PDFJADETEX) has to be run twice for page references to be calculated.  :(
%.pdf: %.xml
	-$(TEX_ENV) $(PDFJADETEX) $<
endif

# -----------------------------------------------------------------------------

# PassiveTeX conversions ------------------------------------------------------

# Generate a PDF file from an XML file using PassiveTeX.  This one requires
# that a simple TeX file be generated from the XML first (see below).
ifeq ($(FO_TOOL), PASSIVE_TEX)
$(PDF_FILES): $(FO_FILES)

%.pdf: %.fo
	$(TEX_ENV) $(PDFXMLTEX) $*.fo
#	$(TEX_ENV) $(PDFXMLTEX) $*.fo

# Generate a TeX file for use with PassiveTeX.
#%.tex: %.xml
#	@echo "Generating $*.tex from $<"
#	@echo '\def\xmlfile{$*.fo}' >$*.tex
#	@echo '\input xmltex' >>$*.tex
endif

# -----------------------------------------------------------------------------

# RenderX XEP conversions -----------------------------------------------------

# Generate a PDF file using XEP from RenderX.  This requires that an FO file
# be generated first.
ifeq ($(FO_TOOL), XEP)
$(PDF_FILES): $(FO_FILES)

%.pdf: %.fo
	$(XEP) $*.fo
endif

# -----------------------------------------------------------------------------

clean:
	$(RM) *.aux *.out *.tex *.log *.fo *~ $(CLEAN_FILES)
ifneq ($(CLEAN_DIRS), )
	$(RM) -r $(CLEAN_DIRS)
endif

clobber:
	@$(MAKE) clean
	$(RM) -f *.html *.pdf $(LINK_DEPS) $(CLOBBER_FILES)
	$(RM) -rf $(XML_FILES:.xml=)
ifneq ($(CLOBBER_DIRS), )
	$(RM) -r $(CLOBBER_DIRS)
endif
