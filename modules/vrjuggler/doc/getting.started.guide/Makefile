# $Id$

default: html

NAME=		book

XML_FILES=	$(NAME).xml
HTML_FILES=	$(NAME).html
PDF_FILES=	$(NAME).pdf

# Fill this in!!
webroot=	$(HOME)/public_html/phpjug

instdir=	vrjuggler/1.0/getting.started.guide
prefix=		$(webroot)/$(instdir)
INSTALL_DIRS=	figures
INSTALL_FILES=	$(webroot)/base_style.css

$(NAME).html: $(NAME).xml
$(NAME).pdf: $(NAME).xml $(NAME).fo

include ../../../../docs/doc$(NAME).mk
