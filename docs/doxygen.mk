# $Id$

DOXYGEN?=		doxygen
HTML_OUTPUT_DIR?=	html
LATEX_OUTPUT_DIR?=	latex
MAN_OUTPUT_DIR?=	man
RTF_OUTPUT_DIR?=	rtf

html: $(HTML_OUTPUT_DIR)
	$(DOXYGEN) $(HTML_DOXYGEN_FILE)

man:
	$(DOXYGEN) $(MAN_DOXYGEN_FILE)

latex:
	$(DOXYGEN) $(LATEX_DOXYGEN_FILE)

ps: $(LATEX_OUTPUT_DIR)
	$(MAKE) -C $(LATEX_OUTPUT_DIR) ps

pdf: $(LATEX_OUTPUT_DIR)
	$(MAKE) -C $(LATEX_OUTPUT_DIR) pdf

rtf:
	$(DOXYGEN) $(RTF_DOXYGEN_FILE)

install-html:
	if [ ! -d $(prefix) ]; then mkdir -p $(prefix); fi
	cp -r $(HTML_OUTPUT_DIR)/* $(prefix)

install-ps:
	if [ ! -d $(prefix) ]; then mkdir -p $(prefix); fi
	cp $(LATEX_OUTPUT_DIR)/refman.ps $(prefix)

install-pdf:
	if [ ! -d $(prefix) ]; then mkdir -p $(prefix); fi
	cp $(LATEX_OUTPUT_DIR)/refman.pdf $(prefix)
