# $Id$

default: txt

README=		README
INSTALL=	INSTALL
XML_FILES=	$(README).xml $(INSTALL).xml

XSLT_TOOL=Saxon 
#FO_VERSION=XEP

$(README).txt: $(README).html
$(README).pdf: $(README).fo
$(INSTALL).txt: $(INSTALL).html
$(INSTALL).pdf: $(INSTALL).fo

CLOBBER_FILES=	$(TXT_FILES)

include docbook.mk
