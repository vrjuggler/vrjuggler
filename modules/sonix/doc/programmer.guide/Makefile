# $Id$

NAME =  programmer.guide
MK_PATH=../../../../doc/
default: html pdf chunk-html

docs: html chunk-html pdf
install-docs: install-html install-chunk-html install-pdf

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	sonix/$(PROJECT_NUMBER)/programmer.guide

prefix=		$(webroot)/$(instdir)
#INSTALL_DIRS=	figures
INSTALL_FILES=	$(webroot)/base_style.css *.png

# comment these out if you don't want to use renderX
XSLT_TOOL=Saxon 
FO_VERSION=XEP

#########################################################
#EXTRA_XALAN_FO_PARAMS= -PARAM draft.watermark.image " " -PARAM draft.watermark.image.frag " " -PARAM background-image " "
XML_FILES=    $(NAME).xml
TXT_FILES=    ${NAME}.txt
HTML_FILES=   ${NAME}.html
PDF_FILES=    ${NAME}.pdf

${NAME}.txt:  ${NAME}.xml # README.fo
${NAME}.html: ${NAME}.xml
${NAME}.pdf:  ${NAME}.xml ${NAME}.fo # README.tex

include ${MK_PATH}/docbook.mk
include ../version.mk
