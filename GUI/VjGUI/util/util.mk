# -----------------------------------------------------------------------------
# $Id$
#
# This include file <util.mk> defines the list of .java files that are built
# into Java bytecode with javac.
# -----------------------------------------------------------------------------
# Variables defined here are:
#
# VJGUIUTIL_SOURCES - The list of .java files to build
# VJGUIUTIL_OBJS    - The list of .class files that will be created.
# -----------------------------------------------------------------------------

VJGUIUTIL_SOURCES = VjGUI/util/FloatTextField.java	\
		    VjGUI/util/IntegerTextField.java	\
		    VjGUI/util/NoSpaceTextField.java	\
		    VjGUI/util/StringTextField.java     \
		    VjGUI/util/JFrameParent.java        \
		    VjGUI/util/LinkedList.java          \
		    VjGUI/util/ListIterator.java        \
		    VjGUI/util/ListElem.java		\
		    VjGUI/util/PlugPanel.java		\
		    VjGUI/util/ChildFrame.java

VJGUIUTIL_OBJS = ${VJGUIUTIL_SOURCES:.java=.class}
