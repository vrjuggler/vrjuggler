# $Id$

NAME = sonix.ieeevr2002
MK_PATH=../../../../docs/
default: html pdf

# comment these out if you don't want to use renderX
XSLT_TOOL=Saxon 
FO_VERSION=XEP


#########################################################
#EXTRA_XALAN_FO_PARAMS= -PARAM draft.watermark.image " " -PARAM draft.watermark.image.frag " " -PARAM background-image " "
TXT_FILES=    ${NAME}.txt
HTML_FILES=   ${NAME}.html
PDF_FILES=    ${NAME}.pdf

${NAME}.txt:  ${NAME}.xml # README.fo
${NAME}.html: ${NAME}.xml
${NAME}.pdf:  ${NAME}.xml ${NAME}.fo # README.tex

include ${MK_PATH}/docbook.mk
