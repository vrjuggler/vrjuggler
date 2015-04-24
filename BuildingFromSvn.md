# Basics #

Building VR Juggler from source is basically the same whether the source came from the repository or from a released version. Some things will change (such as renaming `buildwin32.py` to `build_windows.py` for VR Juggler 2.1 and beyond), but most of the information on this page applies to VR Juggler code going back to late 2003 or early 2004.

## How to Get the Code ##

The steps are simple:

  1. [Learn about Subversion](http://svnbook.red-bean.com/) (SVN).
  1. Read about [access to the Subversion repository](http://code.google.com/p/vrjuggler/source/checkout) through Google Code. Developers must use their googlecode.com password in order to check out a read/write copy of the code over HTTPS.
  1. Refer to the [Subversion branches](SvnBranches.md) page which describes what branches and tags are available.

There are subdirectories of the SVN root for VR Juggler (`juggler`), PyJuggler (`PyJuggler`), VRJ.NET (`vrj.net`), the VR Juggler website (`website`), and the patched version of CppUnit (`cppunit`) that we use.

The anonymous checkout command for the Juggler Project trunk is as follows:

```
svn co http://vrjuggler.googlecode.com/svn/juggler/trunk juggler
```

The anonymous checkout command for the Juggler Project 2.2 branch is as follows:

```
svn co http://vrjuggler.googlecode.com/svn/juggler/branches/2.2 juggler-2.2
```

The anonymous checkout command for the Juggler Project 2.0 branch is as follows:

```
svn co http://vrjuggler.googlecode.com/svn/juggler/branches/2.0 juggler-2.0
```

The anonymous checkout command for the Juggler Project 1.0 branch (which is deprecated, unsupported, etc.) is as follows:

```
svn co http://vrjuggler.googlecode.com/svn/juggler/branches/1.0 juggler-1.0
```

The anonymous checkout command for the PyJuggler HEAD branch is as follows:

```
svn co http://vrjuggler.googlecode.com/svn/PyJuggler/trunk PyJuggler
```

The anonymous checkout command for the PyJuggler 1.0 branch is as follows:

```
svn co http://vrjuggler.googlecode.com/svn/PyJuggler/branches/1.0 PyJuggler-1.0
```

## Dependencies ##

### Boost ###

We make use of many features of the Boost libraries.  In general, the Juggler code uses Boost libraries that do not have to be compiled.  The exception to this is Boost.Filesystem.  Furthermore, Boost must be installed, just like any other third-party library, in order for Juggler to be compiled against it.  We currently require Boost 1.31.0 or newer.  Boost 1.32.0, 1.33.0, 1.33.1, and 1.34.0 have been tested successfully.

Documentation for compiling Boost can be found on the [Boost website](http://www.boost.org/).  That documentation will not be duplicated here as it is quite extensive and may change in the future.

### Python (Win32 only) ###

On Windows, a Visual Studio solution is used to build the Juggler code.  A Python script called `buildwin32.py` (`build_windows.py` in VR Juggler 2.1 and newer) is used to set up the build environment and to launch Visual Studio so that the dependencies will be found correctly.  Python 2.3 is recommended, but Python 2.2 will work just fine.

Python can be found at http://www.python.org/

### NSPR ###

We require the use of NSPR 4.4 or newer on the following platforms:

  * Windows (prior to VR Juggler 3.0)
  * Solaris

NSPR is _optional_ on all other platforms.  Binary distributions of NSPR can be downloaded from http://www.mozilla.org/projects/nspr/

### Java development environment ###

We require the [Java SDK](http://java.sun.com/j2se/) (J2SE) for compiling and execution of our Java-based code.  We require version 1.4 or newer.  Version 1.5.0 is recommended.

### JDOM ###

As of October 15, 2003, JDOM comes with the VR Juggler source tree.  There is no need to download it separately.  People interested in exploring the use of JDOM on their own can go the the JDOM website:

  * [JDOM](http://www.jdom.org/): Java DOM API

### CORBA Implementation for C++ ###

The Tweek module makes use of CORBA to facilitate communication between Java and C++ code.  To use the Tweek C++ API (which is optional and probably always will be), you have to have a C++ implementation of CORBA.  So far, we have done all of our testing with omniORB, which can be downloaded from the following site:

  * [omniORB](http://omniorb.sf.net/): C++ CORBA ORB

We currently require 4.0 or higher, and we highly recommend the use of version 4.0.7 or newer. Note that omniORB 4.0.4 has problems on Windows, so using either 4.0.3 or 4.0.5 is the best option for that platform. Visit the [omniORB download page](http://sourceforge.net/project/showfiles.php?group_id=51138) for more information.

omniORB binary and source RPMs can be found using a web search.

### CORBA Implementation for Java ###

To use the Tweek JAVA API (which is required if you want to build VRJConfig), you must have a Java implementation of CORBA.  At this time, the Tweek build system supports three such implementations:

  * Java IDL: configure Tweek with `--with-java-orb=JDK` (recommended for JDK 1.4 users)
  * OpenORB: configure Tweek with `--with-java-orb=OpenORB` (recommended for JDK 1.3 users)
  * JacORB: configure Tweek with `--with-java-orb=JacORB`

Java IDL comes with the JDK, but you must use JDK 1.4 or newer.  Previous versions of Java IDL will not work.  If you have JDK 1.4 or newer installed, it is much easier to skip the use of OpenORB and use the JDK implementation of CORBA.

OpenORB used to be maintained by [the Exolab Group](http://openorb.exolab.org/), but it now exists a community project on SourceForge:

  * [OpenORB](http://openorb.sourceforge.net/): Java CORBA ORB

When using OpenORB 1.3.0, make sure you specify all _three_ required JAR files:

  * openorb-1.3.0.jar
  * avalon-framework.jar
  * logkit.jar

### IDL-to-Java Compiler ###

To compile the IDL code to Java, the Juggler source tree includes the JacORB IDL compiler, which is implemented entirely in Java.  This makes it highly portable and very convenient.  Using JacORB's IDL-to-Java compiler is highly recommended since it is the default and requires no special options to the `configure` script.

**NOTE**: For some reason, JacORB's IDL-to-Java compiler does not work on SGI Prism systems. Instead, `idlj`, which comes with the JDK/J2SE, must be used. Use the `--with-java-idl` option to `configure.pl` to give the full path to the `idlj` executable.

# Compiling #

## How to Build It ##

First, read the documentation (begin with `INSTALL.txt` or `INSTALL.html` in the top-level source directories).  This page is not meant to replace the documentation that comes with the source.  This page only describes things that are specific for the SVN version or that people frequently run into.

We have found that it's best to use the absolute path to `configure.pl` when configuring a build.  On Linux, this helps DDD (and possibly other tools) get its bearings so that debugging can be done effectively.  Relative paths to source files tend to confuse DDD, and using the absolute path causes the source directories in the makefiles to appear as absolute paths.

## Tips for Building Dependencies ##

### Minimizing Boost Installation ###

The compiled Boost libraries required by VR Juggler versions are as follows:

  * VR Juggler 2.0: Boost.Filesystem
  * VR Juggler 2.1 and 2.2: Boost.Filesystem and Boost.Signals
  * VR Juggler 2.3: Boost.Filesystem, Boost.Signals, and Boost.program\_options

The time spent building Boost can be shortened by building only the Boost libraries that are required. For example, to build only Boost.Filesystem and Boost.Signals, add the following command line options when running `bjam` to build and install Boost:

```
--with-filesystem --with-signals
```

### omniORB on IRIX ###

When compiling omniORB with the MIPSpro compilers, make sure that the option `-LANG:std` is used.  This is needed for compatibility with the way that VR Juggler, Boost, etc. are compiled with the MIPSpro Compilers.  Configure omniORB as follows for csh or tcsh:

```
% env CC=cc CXX="CC -LANG:std" ./configure --prefix=/some/install/path
```

For sh or sh-derived shells, use the following:

```
% CC=cc CXX="CC -LANG:std" ./configure --prefix=/some/install/path
```

## Universal Binaries on Mac OS X ##

### Boost for i386 and PowerPC ###

#### Boost 1.33.x ####

To target i386 from PowerPC:

```
PYTHON_VERSION=2.3 bjam -sTOOLS=darwin "-sBUILD=debug release <runtime-link>static/dynamic <threading>single/multi <cxxflags>-arch <cxxflags>i386 <cxxflags>-isysroot <cxxflags>/Developer/SDKs/MacOSX10.4u.sdk <linkflags>-arch <linkflags>i386 <linkflags>-isysroot <linkflags>/Developer/SDKs/MacOSX10.4u.sdk" install --prefix=$HOME/boost-1.33.1-i386
rm -rf bin
PYTHON_VERSION=2.3 bjam -sTOOLS=darwin install --prefix=$HOME/boost-1.33.1-ppc
```

To target PowerPC from i386:

```
PYTHON_VERSION=2.3 bjam -sTOOLS=darwin "-sBUILD=debug release <runtime-link>static/dynamic <threading>single/multi <cxxflags>-arch <cxxflags>ppc <cxxflags>-isysroot <cxxflags>/Developer/SDKs/MacOSX10.4u.sdk <linkflags>-arch <linkflags>ppc <linkflags>-isysroot <linkflags>/Developer/SDKs/MacOSX10.4u.sdk" install --prefix=$HOME/boost-1.33.1-ppc
rm -rf bin
PYTHON_VERSION=2.3 bjam -sTOOLS=darwin install --prefix=$HOME/boost-1.33.1-i386
```

To create the universal binaries:

```
cd $HOME
mkdir -p boost-1.33.1-universal/lib
cd boost-1.33.1-i386/lib
foreach f ( * )
lipo -create $HOME/boost-1.33.1-ppc/lib/$f $f -output $HOME/boost-1.33.1-universal/lib/$f
end
cd ..
mv include $HOME/boost-1.33.1-universal
cd $HOME
rm -rf boost-1.33.1-ppc boost-1.33.1-i386
```

#### Boost 1.34.x ####

To target i386 from PowerPC, first make the PowerPC build:

```
./configure --prefix=$HOME/boost-1.34.1-ppc
make
make install
```

Then, use the following as the contents for `user-config.jam`:

```
# Boost.Build Configuration
# Automatically generated by Boost configure 

# Compiler configuration
using darwin : : : <cxxflags>-arch <cxxflags>i386
                   <cxxflags>-isysroot <cxxflags>/Developer/SDKs/MacOSX10.4u.sdk
                   <linkflags>-arch <linkflags>i386
                   <linkflags>-isysroot <linkflags>/Developer/SDKs/MacOSX10.4u.sdk ; 

# Python configuration
using python : 2.3 : /System/Library/Frameworks/Python.framework/Versions/2.3 ;
```

Then, modify the generated `Makefile` to change the installation prefix to something such as `$HOME/boost-1.34.1-i386` and run the build again:

```
make
make install
```

To target PowerPC from i386, first make the i386 build:

```
./configure --prefix=$HOME/boost-1.34.1-i386
make
make install
```

Then, use the following as the contents for `user-config.jam`:

```
# Boost.Build Configuration
# Automatically generated by Boost configure 

# Compiler configuration
using darwin : : : <cxxflags>-arch <cxxflags>ppc
                   <cxxflags>-isysroot <cxxflags>/Developer/SDKs/MacOSX10.4u.sdk
                   <linkflags>-arch <linkflags>ppc
                   <linkflags>-isysroot <linkflags>/Developer/SDKs/MacOSX10.4u.sdk ; 

# Python configuration
using python : 2.3 : /System/Library/Frameworks/Python.framework/Versions/2.3 ;
```

Then, modify the generated `Makefile` to change the installation prefix to something such as `$HOME/boost-1.34.1-ppc` and run the build again:

```
make
make install
```

Finally, to create the universal binaries:

```
cd $HOME
mkdir -p boost-1.34.1-universal/lib
cd boost-1.34.1-i386/lib
foreach f ( * )
lipo -create $HOME/boost-1.34.1-ppc/lib/$f $f -output $HOME/boost-1.34.1-universal/lib/$f
end
cd ..
mv include $HOME/boost-1.34.1-universal
cd $HOME
rm -rf boost-1.34.1-ppc boost-1.34.1-i386
```

### omniORB for i386 and PowerPC ###

**IMPORTANT**: The omniORB build does not currently support creating universal binaries on Mac OS X. While the commands shown below will create universal binaries for PowerPC and i386, the PowerPC binaries at least will not function correctly due to the endianness of the hardware being detected incorrectly. It is not known if the i386 binaries built from the commands below work. The most likely way of creating universal binaries for omniORB is to make separate builds on a PowerPC Mac and on an i386 Mac and then combine them using `lipo`.

```
CC='gcc -arch ppc -arch i386 -isysroot /Developer/SDKs/MacOSX10.4u.sdk -mmacosx-version-min=10.4' CXX='g++ -arch ppc -arch i386 -isysroot /Developer/SDKs/MacOSX10.4u.sdk -mmacosx-version-min=10.4'  CPP='gcc -E -traditional-cpp' CXXCPP='g++ -E' ./configure --prefix=...
make
make install
```

### CppDOM for i386 and PowerPC ###

For CppDOM 0.6.x, use the following:

```
scons optimize=yes universal=yes sdk=/Developer/SDKs/MacOSX10.4u.sdk prefix=... install
```

For 0.7.9 and beyond, use the following:

```
scons darwin_universal=yes var_arch=universal var_type=optimized darwin_sdk=/Developer/SDKs/MacOSX10.4u.sdk
```

## Tips for Using `configure.pl` ##

When specifying multiple JAR files, separate them using the local shell's path separator character.  In other words, if you are using a UNIX-style shell, use ':' (colon) to separate each JAR file.  In DOS/Win32, use ';' (semi-colon).  Whatever string you pass in is handed directly to the Java compiler and runtime, so the path must be valid for that specific version.

### Example `configure.pl` Calls ###

Below are some example calls to `configure.pl` that demonstrate how to provide paths for optional features (primarily the Java code).  On IRIX and Linux (Red Hat, anyway), it should be possible to run `configure.pl` with _no_ options and get a usable build of the C++ code base.

#### IRIX SPROC (MIPSpro, 32-bit) ####

```
configure.pl --with-jdkhome=/usr/java2 --with-java-orb=JDK --with-vrpn=/home/vr/Juggler/irix/mipspro/vrpn --with-cppdom=/home/vr/Juggler/irix/mipspro --with-gmtl=/home/vr/Juggler/irix/mipspro --with-boost=/home/vr/Juggler/irix/mipspro --with-boost-includes=/home/vr/Juggler/irix/mipspro/include/boost-1_31
```

#### IRIX SPROC (MIPSpro, 64-bit) ####

```
configure.pl --with-abi=64_M4 --with-jdkhome=/usr/java2 --with-java-orb=JDK --with-vrpn=/home/vr/Juggler/irix64/mipspro/vrpn --with-cppdom=/home/vr/Juggler/irix64/mipspro --with-gmtl=/home/vr/Juggler/irix64/mipspro -with-boost=/home/vr/Juggler/irix64/mipspro --with-boost-includes=/home/vr/Juggler/irix64/mipspro/include/boost-1_31
```

#### IRIX POSIX (MIPSpro, 32-bit) ####

```
configure.pl --enable-subsystem=POSIX --with-jdkhome=/usr/java2 --with-java-orb=JDK --with-cxx-orb=omniORB4 --with-cxx-orb-root=/home/vr/Juggler/irix/mipspro --with-audiere=/home/vr/Juggler/irix/mipspro --with-openal=/home/vr/Juggler/irix/mipspro --with-vrpn=/home/vr/Juggler/irix/mipspro/vrpn --with-cppdom=/home/vr/Juggler/irix/mipspro --with-gmtl=/home/vr/Juggler/irix/mipspro --with-boost=/home/vr/Juggler/irix/mipspro --with-boost-includes=/home/vr/Juggler/irix/mipspro/include/boost-1_31
```

#### IRIX POSIX (MIPSpro, 64-bit) ####

```
configure.pl --with-abi=64_M4 --enable-subsystem=POSIX --with-jdkhome=/usr/java2 --with-java-orb=JDK --with-cxx-orb=omniORB4 --with-cxx-orb-root=/home/vr/Juggler/irix64/mipspro --with-audiere=/home/vr/Juggler/irix64/mipspro --with-openal=/home/vr/Juggler/irix64/mipspro --with-vrpn=/home/vr/Juggler/irix64/mipspro/vrpn --with-cppdom=/home/vr/Juggler/irix64/mipspro --with-gmtl=/home/vr/Juggler/irix64/mipspro --with-boost=/home/vr/Juggler/irix64/mipspro --with-boost-includes=/home/vr/Juggler/irix64/mipspro/include/boost-1_31
```

#### Red Hat Linux 8.0 (POSIX) ####

```
configure.pl --with-java-orb=JDK --with-jdkhome=/usr/java/j2sdk1.4.1_02 --with-cxx-orb=omniORB4 --with-cxx-orb-root=/home/vr/Juggler/linux-rh80 --with-audiere=/home/vr/Juggler/linux-rh80 --with-openal=/home/vr/Juggler/linux-rh80 --with-vrpn=/home/vr/Juggler/linux-rh80/vrpn --with-cppdom=/home/vr/Juggler/linux-rh80 --with-gmtl=/home/vr/Juggler/linux-rh80 --with-boost=/home/vr/Juggler/linux-rh80 --with-boost-includes=/home/vr/Juggler/linux-rh80/include/boost-1_31
```

#### Fedora Core 1 (POSIX) ####

```
configure.pl --with-java-orb=JDK --with-jdkhome=/usr/java/j2sdk1.4.1_02 --with-cxx-orb=omniORB4 --with-cxx-orb-root=/home/vr/Juggler/linux-fc1 --with-audiere=/home/vr/Juggler/linux-fc1 --with-openal=/home/vr/Juggler/linux-fc1 --with-vrpn=/home/vr/Juggler/linux-fc1/vrpn --with-cppdom=/home/vr/Juggler/linux-fc1 --with-gmtl=/home/vr/Juggler/linux-fc1 --with-boost=/home/vr/Juggler/linux-fc1 --with-boost-includes=/home/vr/Juggler/linux-fc1/include/boost-1_31
```

### Make an Argument Defaults File ###

Users can simplify calls to `configure.pl` by making a defaults file.  Such files come in two flavors: simple plain text or Perl-based.  The plain text file groups the default arguments by platform and lists each default argument on a single line.  See below for more details on this topic.

# Platform-Specific Gotchas #

## Mac OS X ##

  * Build Boost using the [`darwin` toolset](http://www.boost.org/tools/build/v1/darwin-tools.html)

## Solaris ##

  * Requires NSPR
    * Solaris headers define system calls such as `connect`(2) and `listen`(2) as preprocessor `#define`s.  This munges class declarations in VPR, and there is no easy workaround.

  * CppDOM and GMTL do not compile with GCC 2.95.3.  GCC 3.0 or newer is required.
  * The default compiler is the Sun Forte C++ compiler.  While this compiler is not actually capable of [compiling the code](CompilerStatus.md), this is done for consistency with other operating systems (IRIX, Windows, HP-UX) where the primary compiler is not GCC.  To use GCC as the compiler, you must specify `--with-gcc` when running the configure script(s).

## Win32 ##

  * Requires NSPR

# Win32 Annoyances #

## Visual C++ Environment Variables ##

All of the above assumes that your compiler is set up so that you can type `cl` or `link` at the command prompt and that your MSVC++ `= and `= env paths are set up. Getting this set up correctly this can be quite the pain in the back side.  The easiest way to do for general use it is to edit the system wide environment variables under Control Panel &gt; System.  If Windows only had a good set of command lines tools this may not be as painful.

The Python script `buildwin32.py` determines what version of Visual Studio (either 7.0 or 7.1, aka .NET 2002 or .NET 2003 respectively) to use based on what is found in your path.  If no Visual Studio installation is found in your path, then the script searches the following paths (in this order):

  * `C:\Program Files\Microsoft Visual Studio 8`
  * `C:\Program Files\Microsoft Visual Studio .NET 2003`

When executing `build_windows.py` from a DOS shell prompt, the best way to override this is to run the `vsvars32.bat` batch file to set up all the Visual Studio-related environment variables.  If running `build_windows.py` from Windows Explorer by double-clicking on the icon, the above approach of modifying the system-wide environment variables is the best bet.

# Advanced Uses #

This section details interesting tidbits about the VR Juggler build system that are not documented anywhere else at this time.  These pieces are not documented for one or more of the following reasons:

  1. They are still in development
  1. They are not required for typical use
  1. Inclusion in the general documentation may cause unnecessary confusion
  1. The documentation is still in development and not ready for widespread public consumption

This is not to say that this information will never be included but rather that it is not included _yet_.

## Default Arguments for `configure.pl` ##

In late June 2002, two methods for defining default arguments to pass to the top-level `configue.pl` were implemented:

  1. A simple configuration file that groups default arguments based on platform-specific identifiers
  1. Dynamically loaded Perl code that allows users to write whatever default-argument-loading mechanism they want

The following subsections describe each of these and how they may be used.

### Configuration File ###

The format for this file is intended to be extremely simple.  It uses the following basic format:

```
name1
{
	argument1
	argument2
	:
	:
	argumentN
}
```

The values for `name1` should correspond to operating system names such as `irix`, `freebsd`, or `windows`.  These names are _case-insensitive_.  There is one reserved keyword, `all`, that signifies that the arguments listed within the curly braces can (and will) be used on all platforms.

Within each platform-specific block, there must be one argument per line.  The individual arguments are passed to the Autoconf-generated configure scripts exactly as they appear in the file.  No intermediate processing is done to expand variable expressions, for example.

### User-Specified Perl Code ###

The rules for writing your own Perl code to set defaults arguments are simple.  They are as follows:

  * The Perl code must be a valid Perl package.  That means it must declare a package name (any name is fine) and have the following as the last line:

```
#!perl
1;
```

  * The module must have a subroutine that takes no arguments and returns an array (by copy).  The loading script is informed of this subroutine through a global variable in its scope: `$main::CFG_LOAD_FUNC`.

To plug into the loading configure script, you must assign a reference to your module's subroutine as the first step in the module.  For example, consider the case where we have a subroutine called `&loader`.  The loading module (`configure.pl`) would be informed of this subroutine as shown below:

```
#!perl
$main::CFG_LOAD_FUNC = \&loader;
```

The array returned by the user-defined Perl subroutine is merged with `@ARGV`.  Arguments in `@ARGV` take precedence over default arguments.  Thus, an argument given on the command line will always be used regardless of any default setting.