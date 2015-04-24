# Compiling And Installing #

This information is supplimental to the `INSTALL.html` (or `INSTALL.txt`) found in the root of the VR Juggler source tree.

## Win32 ##

Compiling VR Juggler 1.1 on Windows is a little different, but not too bad if you follow these rules:

  * When supplying paths to configure and for env variables, use the format `DRIVELETTER:/mypath/to/juggler/subpath/`.  NOTE: it is importatnt you use UNIX-style slashes instead of the DOS-style slashes.

Here is an example of how to compile under Win32:

NOTE: This assumes that your compiler is set up so that you can type cl or link at the command prompt, and your MSVC++ `INCLUDE` and `LIB` env paths are set up...

First, set up your shell environment variables, here we do it for tcsh available with the [Cygwin UNIX toolkit](http://www.cygwin.com/).

```
# So the build system doesn't use gcc compiler.
setenv CC cl
setenv CXX cl
setenv LD link

# you can customize these to your needs...
set HOSTTYPE=win32
set HOME=c:/home/kevn
set modules_root=$HOME/src/juggler/build.$HOSTTYPE/
set install_dir=$HOME/tempinstall/$HOSTTYPE

# base directories (shouldn't need to change)
setenv VPR_BASE_DIR $modules_root/instlinks
setenv GADGET_BASE_DIR $modules_root/instlinks
setenv JCCL_BASE_DIR $modules_root/instlinks
setenv VJ_BASE_DIR $modules_root/instlinks
setenv SNX_BASE_DIR $modules_root/instlinks
```

Next configure the juggler makefiles with configure (this assumes that you have all libraries that juggler depends on, and have run autogen.sh already - see `INSTALL.txt` for more info on this).

To configure:

```
mkdir build.$HOSTTYPE
cd build.$HOSTTYPE
../configure.pl --cfg=../juggler.cfg \
  --with-nspr=$HOME/src/nspr-4.2 \
  --with-nspr-includes=$HOME/src/nspr-4.2/include
```

NOTE: A binary build of nspr was found on the http://www.mozilla.org website.

NOTE: you may need to specify cppdom and GMTL directories with `--with-cppdom` and `--with-gmtl`, use the same path name convention as shown above.