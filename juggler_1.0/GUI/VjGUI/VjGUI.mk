# -----------------------------------------------------------------------------
# $Id$
#
# This include file <VjGUI.mk> defines the list of .java files that are built
# into Java bytecode with javac.
# -----------------------------------------------------------------------------
# Variables defined here are:
#
# VJGUI_SOURCES - The list of .java files to build
# VJGUI_OBJS    - The list of .class files that will be created.
# -----------------------------------------------------------------------------

VJGUI_SOURCES = VjGUI/AppCore.java		\
                VjGUI/BorderedPanel.java	\
                VjGUI/DescEnumElemPanel.java	\
                VjGUI/CfgFileRequester.java	\
                VjGUI/DescEnumFrame.java	\
                VjGUI/ElCheapoRequester.java	\
                VjGUI/FileControl.java		\
                VjGUI/ChunkDescDBPanel.java	\
                VjGUI/NetControl.java		\
                VjGUI/ChunkDescFrame.java	\
                VjGUI/ClientGlobals.java	\
                VjGUI/ClientUI.java		\
                VjGUI/PropertyDescPanel.java	\
                VjGUI/PropertyPanel.java	\
                VjGUI/ConfigChunkDBPanel.java	\
                VjGUI/ConfigChunkFrame.java	\
                VjGUI/VarValuePanel.java	\
                VjGUI/ConnectionPanel.java	\
                VjGUI/MainWindowPanel.java	\
                VjGUI/DataDisplayFrame.java	\
                VjGUI/ProcGroupPanel.java	\
                VjGUI/ProcessFrame.java		\
                VjGUI/NoSpaceTextField.java	\
                VjGUI/IntegerTextField.java	\
                VjGUI/FloatTextField.java

VJGUI_OBJS = ${VJGUI_SOURCES:.java=.class}
