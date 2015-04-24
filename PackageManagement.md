# General Issues #

Here, we discuss and lay out a process for packaging VR Juggler in a standard (or at least, reasonably standard) way. An important part of this is to identify the needs of various VR Juggler installations so that the packaging mechanism and recommendations meet these needs. [Managing VR Juggler configurations](ConfigFileManagement.md) is a separate topic, though these two are related. Indeed, solving the configuration file management issue goes a long way towards solving package management issues.

Much of this discussion concentrates on Linux distributions, but such operating systems are not the only platforms with software packaging. As the discussion evolves, it would be desireable for it to branch out and try to cover as many approaches as possible. Doing so would most likely result in a very robust recommendation for how to package and install VR Juggler.

## Installation Types ##

A common technique on Linux distributions is to separate packages into the runtime package (shared libraries, executables, data files), the development package (headers, static libraries, build bits), and the doucmentation. Following this approach for VR Juggler packaging is reasonable, and it has a benefit that will be seen later: it could facilitate side-by-side installations of multiple versions of VR Juggler. For this benefit to be realized, however, certain decisions have to be made, and those will be addressed below.

### Runtime Installation ###

What goes into the runtime installation? The files are those needed to execute pre-compiled VR Juggler applications. The usual approach with Red Hat-based Linux distributions is to install the following for a library package:

  * Versioned shared library with no `.so` symlink
    * The means by which a shared library is "versioned" on Linux is a topic of debate.
  * Executables not needed for building software using the library
    * In VR Juggler, that would primarily mean VRJConfig and the Tweek Java GUI.
  * Data files needed for executing applications that use the library in question
    * This would exclude things like `vrjuggler-config`, makefile stubs, `.pc` files, etc.

### Development Installation ###

The development installation complements the runtime installation by providing the files needed to compile VR Juggler applications. Looking at the VR Juggler installation tree, the developer installation would contain the files not included in the runtime installation. They would be the following:

  * Header files
  * The `.so` symlink(s) to the versioned shared library
  * Data files needed for compiling applications
    * This would include `vrjuggler-config`, makefiles subs, etc.

### Documentation Installation ###

