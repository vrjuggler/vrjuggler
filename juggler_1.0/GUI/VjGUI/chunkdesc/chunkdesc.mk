# -----------------------------------------------------------------------------
# $Id$
#
# This include file <VjGUI.mk> defines the list of .java files that are built
# into Java bytecode with javac.
# -----------------------------------------------------------------------------
# Variables defined here are:
#
# VJGUICHUNKDESC_SOURCES - The list of .java files to build
# VJGUICHUNKDESC_OBJS    - The list of .class files that will be created.
# -----------------------------------------------------------------------------

VJGUICHUNKDESC_SOURCES = VjGUI/chunkdesc/ChunkDescFrame.java	\
			 VjGUI/chunkdesc/DescEnumElemPanel.java \
			 VjGUI/chunkdesc/DescEnumFrame.java     \
			 VjGUI/chunkdesc/PropertyDescPanel.java

VJGUICHUNKDESC_OBJS = ${VJGUICHUNKDESC_SOURCES:.java=.class}
