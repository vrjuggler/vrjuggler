# ============================================================================
# Makefile for creating PDF files from HTML books.  It requires GNU make.
# ============================================================================

default: books

.SUFFIXES: .html .ps .pdf

# ----------------------------------------------------------------------------
# Variables.
# ----------------------------------------------------------------------------
# FILL ME IN!!!!: set prefix to your install-web dest directory.
prefix		= /home/wwwadm/www-vrjuggler/docs
book_srcdir	= $(prefix)/html/books
book_distdir	= $(book_srcdir)

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
                  programmer.reference	\
                  vjcontrol.guide

HTML_BOOKS	= $(addsuffix .html, $(addprefix $(book_srcdir), $(BOOKS)))
PS_BOOKS	= $(addsuffix .ps, $(BOOKS))
PDF_BOOKS	= $(addsuffix .pdf, $(BOOKS))

vpath %.html $(book_srcdir)

# ----------------------------------------------------------------------------
# Targets
# ----------------------------------------------------------------------------
books: $(PDF_BOOKS)
	@echo "Done."

install-books:
	@if test ! -d $(book_distdir) ; then	\
            mkdir -p $(book_distdir) ;		\
        fi
	@for book in $(PDF_BOOKS) ; do		\
            echo "$$book --> $(book_distdir)" ;	\
            cp $$book $(book_distdir) ;		\
        done

%.pdf: %.html
	$(PSGEN) $(PSGEN_FLAGS) $< > $*.ps
	$(PDFGEN) $(PDFGEN_FLAGS) $*.ps
	rm -f $*.ps

#getting.started.guide.pdf: getting.started.guide.html