The rendered VR Juggler documentation currently gets bundled up into a form that mirrors the structure of the VR Juggler website. This is done primarily to keep the installation process simple for the cases of installing documentation to the VR Juggler website and to the redistributable archive. For a VR Juggler documentation package, this would probably need to be rethought. The [Linux File System Hierarchy Standard](http://www.pathname.com/fhs/) would be a good place to start. A likely structure would be the following:

```
/usr/share/doc --+-- gadgeteer-1.0
                 |
                 +-- jccl-1.0
                 |
                 +-- sonix-1.0
                 |
                 +-- tweek-1.0
                 |
                 +-- vapor-1.0
                 |
                 +-- vrjuggler-2.0
```

## Parallel Installations ##
In some cases, it may be necessary to have multiple versions of VR Juggler installed in parallel. There are several reasons why this would happen, but one big reason is that binary compatibility is not guaranteed between any two releases of VR Juggler. For example, an application compiled against VR Juggler 2.0.1 would have to be recompiled to be used with VR Juggler 2.0.2. If a user has applications compiled against 2.0.x and wants to update to 2.0.y, it may not be possible to rebuild all the existing applications against the 2.0.y release. Hence, it would be necessary to keep the 2.0.x installation to be able to continue running all the installed applications.

Another case of using parallel installations would be for having VR Juggler 2.x and VR Juggler 2.y installed. That may mean having a stable release and an unstable release installed at the same time. The motivation for this is basically the same as what is described above.

Ultimately, parallel installations are needed to handle VR Juggler versions that lack binary compatibility. If VR Juggler 2.0.x and VR Juggler 2.0.y are binary compatible, then the 2.0.x installation can be replaced by the 2.0.y installation without impacting existing installed applications. Having binary compatibility reduces the need for parallel installations, and it allows existing applications to get bug fixes without having to be recompiled.

### Problems with Parallel Installations ###
Allowing parallel installations raises many complications, and as such, most projects do not allow for multiple versions to be installed in parallel. With VR Juggler, however, it would be beneficial to allow for multiple version to be installed side by side. Before we can accomplish this, we have to be aware of the problems posed by parallel installations. They are as follows:

  * VRJConfig must be able to find the correct `.jdef` files
  * Juggler must be able to find the correct `.jdef` files
  * The Input Manager, Cluster Manager, and Config Manager must be able to find the correct plug-ins
  * Juggler needs to know where to look for config files (`VJ_CFG_PATH` and a possible default configuration)
  * Dependencies (Boost, CppDOM, etc.) must be able to be installed in parallel
  * Binary compatibility would reduce the need for having multiple patch releases installed in parallel

In general, the common theme here is being able to find the correct data files for a given installation at run time. With two code bases involved (C++ and Java), we have to be sure that both can find the requisite data files in a reliable manner.

### Implementing Parallel Installations ###

So, we have to ask how to achieve parallel installations that do not conflict with each other. There are a variety of ways of doing this.

  * Install header files and libraries into versioned subdirectories
    * OpenSG installs its shared libraries into versioned subdirectories but not its headers.
  * Use fully versioned shared libraries and install header files into versioned subdirecctories
    * This is a very common practice for GTK+ and GNOME. With GTK+, the headers are installed into `/usr/include/gtk-2.0`; the shared libraries are named as `libgtk-1.2.so.0.9.1` and `libgtk-x11-2.0.so.0.600.10` (on Fedora Core 4). The [omniORB](http://omniorb.sourceforge.net/) shared librares also use this convention.
    * Boost uses this approach, though (bafflingly) not when installed from the RPM. The way that Boost works can be annoying, but it allows for many build variations _and_ versions of the compiled Boost libraries to be installed in parallel.
    * The Juggler libraries are already versioned in this way. For example, VPR 1.1.16's shared library is `libvpr.so.1.1.16` with symlinks named `libvpr.so`, `libvpr.so.1`, and `libvpr.so.1.1`.
      * Shared library versioning is not used on Windows, however. In that case, the DLL naming would probably have to follow the omniORB DLL convention: `omniORB407.dll`.

Aside from the shared libraries loaded by applications, the appropriate data files must be discoverable at run time. This means that data directories need to be versioned, too. Right now, we have the following structure:

```
$VJ_BASE_DIR/share -+-- gadgeteer
                    |
                    +-- jccl
                    |
                    +-- sonix
                    |
                    +-- tweek
                    |
                    +-- vpr
                    |
                    +-- vrjuggler
```

This would need to change to include the module's version number, as shown below:

```
$VJ_BASE_DIR/share -+-- gadgeteer-1.0
                    |
                    +-- jccl-1.0
                    |
                    +-- sonix-1.0
                    |
                    +-- tweek-1.0
                    |
                    +-- vpr-1.0
                    |
                    +-- vrjuggler-2.0
```

This assumes, however, that the data files are compatible between patch releases. Without this guarantee, we would have to do the following:

```
$VJ_BASE_DIR/share -+-- gadgeteer-1.0.1
                    |
                    +-- jccl-1.0.1
                    |
                    +-- sonix-1.0.1
                    |
                    +-- tweek-1.0.1
                    |
                    +-- vpr-1.0.1
                    |
                    +-- vrjuggler-2.0.1
```

Continuing with this trend of versioning everything, the contents of `$VJ_BASE_DIR/bin` would need version information, too. In order for VRJConfig to find the correct `.jdef` files, it would need to know where to look. Installing the `vrjconfig` shell script with a version number (such as `vrjconfig-2.0.2`) would provide the basis for this. Similarly, the `*-config` scripts would need version information in order to provide the correct compiler and linker options (depending on how headers and shared librares are installed for the purposes of compiling applications). A naming convention could be `vrjuggler20-config`. A more robust approach would be to use [`pkg-config`](http://pkgconfig.freedesktop.org/wiki/) since it has built-in support for package versioning, but that should be a separate discussion.

Underlying all of this would have to be an extension to Juggler's run-time behavior to do version-specific searches. The library version number is known at compile time, but it would be nice if users could provide a specific version number through an environment variable (an approach that is actually required for the Java code to function at this time). This new environment variable could be namd `VJ_VERSION`, and its purpose would be to help Juggler find what it needs at run time.

There is a problem with a single version number environment variable, however. The Juggler modules do not all have the same version number, and it is unlikely that they ever will. Given that the various modules undergo development at different rates, version numbers would have to be altered artificially to keep them the same, and this could lead to a lot of annoyances for developers. Since there are already multiple "base directory" environment variables (viz., `VJ_BASE_DIR`, `JCCL_BASE_DIR`, and so on), it would not be unreasonable to have multiple version number environment variables. If we agree that using the version number environment variable would only be for the purposes of overriding default behaviors, then it follows that the people doing this are almost certainly capable of understanding how multiple version number environment variables would have to be handled.

## Self-Contained Installations ##

Instead of pursuing parallel installations, we could use an approach where everything is installed under a version-specific root directory. Basically, this is what has been done to date with VR Juggler installations. In this case, the user is required to set `VJ_BASE_DIR`, `PATH`, `LD_LIBRARY_PATH`, etc. accordingly based on the needed version of VR Juggler. This has led to the proliferation of launch scripts to get all of this done correctly. While it does ensure that a given application will continue to work as long as its VR Juggler version remains installed, it gives up the benefits of using shared libraries and results in a management nightmare. Nevertheless, it is an option that many, many people have used, and formalizing it with package management must be considered.

The idea here is to provide some form of packaging for VR Juggler that would install _everything_ under a directory such as `/usr/local/vrjuggler-2.0.2`. Multiple VR Juggler versions could be installed this way without conflicting and without having to worry about dependency conflicts other complication posed by using parallel installations. As noted above, this approach requires more effort on the part of the user because environment variables would have to be set correctly in order to use the appropriate version.

## Dependency Management ##

Any decision made about how to manage VR Juggler installation requires that we consider how VR Juggler dependencies are managed. Excluding dependencies such as the X Window System and OpenGL, a VR Juggler runtime depends on shared libraries from CppDOM and Boost.Filesystem. (PyJuggler needs Boost.Python as well as the dependencies of VR Juggler.) The current version of CppDOM (0.6.1 as of this writing) has a specification file for RPMs providing runtime and development installations, but its shared libraries are not versioned. Fedora Core and other Linux distributions provide packaged versions of Boost, but for some reason, the RPM ignores the versioning of the shared libraries that Boost normally has.

# Platform-Specific Approaches #

## Linux ##

This section is for information on using Linux-specific package management systems for deploying VR Juggler on Linux distributions.

### RPM ###

As I see it, there are two options for how Juggler RPMs could be defined:

  1. Monolithic package
  1. Individual packages

The first is what has been done so far. It includes everything under one roof: the various Juggler modules, the plug-ins, and the Java bits. The second would be very fine grained.

#### Monolithic RPM ####

Using a monolithic RPM collects everything that can be built into two installable units called `vrjuggler` and `vrjuggler-devel`. The RPM spec file checked in to the Juggler repository provides this. A third package called `vrjuggler-docs` would probably need to be added.

**Pros**

  * This is simple for users to manage. They only have to worry about one package to worry about.

**Cons**

  * It does not capture the optional aspectcs of building Juggler. This means various driver plug-ins, Sonix plug-ins, and all the Java pieces.
  * Upgrading requires repacakging and reinstalling everything. This could be error prone if the RPM is not built with all the same optional pieces as in the previous build.

#### Multiple RPMs ####

**Pros**

  * Individual pieces can be upgraded.
  * Optional aspects are captured correctly.
  * Third parties can package extensions (plug-ins) and have them integrate nicely into the overall package management.
  * It encourages reuse of the Juggler modules by allowing users to install just the parts that they need. For example, if some software tool requires VPR but nothing else, the VPR package can be installed as the dependency.

**Cons**

  * Having more packages means more for users to understand.
    * If a package repository were set up, the process of getting things installed could be simplified quite a lot. Moreover, having an umbrella RPM (possibly called `juggler`) could provide a way to get everything installed as a dependency of that single RPM without needing a monolithic approach. This relies upon using a tool such as APT or YUM to do the dependency management. Manual installation of the RPMs would be a pain, but this is true of many, many packages that one can install on Red Hat-based distributions of Linux.

### Debian ###

TODO

# Proposal #

Given all of the above, the following is a proposal for how to achieve our goals. There are many steps involved, and changes to the Juggler project would have to be made on all platforms.

  1. Guarantee binary compatibility for a stable release series (2._x_ where _x_ is an even number).
  1. The "stable version" of VR Juggler that is installed is the default.
    * How is the default chosen/set?
  1. If users want to use a version other than the default, `VJ_BASE_DIR` and `VJ_VERSION` must be set.
    * Do there need to be corresponding `_VERSION` environment variables for the other modules? Probably.
  1. Shared library (DLL) versioning must be implemented on all platforms.
    * This will mean changing the names of the shared libraries in a way that is not backwards compatible with any VR Juggler 1.1 or 2.0 release up to 2.0.1.
  1. The data directories must be versioned.
  1. Guarantees must be made about `.jdef` file compatibility in a stable release series.
    * VRJConfig must be updated to search the versioned data directory for the correct `.jdef` files.
  1. Scripts such as `vrjuggler-config` and `vrjconfig` in the `bin` directory must be verrsioned.
    * Versioning the `*-config` scripts may not be necessary since they are only installed with a developer installation. See below for a further comment on installing multiple developer installations.

Additionally, applications could make use of the VR Juggler version information at compile time to ensure that run-time requirements are met. However, the run-time loader would address the need to get the correct shared library if the shared libraries are named to ensure multi-platform versioning.

A decision must be made about whether more than one Juggler developer installation can be installed at a time. Without versioning the header directories, it would not be possible to do this. If only one developer installation can be installed, then that is the onlly version that can be used for compiling VR Juggler applications on that machine.

Versioning of files and directories requires decisions about conventions. If we have binary compatibility and `.jdef` compatibility in a stable release series, then the versioned directories and shared libraries only need to include the major and minor version numbers. For an unstable series (those with an odd-numbered minor version), the need to maintain compatibility is greatly reduced. In that case, the version naming convention would have to contain the major, minor, and patch version numbers. To see the results of this, imagine that we have the runtime packages of Gadgeteer 1.0, 1.2, 1.3.4, and 1.3.9 installed. The shared libraries and data directories would appear as follows:

```
/usr/lib
         libgadget-1_0.so
         libgadget-1_2.so
         libgadget-1_3_4.so
         libgadget-1_3_9.so

/usr/share --+-- gadgeteer-1.0
             +-- gadgeteer-1.2
             +-- gadgeteer-1.3.4
             +-- gadgeteer-1.3.9
```

## Tasks ##

Making this proposal a reality requires work. Below, we list the tasks that must be performed to achieve our goals. Initially, the work would be done in VR Juggler 2.1, and a decision would have to be made regarding whether to backport the changes to VR Juggler 2.0.

Considering that VR Juggler 2.0 will probably be in use for quite some time, it would be beneficial if the next VR Juggler release (2.0.2) could take advantage of the capabilities that would exist for packaging VR Juggler 2.1 and beyond. However, doing so would essentially mean that users could have to go through some trouble to migrate to VR Juggler 2.0.2. By and large, details such as shard library naming changes would be captured by using `vrjuggler-config` and friends, but it is impossible to say how many users take advantage of the `*-config` scripts.

The VR Juggler 2.2 release could be accelerated to address some of these issues sooner. This would mean that most of the [goals for VR Juggler 2.2](DevelopmentStatus#VR_Juggler_2_2_Features.md) would have to be delayed for VR Juggler 2.4. However, the sooner users are migrated to VR Juggler 2.2 and any and all new installation conventions, the better.

### Binary Compatibility ###

Without binary compatibility, capabilities such as being able to upgrade an installed package become useless. If an installed VR Juggler application only works with VR Juggler 2.0.1, then VR Juggler 2.0.1 cannot be replaced by VR Juggler 2.0.2. Rather, both must be installed in parallel, and for said application to benefit from bug fixes in VR Juggler 2.0.2, it must be recompiled. In that case, the application might as well be linked statically against the Juggler libraries.

How can we achieve binary compatibility for VR Juggler? Short of redesinging the libraries to have a mutable internal interface and a consistent public interface, guaranteeing binary compatibility means being careful about making changes. If a change to the VR Juggler 2.0 release branch would break binary compatibility with previous 2.0.x releases, then that change cannot be made. Essentially, this means that the release branch would have to be strictly for bug fixes and stability enhancements that do not break binary compatibility. Informally, this is the intent, but the policy has not been enforced.

### Shared Library Versioning ###

Currently, shared libraries are versioned on UNIX-based platforms but not on Windows. The versioning on Mac OS X is the same as on Linux, FreeBSD, etc., but this is not a strictly correct approach. If only for the sake of consistency, we should be versioning shared libraries consistently on all platforms.

The Boost model provides a good approach by putting the version number into the shared library name in a way that does not interfere with platform-specific needs to identify files with three-letter extensions. For example, the optimized, multi-threaded Boost.Python library is named `libboost_python-gcc-mt-1_33_1.so` on GCC-based platforms. On Windows, it is named `boost_python-vc7-mt-1_33_1.dll` (for the `vc7` toolset). If we focus on just the version number, this would mean changing the Juggler libraries to be named as `libvpr-1_0_2.so` and `vpr-1_0_2.dll` instead of `libvpr.so.1.0.2` and `vpr.dll`. If we have binary compatibility for a given release series, we can simplify this to `libvpr-1_0.so` and `vpr-1_0.dll`.

### Data Directory Versioning ###

Versioning the data directories means that run-time searches for data files have to take the version number into account. Additionally, the build system must be extended to factor the module version number into the data directory name when installing. This should not be too difficult, but it has not been tried before. The configure scripts do know the data directory version, and the makefiles all reference `$(datadir)`.

### Versioning in the `bin` Directory ###

Versioning scripts in the `bin` directory is necessary for getting the correct information when they are called. For example, the `vrjconfig` script (batch file) would need to provide the JVM with the version number information needed to find the "built-in" `.jdef` files at run time. For example, `vrjconfig26` would be for VR Juggler 2.6, and `vrjconfig278` would be for VR Juggler 2.7.8. Both of these scripts could be installed in parallel, and they would set `VJ_VERSION` accordingly.

  * Should there be a `vrjconfig` for the default Juggler installation? This would be convenient for users. It would depend on how the default installation is chosen.

### Windows Registry Installation Lookup ###

On UNIX-based platforms, a packaged version of VR Juggler has a built-in default value for `VJ_BASE_DIR` so that users do not have to set it. This value is determined based on the value of the `--prefix` option used when compiling the release. With an RPM on Linux, this would be `/usr`. On Windows, however, the user can choose an alternate location for the VR Juggler installation using the installer wizard. At run time, there is no way for Juggler to determine where it was installed. This can be solved by having the VR Juggler kernel check the Windows registry for its installation path if `VJ_BASE_DIR` is not set.

### Implement CppDOM Shared Library Versioning ###

Currently, CppDOM is the only Juggler dependency that does not have shared library versioning at all. It needs to be extended to use the same conventions that are chosen for Juggler to allow parallel runtime installations.

### Fix CppDOM RPM Spec File ###

The RPM spec file for CppDOM is using some deprecated aspects. It needs to be updated to fix these problems so that the CppDOM RPMs can be built again. It would also have to be extended to be aware of versioning information.