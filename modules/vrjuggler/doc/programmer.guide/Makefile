# $Id$

.SUFFIXES: .html .xml .pdf

FOP		= /home/users/allenb/Software/fop/fop.sh
RM		= rm -f
XSLTPROC	= /usr/bin/xsltproc

DOCBOOK_ROOT	= /home/users/allenb/Docs/vj_docs/docbook
XSL_DIR		= $(DOCBOOK_ROOT)/xsl
#DOCBOOK_ROOT	= /usr/share/sgml/docbook
#XSL_DIR		= $(DOCBOOK_ROOT)/xsl-stylesheets-1.29

guide.html: guide.xml
guide.pdf: guide.xml

.xml.html:
	$(XSLTPROC) --output $@ $(XSL_DIR)/html/docbook.xsl $<

.xml.pdf:
	$(XSLTPROC) --output $*.fo-temp $(XSL_DIR)/fo/docbook.xsl $<
	$(XSLTPROC) --output $*.fo $(XSL_DIR)/fo/fo-patch-for-fop.xsl $*.fo-temp
	$(FOP) -fo $*.fo -pdf $@
	@$(RM) $*.fo $*.fo-temp

clean:
	$(RM) *.fo *~

clobber:
	$(RM) *.html *.pdf
