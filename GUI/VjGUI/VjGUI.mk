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

VJGUI_SOURCES = VjGUI/VjControl.java		\
                VjGUI/FileControl.java		\
                VjGUI/NetControl.java		\
		VjGUI/ChunkDBPanel.java         \
		VjGUI/ChunkDBTreeCellRenderer.java \
		VjGUI/ChunkDBTreeModel.java     \
		VjGUI/ChunkOrgTree.java         \
		VjGUI/OrgTreeElem.java          \
		VjGUI/ChunkOrgTreePane.java     \
		VjGUI/ChunkTreeNodeInfo.java    \
                VjGUI/ConfigurePane.java        \
		VjGUI/ConnectionPane.java       \
		VjGUI/ConsolePane.java          \
		VjGUI/ControlUI.java		\
		VjGUI/Core.java			\
		VjGUI/DescDBPanel.java		\
		VjGUI/HTMLFrame.java		\
		VjGUI/SuffixFilter.java         \
		VjGUI/DependencyFrame.java      \
		VjGUI/CoreDBEvent.java		\
		VjGUI/CoreDBListener.java	\
		VjGUI/LogMessageEvent.java	\
		VjGUI/LogMessageListener.java	\

VJGUI_OBJS = ${VJGUI_SOURCES:.java=.class}
