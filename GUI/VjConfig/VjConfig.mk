# -----------------------------------------------------------------------------
# $Id$
#
# This include file <VjConfig.mk> defines the list of .java files that are
# built into Java bytecode with javac.
# -----------------------------------------------------------------------------
# Variables defined here are:
#
# VJCONFIG_SOURCES - The list of .java files to build
# VJCONFIG_OBJS    - The list of .class files that will be created.
# -----------------------------------------------------------------------------

VJCONFIG_SOURCES = VjConfig/ConfigChunkDB.java		\
                   VjConfig/ConfigChunk.java		\
                   VjConfig/Property.java		\
                   VjConfig/Property.java		\
                   VjConfig/ChunkDescDB.java		\
                   VjConfig/ChunkDesc.java		\
                   VjConfig/DescEnum.java		\
                   VjConfig/PropertyDesc.java		\
                   VjConfig/ValType.java		\
                   VjConfig/VarValue.java		\
                   VjConfig/VarValueException.java	\
                   VjConfig/ConfigStreamTokenizer.java  

VJCONFIG_OBJS = ${VJCONFIG_SOURCES:.java=.class}
