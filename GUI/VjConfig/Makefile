
CORE_SOURCES = \
        VjConfig/ConfigChunkDB.java VjConfig/ConfigChunk.java  \
	VjConfig/Property.java      VjConfig/Property.java     \
	VjConfig/ChunkDescDB.java   VjConfig/ChunkDesc.java    \
	VjConfig/DescEnum.java      VjConfig/PropertyDesc.java \
	VjConfig/ValType.java       VjConfig/VarValue.java     \
	VjConfig/VarValueException.java VjConfig/ConfigStreamTokenizer.java  

OBJECTS = $(CORE_SOURCES:.java=.class)
.SUFFIXES: .class .java

##### RULES #####

.java.class: 
	javac $<

app: ${OBJECTS}
	@echo "Making VjConfig Package"

clean:
	rm -f *.class *~ core

