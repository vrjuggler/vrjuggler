# $Id$

default: txt

README=		README
INSTALL=	INSTALL
FILES=		$(README) $(INSTALL)

XSLT_TOOL=Saxon 
#FO_VERSION=XEP

TXT_FILES=	$(FILES:=.txt)
HTML_FILES=	$(FILES:=.html)
PDF_FILES=	$(FILES:=.pdf)

$(README).txt: $(README).xml $(README).html # $(README).fo
$(README).html: $(README).xml
$(README).pdf: $(README).xml $(README).fo
$(INSTALL).txt: $(INSTALL).xml $(INSTALL).html # $(INSTALL).fo
$(INSTALL).html: $(INSTALL).xml
$(INSTALL).pdf: $(INSTALL).xml $(INSTALL).fo

CLOBBER_FILES=	$(TXT_FILES)

include docbook.mk
