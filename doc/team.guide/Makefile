# $Id$

default: html

NAME=		book

XML_FILES=	$(NAME).xml
HTML_FILES=	$(NAME).html
PDF_FILES=	$(NAME).pdf

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/phpjug
instdir=	vrjuggler/1.0/juggler.team.guide

prefix=		$(webroot)/$(instdir)
INSTALL_FILES=	$(webroot)/base_style.css

NEED_DB_IMAGES=	1

$(NAME).html: $(NAME).xml
$(NAME).pdf: $(NAME).xml $(NAME).fo

include ../docbook.mk
