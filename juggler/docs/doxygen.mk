# $Id$

DOXYGEN?=		doxygen
HTML_OUTPUT_DIR?=	html
LATEX_OUTPUT_DIR?=	latex
MAN_OUTPUT_DIR?=	man

html: $(HTML_OUTPUT_DIR)
	$(DOXYGEN) $(DOXYGEN_FILE)

ps: $(LATEX_OUTPUT_DIR)
	$(MAKE) -C $(LATEX_OUTPUT_DIR) ps

pdf: $(LATEX_OUTPUT_DIR)
	$(MAKE) -C $(LATEX_OUTPUT_DIR) pdf

$(HTML_OUTPUT_DIR) $(LATEX_OUTPUT_DIR) $(MAN_OUTPUT_DIR):
	$(DOXYGEN) $(DOXYGEN_FILE)

install-html:
	if [ ! -d $(prefix) ]; then mkdir -p $(prefix); fi
	mv $(HTML_OUTPUT_DIR)/* $(prefix)

install-ps:
	if [ ! -d $(prefix) ]; then mkdir -p $(prefix); fi
	mv $(LATEX_OUTPUT_DIR)/refman.ps $(prefix)

install-pdf:
	if [ ! -d $(prefix) ]; then mkdir -p $(prefix); fi
	mv $(LATEX_OUTPUT_DIR)/refman.pdf $(prefix)
