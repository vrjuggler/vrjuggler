# -----------------------------------------------------------------------------
# $Id$
#
# This include file <VjPerf.mk> defines the list of .java files that are
# built into Java bytecode with javac.
# -----------------------------------------------------------------------------
# Variables defined here are:
#
# VJPERF_SOURCES - The list of .java files to build
# VJPERF_OBJS    - The list of .class files that will be created.
# -----------------------------------------------------------------------------

VJPERF_SOURCES =   VjPerf/PerfAnalyzerPanel.java       	\
		   VjPerf/GraphFrame.java		\
		   VjPerf/PerfDataCollection.java	\
		   VjPerf/PerfDataCollector.java       	\
		   VjPerf/DataLine.java

VJPERF_OBJS = ${VJPERF_SOURCES:.java=.class}
