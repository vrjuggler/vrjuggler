# ============================================================================
# Makefile for creating PDF files from HTML books.  It requires GNU make.
# ============================================================================

default: books

.SUFFIXES: .html .ps .pdf

# ----------------------------------------------------------------------------
# Variables.
# ----------------------------------------------------------------------------
prefix		= !!FILL ME IN!!
bookdir		= $(prefix)/html/books

PSGEN		= /home/users/patrick/bin/html2ps
PSGEN_FLAGS	= -f $(BOOK_CFG_FILE)
PDFGEN		= ps2pdf13
PDFGEN_FLAGS	=

BOOK_CFG_FILE	= ../style/book.cfg

BOOKS		= getting.started.guide	\
                  juggler.arch		\
                  juggler.overview	\
                  juggler.team.guide	\
                  programmer.guide	\
                  vjcontrol.guide

HTML_BOOKS	= $(addsuffix .html, $(addprefix $(bookdir), $(BOOKS)))
PS_BOOKS	= $(addsuffix .ps, $(BOOKS))
PDF_BOOKS	= $(addsuffix .pdf, $(BOOKS))

vpath %.html $(bookdir)

# ----------------------------------------------------------------------------
# Targets
# ----------------------------------------------------------------------------
books: $(PDF_BOOKS)
	@echo "Done."

install-books:
	@for book in $(PDF_BOOKS) ; do		\
            echo "$$book --> $(prefix)/pub" ;	\
            cp $$book $(prefix)/pub ;		\
        done

%.pdf: %.html
	$(PSGEN) $(PSGEN_FLAGS) $< > $*.ps
	$(PDFGEN) $(PDFGEN_FLAGS) $*.ps
	rm -f $*.ps

#getting.started.guide.pdf: getting.started.guide.html
