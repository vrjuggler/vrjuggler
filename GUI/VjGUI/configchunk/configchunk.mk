# -----------------------------------------------------------------------------
# $Id$
#
# This include file <configchunk.mk> defines the list of .java files that are built
# into Java bytecode with javac.
# -----------------------------------------------------------------------------
# Variables defined here are:
#
# VJGUICONFIGCHUNK_SOURCES - The list of .java files to build
# VJGUICONFIGCHUNK_OBJS    - The list of .class files that will be created.
# -----------------------------------------------------------------------------

VJGUICONFIGCHUNK_SOURCES = VjGUI/configchunk/ConfigChunkFrame.java 	  \
			   VjGUI/configchunk/ListBoxModel.java            \
			   VjGUI/configchunk/PropertyPanel.java		  \
			   VjGUI/configchunk/VarValuePanel.java           \
			   VjGUI/configchunk/VarValueStandardPanel.java   \
			   VjGUI/configchunk/VarValueMiniChunkPanel.java  \
			   VjGUI/configchunk/VarValueBigChunkPanel.java

VJGUICONFIGCHUNK_OBJS = ${VJGUICONFIGCHUNK_SOURCES:.java=.class}
