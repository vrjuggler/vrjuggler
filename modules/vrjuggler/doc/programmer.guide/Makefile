# $Id$

default: html

docs: html chunk-html pdf
install-docs: install-html install-chunk-html install-pdf

docdir=		../../../../doc
NAME=		programmer.guide
XML_FILES=	$(NAME).xml

XSLT_TOOL=	Saxon

SVG_IMAGES= \
	figures/AppHierarchy.svg		\
	figures/GlAppHierarchy.svg		\
	figures/Kernel_Loop_Call_Sequence.svg	\
	figures/how_sonix_plugins_work.svg	\
	figures/sonix_interface.svg		\
	figures/sonix_system_view.svg		\
	figures/userOglApp_hierarchy.svg	\
	figures/userOpenSGApp_hierarchy.svg	\
	figures/userOsgApp_hierarchy.svg	\
	figures/userPfApp_hierarchy.svg
IMAGES=		$(SVG_IMAGES:%.svg=%.png)	\
		figures/eq_m1_eq_inv_m2.png	\
		figures/eq_m1_eq_m1_m2.png	\
		figures/eq_m1_eq_m2_m1.png	\
		figures/eq_m1_eq_tr_m2.png	\
		figures/eq_v_eq_m_v.png

# Fill these in!!  Together, they make up the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	vrjuggler/$(PROJECT_NUMBER)/programmer.guide

prefix=		$(webroot)/$(instdir)
INSTALL_DIRS=	figures
INSTALL_FILES=	$(docdir)/base_style.css

NEED_DB_IMAGES=	1

$(NAME).html: $(IMAGES)
$(NAME)/index.html: $(IMAGES)
$(NAME).pdf: $(IMAGES)

CLOBBER_FILES=	$(SVG_IMAGES:%.svg=%.png)

include ../version.mk
include $(docdir)/docbook.mk
