# $Id$

.SUFFIXES: .html .xml .pdf .tex .fo .txt

FOP=		sh $(DOCBOOK_ROOT)/fop/fop.sh
JADE=		openjade
JADETEX=	jadetex
PDFJADETEX=	pdfjadetex
RM=		rm -f
SAXON=		$(DOCBOOK_ROOT)/saxon-6.5.1/saxon.sh
#SAXON=		$(DOCBOOK_ROOT)/saxon-7.0/saxon.sh
#XALAN=		$(DOCBOOK_ROOT)/xalan-j_2_3_0/bin/xalan.sh
XALAN=		$(DOCBOOK_ROOT)/xalan-j_20020214/bin/xalan.sh
#XALAN=		$(DOCBOOK_ROOT)/xalan-j_20020219/bin/xalan.sh
XEP=		sh $(DOCBOOK_ROOT)/XEP/run.sh
XSLTPROC=	/usr/bin/xsltproc

SAXON_PARAMS=	fop.extensions=1

# Use one of the following depending on what will be processing the generated
# FO.  The first is for FOP; the second is for XEP; and the third is for
# PassiveTeX.
XALAN_FO_PARAMS=	-PARAM fop.extensions "1" # -PARAM column.count "2"
#XALAN_FO_PARAMS=	-PARAM xep.extensions "1" # -PARAM column.count "2"
#XALAN_FO_PARAMS=	-PARAM passivetex.extensions "1"	\
			-PARAM tex.math.in.alt "latex"

XALAN_TXT_PARAMS=	-PARAM page.margin.bottom "0in"	\
			-PARAM page.margin.inner "0in"	\
			-PARAM page.margin.outer "0in"	\
			-PARAM page.margin.top "0in"	\
			-PARAM paper.type "A5"

DOCBOOK_ROOT=	/home/vr/Juggler/docbook
SGML_ROOT=	/usr/share/sgml/docbook

DSSSL_DIR=	$(DOCBOOK_ROOT)/docbook-dsssl-1.74b
XSL_DIR=	$(DOCBOOK_ROOT)/docbook-xsl-1.49
#TEXINPUTS=	.:$(HOME)/tex:$(HOME)/latex/passivetex:$(HOME)/latex/base:
#TEXMF=		$(HOME)/latex

txt: $(TXT_FILES)

html: $(HTML_FILES)

chunk-html:
	for file in $(XML_FILES) ; do \
	  dir=`echo $$file | sed -e 's/\.xml//'` ; \
	  if [ ! -d $$dir ] ; then mkdir $$dir ; fi ; \
          cur_dir=`pwd` ; \
          cd $$dir ; \
          $(SAXON) -i $$cur_dir/$$file -xsl $(XSL_DIR)/html/chunk.xsl $(SAXON_PARAMS); \
          cd $$cur_dir ; \
	done

pdf: $(PDF_FILES)

# Basic XSL conversions -------------------------------------------------------

.xml.html:
	$(XALAN) -in $< -xsl $(XSL_DIR)/html/docbook.xsl -out $@

.xml.fo:
	$(XALAN) -in $< -xsl $(XSL_DIR)/fo/docbook.xsl -out $@		\
          $(XALAN_FO_PARAMS)

.xml.txt:
	$(XALAN) -in $< -xsl $(XSL_DIR)/fo/docbook.xsl -out $@		\
          $(XALAN_TXT_PARAMS)
	$(FOP) -fo $< -txt $@

# Generate a PDF file from an FO file using FOP.
.fo.pdf:
	$(FOP) $< $@

.fo.txt:
	$(FOP) -fo $< -txt $@

# Generate a PDF file from an XML file using FOP.
#.xml.pdf:
#	$(FOP) -xml $< -xsl $(XSL_DIR)/fo/docbook.xsl -pdf $@

# -----------------------------------------------------------------------------

# Jade/JadeTex conversions ----------------------------------------------------

# Generate a TeX file using Jade.
.xml.tex:
	$(JADE) -t tex -d $(DSSSL_DIR)/print/docbook.dsl $<

# Generate a PDF file using JadeTeX.  This one requires that a TeX file be
# generated using Jade first (see above).
#.xml.pdf:
#	$(JADETEX) $*.tex
#	$(PDFJADETEX) $*.tex

# -----------------------------------------------------------------------------

# PassiveTeX conversions ------------------------------------------------------

# Generate a PDF file from an XML file using PassiveTeX.  This one requires
# that a simple TeX file be generated from the XML first (see below).
#.xml.pdf:
#	TEXINPUTS="$(TEXINPUTS)" HOMETEXMF="$(TEXMF)" TEXMF="$(TEXMF):/usr/share/texmf" pdflatex $*.tex
#	TEXINPUTS="$(TEXINPUTS)" TEXMF="{\$$TEXMFMAIN,!!$(TEXMF)}" pdflatex $*.tex

# Generate a TeX file for use with PassiveTeX.
#.xml.tex:
#	@echo "Generating $*.tex from $<"
#	@echo '\def\xmlfile{$*.fo}' >$*.tex
#	@echo '\input xmltex' >>$*.tex

# -----------------------------------------------------------------------------

# RenderX XEP conversions -----------------------------------------------------

# Generate a PDF file using XEP from RenderX.  This requires that an FO file
# be generated first.
#.fo.pdf:
#	$(XEP) $*.fo

# -----------------------------------------------------------------------------

clean:
	$(RM) *.tex *.log *.fo *~

clobber:
	@$(MAKE) clean
	$(RM) *.html *.pdf
