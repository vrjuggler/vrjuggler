
CORE_SOURCES = \
	VjGUI/AppCore.java             VjGUI/BorderedPanel.java             \
	VjGUI/DescEnumElemPanel.java   VjGUI/CfgFileRequester.java          \
	VjGUI/DescEnumFrame.java       VjGUI/ElCheapoRequester.java         \
	VjGUI/FileControl.java         VjGUI/ChunkDescDBPanel.java          \
	VjGUI/NetControl.java          VjGUI/ChunkDescFrame.java            \
	VjGUI/ClientGlobals.java       VjGUI/ClientUI.java                  \
	VjGUI/PropertyDescPanel.java   VjGUI/PropertyPanel.java             \
	VjGUI/ConfigChunkDBPanel.java  VjGUI/ConfigChunkFrame.java          \
	VjGUI/VarValuePanel.java       VjGUI/ConnectionPanel.java           \
	VjGUI/MainWindowPanel.java     VjGUI/DataDisplayFrame.java          \
	VjGUI/ProcGroupPanel.java      VjGUI/ProcessFrame.java


OBJECTS = $(CORE_SOURCES:.java=.class)

.SUFFIXES: .class .java
##### RULES #####

.java.class: 
	javac $<

app: ${OBJECTS}
	@echo "Built VjGUI."

clean:
	rm -f *.class *~ core




