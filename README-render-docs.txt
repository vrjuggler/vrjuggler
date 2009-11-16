Rendering the Juggler documentation is very simple. First, get the latest
version of the tools and stylesheets used to do the rendering:

   http://www.infiscape.com/~patrick/docbook.tar.bz2 (31.8 MB)

or

   http://www.infiscape.com/~patrick/docbook.7z (24 MB)

Unpack the tools somewhere convenient (such as $HOME/tools). Version 1.4.2
or newer of the Java runtime environment is required. This can be retrieved
from the following site:

   http://java.sun.com/javase/downloads/index.jsp

With a Java installation, set the environment variable JAVA_HOME to point
to the directory containing bin/java (for example, with Java 1.5.0_08 on
Linux, a likely setting would be JAVA_HOME=/usr/java/jdk1.5.0_08). Then, run
the following using GNU Make:

   make -f Makefile.docs DOCBOOK_ROOT=$HOME/tools/docbook all

This creates three renderings of the documentation:

   1. PDF
   2. Single-page HTML
   3. Multi-page HTML

The default target (the one run without the 'docs' argument) generates
the single-page HTML version. Other targets are 'pdf', 'html',
'chunk-html', 'install-docs', 'install-html', 'install-chunk-html', and
'install-pdf'.
