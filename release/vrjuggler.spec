# Spec file for vrjuggler.
#
# There are three optional packages that can be enabled through command line
# options to rpmbuild(1). They are as follows:
#
#    _with_ftd2xx:
#    _with_trackdapi:
#    _with_vrpn:
#
# These are the symbols to define using the --define option. The value must
# then be --with-<name>=/path/to/software where <name> is vrpn, ftd2xx, or
# trackdapi and /path/to/software is the path to the package root. For example,
# to provide the build with a path to VRPN, run rpmbuild(1) as follows:
#
#    rpmbuild -ba --define "_with_vrpn --with-vrpn=/path/to/vrpn" vrjuggler.spec
#
# Other optional packages such as Java, OpenAL, Audiere, and OpenGL Performer
# are detected automatically. They must be installed in /usr in order to be
# detected correctly.
%define name vrjuggler
%define global_version 2.2.2
%define global_release 1

%define vpr_version 2.0.2
%define vpr_release %{global_release}%{?dist}
%define tweek_version 1.2.2
%define tweek_release %{global_release}%{?dist}
%define jccl_version 1.2.2
%define jccl_release %{global_release}%{?dist}
%define sonix_version 1.2.2
%define sonix_release %{global_release}%{?dist}
%define gadgeteer_version 1.2.2
%define gadgeteer_release %{global_release}%{?dist}
%define gadgeteer_dataglove_release %{global_release}%{?dist}
%define gadgeteer_dtrack_release %{global_release}%{?dist}
%define gadgeteer_etherio24_release %{global_release}%{?dist}
%define gadgeteer_flock_release %{global_release}%{?dist}
%define gadgeteer_fastrak_release %{global_release}%{?dist}
%define gadgeteer_ibox_release %{global_release}%{?dist}
%define gadgeteer_intersense_api_release %{global_release}%{?dist}
%define gadgeteer_is900_release %{global_release}%{?dist}
%define gadgeteer_joydev_release %{global_release}%{?dist}
%define gadgeteer_motionstar_release %{global_release}%{?dist}
%define gadgeteer_p5glove_release %{global_release}%{?dist}
%define gadgeteer_pinchglove_release %{global_release}%{?dist}
%define gadgeteer_serial_encoder_release %{global_release}%{?dist}
%define gadgeteer_spaceball_release %{global_release}%{?dist}
%define gadgeteer_threedmouse_release %{global_release}%{?dist}
%define gadgeteer_trackd_release %{global_release}%{?dist}
%define gadgeteer_trackd_api_release %{global_release}%{?dist}
%define gadgeteer_vrpn_release %{global_release}%{?dist}
%define gadgeteer_wanda_release %{global_release}%{?dist}
%define gadgeteer_x_ist_release %{global_release}%{?dist}
%define gadgeteer_app_data_mgr_release %{global_release}%{?dist}
%define gadgeteer_rim_release %{global_release}%{?dist}
%define gadgeteer_start_barrier_release %{global_release}%{?dist}
%define gadgeteer_tcp_swap_lock_release %{global_release}%{?dist}
%define gadgeteer_wired_swap_lock_release %{global_release}%{?dist}
%define vrjuggler_version %{global_version}
%define vrjuggler_release %{global_release}%{?dist}

%define vpr_version_dist 2.0
%define tweek_version_dist 1.2
%define jccl_version_dist 1.2
%define sonix_version_dist 1.2
%define gadgeteer_version_dist 1.2
%define vrjuggler_version_dist 2.2

%define have_java %(if [ -x $JDK_HOME/bin/javac ] ; then echo 1; else echo 0; fi)

%if %have_java
   %define java_ver %(java -version 2>&1 | grep 'version' | sed -e 's/.*"\\([1-9]\\.[0-9]*\\.[0-9][0-9]*\\).*/\\1/')
   %if "%java_ver" == "1.5.0"
      %define java_pkg jre
   %else
      %define java_pkg jdk
   %endif
   %define java_req Requires: %java_pkg >= %java_ver
%else
%define java_req
%endif

%ifarch i386 i486 i586
%define vj_arch i386
%define tweek_arch i686
%else
%define vj_arch %{_arch}
%define tweek_arch %{_arch}
%endif

%ifarch x86_64
%define abi_option --with-abi=ELF_x86_64
%else
%ifarch i386 i486 i586
%define abi_option --with-abi=ELF_i386
%endif
%endif

%{!?_with_ftd2xx: %{!?_without_ftd2xx: %define _without_ftd2xx --without-ftd2xx}}
%{!?_with_trackdapi: %{!?_without_trackdapi: %define _without_trackdapi --without-trackdapi}}
%{!?_with_vrpn: %{!?_without_vrpn: %define _without_vrpn --without-vrpn}}

%define have_omniorb %(if [ -x /usr/bin/omniidl ] ; then echo 1; else echo 0; fi)
%define have_audiere %(if [ -x /usr/bin/audiere-config ] ; then echo 1; else echo 0; fi)
%define have_openal %(if [ -x /usr/bin/openal-config ] ; then echo 1; else echo 0; fi)
%define have_libusb %(if [ -x /usr/bin/libusb-config ] ; then echo 1; else echo 0; fi)
%define have_performer %(if [ -e /usr/lib/libpf.so ] ; then echo 1; else echo 0; fi)
%define have_ftd2xx %{?_with_ftd2xx:1}%{!?_with_ftd2xx:0}
%define have_trackdapi %{?_with_trackdapi:1}%{!?_with_trackdapi:0}
%define have_vrpn %{?_with_vrpn:1}%{!?_with_vrpn:0}

Name: vrjuggler
Summary: VR Juggler is the virtual platform for VR application development
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
Source: %{name}-%{global_version}-%{global_release}-src.tar.bz2
URL: http://www.vrjuggler.org/
Group: Development/Libraries
BuildRoot: %{_tmppath}/%{name}-%{global_version}-%{global_release}-buildroot
License: LGPL
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
Requires: sonix = %{sonix_version}
Requires: gadgeteer = %{gadgeteer_version}
Requires: gadgeteer-plugin-remote-input-manager = %{gadgeteer_version}
Requires: gadgeteer-plugin-start-barrier = %{gadgeteer_version}

%description
VR Juggler provides virtual reality (VR) software developers with a suite of
application programming interfaces (APIs) that abstract, and hence simplify,
all interface aspects of their program including the display surfaces, object
tracking, selection and navigation, graphics rendering engines, and graphical
user interfaces. An application written with VR Juggler is essentially
independent of device, computer platform, and VR system. VR Juggler may be run
with any combination of immersive technologies and computational hardware.

%package -n vpr
Summary: The VR Juggler Portable Runtime (VPR)
Version: %{vpr_version}
Release: %{vpr_release}
URL: http://www.vrjuggler.org/vapor/
Group: Development/C++
Requires: cppdom >= 0.3.3
Requires: boost >= 1.32
BuildRequires: glibc-devel
BuildRequires: e2fsprogs-devel
BuildRequires: cppdom-devel >= 0.3.3
BuildRequires: boost-devel >= 1.32

%description -n vpr
The VR Juggler Portable Runtime (VPR) provides an object-oriented
operating system abstraction layer that allows for easy development of
cross-platform C++ applications.

%package -n vpr-devel
Summary: The VR Juggler Portable Runtime Headers
Version: %{vpr_version}
Release: %{vpr_release}
URL: http://www.vrjuggler.org/vapor/
Group: Development/C++
Requires: flagpoll >= 0.9.1
Requires: cppdom-devel >= 0.3.3
Requires: boost-devel >= 1.32
Requires: vpr = %{vpr_version}

%description -n vpr-devel
The header files and libraries needed for developing programs using the
VR Juggler Portable Runtime.

%package -n tweek-c++
Summary: The Tweek C++ API
Version: %{tweek_version}
Release: %{tweek_release}
URL: http://www.vrjuggler.org/tweek/
Group: Development/C++
Requires: libomniorb >= 4.0.6-2
Requires: vpr = %{vpr_version}
BuildRequires: libomniorb-devel >= 4.0.6-2

%description -n tweek-c++
The Tweek C++ API provides a wrapper around CORBA for writing applications
based on the distributed model/view/controller system. In the scope of
VR Juggler, such applications are usually immersive worlds that have a
GUI that communicates with the immersive application through a network
connection.

%package -n tweek-c++-devel
Summary: The Tweek C++ API Headers
Version: %{tweek_version}
Release: %{tweek_release}
URL: http://www.vrjuggler.org/tweek/
Group: Development/C++
Requires: libomniorb-devel >= 4.0.6-2
Requires: flagpoll >= 0.9.1
Requires: vpr-devel = %{vpr_version}
Requires: tweek-c++ = %{tweek_version}

%description -n tweek-c++-devel
The header files and libraries needed for developing programs using the
Tweek C++ API.

%package -n tweek-java-jni
Summary: The JNI parts of the Tweek Java API
Version: %{tweek_version}
Release: %{tweek_release}
URL: http://www.vrjuggler.org/tweek/
Group: Development/Java
%java_req
AutoReqProv: no

%description -n tweek-java-jni
The Tweek Java API JNI library proivdes a natively compiled extension used
by some parts of the Tweek Java GUI.

%package -n tweek-java
Summary: The Tweek Java API and GUI
Version: %{tweek_version}
Release: %{tweek_release}
URL: http://www.vrjuggler.org/tweek/
Group: Development/Java
%java_req
Requires: tweek-java-jni = %{tweek_version}
Requires: flagpoll >= 0.9.1
AutoReqProv: no

%description -n tweek-java
The Tweek Java API provides a wrapper around CORBA for writing applications
based on the distributed model/view/controller system. In the scope of
VR Juggler, such applications are usually immersive worlds that have a
GUI that communicates with the immersive application through a network
connection. The Tweek Java GUI provides an framework for creating such
GUIs. Application programmers write JavaBeans that plug into the Tweek
Java GUI.

%package -n jccl-c++
Summary: The JCCL C++ API
Version: %{jccl_version}
Release: %{jccl_release}
URL: http://www.vrjuggler.org/jccl/
Group: Development/C++
Requires: vpr = %{vpr_version}

%description -n jccl-c++
The JCCL C++ API provides a dynamic configuration system based on XML.

%package -n jccl-c++-devel
Summary: The JCCL C++ API Headers
Version: %{jccl_version}
Release: %{jccl_release}
URL: http://www.vrjuggler.org/jccl/
Group: Development/C++
Requires: flagpoll >= 0.9.1
Requires: vpr-devel = %{vpr_version}

%description -n jccl-c++-devel
The header files and libraries needed for developing programs using the
JCCL C++ API.

%package -n jccl-rtrc-plugin-c++
Summary: The JCCL Remote Run-Time Reconfiguration C++ Plug-in
Version: %{jccl_version}
Release: %{jccl_release}
URL: http://www.vrjuggler.org/jccl/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: tweek-c++ = %{tweek_version}
Requires: jccl-c++ = %{jccl_version}

%description -n jccl-rtrc-plugin-c++
A plug-in to the JCCL Config Manager written in C++ that provides the
functionality needed for remote run-time reconfiguration through CORBA.

%package -n jccl-java
Summary: The JCCL Java API
Version: %{jccl_version}
Release: %{jccl_release}
URL: http://www.vrjuggler.org/jccl/
Group: Development/Java
%java_req
Requires: tweek-java = %{tweek_version}

%description -n jccl-java
The JCCL Java API provides a collection of JavaBeans that are used for
reading and editing JCCL configuration (.jconf) files and config
definition (.jdef) files. The JavaBeans are designed for use with and
depend on the Tweek Java GUI.

%package -n jccl-rtrc-plugin-java
Summary: The JCCL Remote Run-Time Reconfiguration JavaBean
Version: %{jccl_version}
Release: %{jccl_release}
URL: http://www.vrjuggler.org/jccl/
Group: Development/C++
%java_req
Requires: tweek-java = %{tweek_version}

%description -n jccl-rtrc-plugin-java
A plug-in to the JCCL Config Manager written in C++ that provides the
functionality needed for remote run-time reconfiguration through CORBA.

%package -n sonix
Summary: Sonix
Version: %{sonix_version}
Release: %{sonix_release}
URL: http://www.vrjuggler.org/sonix/
Group: Development/C++
Requires: vpr = %{vpr_version}
BuildRequires: gmtl >= 0.4.5

%description -n sonix
Sonix is a dynamically reconfigurable wrapper around audio APIs.

%package -n sonix-devel
Summary: The Sonix Headers
Version: %{sonix_version}
Release: %{sonix_release}
URL: http://www.vrjuggler.org/sonix/
Group: Development/C++
Requires: vpr-devel = %{vpr_version}
Requires: flagpoll >= 0.9.1
Requires: gmtl >= 0.4.5
Requires: sonix = %{sonix_version}

%description -n sonix-devel
The header files and libraries needed for developing programs using Sonix.

%package -n sonix-plugin-audiere
Summary: Sonix Audiere Plug-in
Version: %{sonix_version}
Release: %{sonix_release}
URL: http://www.vrjuggler.org/sonix/
Group: Development/C++
Requires: sonix = %{sonix_version}
Requires: audiere >= 1.9.3
BuildRequires: audiere-devel >= 1.9.3

%description -n sonix-plugin-audiere
Sonix plug-in that uses Audiere to generate sounds.

%package -n sonix-plugin-openal
Summary: Sonix OpenAL Plug-in
Version: %{sonix_version}
Release: %{sonix_release}
URL: http://www.vrjuggler.org/sonix/
Group: Development/C++
Requires: sonix = %{sonix_version}
Requires: openal >= 0.0.8
Requires: freealut >= 1.0.1
BuildRequires: openal-devel >= 0.0.8
BuildRequires: freealut-devel >= 1.0.1

%description -n sonix-plugin-openal
Sonix plug-in that uses OpenAL to generate sounds.

%package -n gadgeteer
Summary: Gadgeteer and all its device drivers and cluster plug-ins
Version: %{gadgeteer_version}
Release: %{gadgeteer_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: libX11.so.6
Requires: libICE.so.6
Requires: libSM.so.6
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5

%description -n gadgeteer
Gadgeteer is an extensible, highly configurable, cluster-ready input
device management for use with virtual reality applications. Users can
write Device drivers that are loaded dynamically.

%package -n gadgeteer-devel
Summary: The Gadgeteer Headers
Version: %{gadgeteer_version}
Release: %{gadgeteer_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: flagpoll >= 0.9.1
Requires: gmtl >= 0.4.5
Requires: vpr-devel = %{vpr_version}
Requires: jccl-c++-devel = %{jccl_version}
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-devel
The header files and libraries needed for developing programs and device
drivers using Gadgeteer. This is required for writing device driver
and cluster plug-ins to Gadgeteer.

%package -n gadgeteer-driver-5dt-dataglove
Summary: Fakespace Pinch Glove driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_dataglove_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-5dt-dataglove
The Gadgeteer driver plug-in for the 5DT DataGlove input device.

%package -n gadgeteer-driver-dtrack
Summary: ART DTrack driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_dtrack_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-dtrack
The Gadgeteer driver plug-in for the ART DTrack optical tracking system.

%package -n gadgeteer-driver-etherio24
Summary: Elexol Ether I/O 24 driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_etherio24_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-etherio24
The Gadgeteer driver plug-in for the Elexol Ether I/O 24 input device.

%package -n gadgeteer-driver-flock
Summary: Ascension Flock of Birds driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_flock_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-flock
The Gadgeteer driver plug-in for the Ascension Flock of Birds tracking
system.

%package -n gadgeteer-driver-fastrak
Summary: Polhemus Fastrak driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_fastrak_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-fastrak
The Gadgeteer driver plug-in for the Polhemus Fastrak tracking system.

%package -n gadgeteer-driver-ibox
Summary: ImmersionTech Interface Box (IBox) driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_ibox_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-ibox
The Gadgeteer driver plug-in for the ImmersionTech Interface Box (IBox)
input device.

%package -n gadgeteer-driver-intersense-api
Summary: InterSense Interface SDK driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_intersense_api_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-intersense-api
The Gadgeteer driver plug-in for the InterSense Interface SDK, used for
communicating with all InterSense tracker products.

%package -n gadgeteer-driver-is900
Summary: InterSense IS-900 driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_is900_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-is900
The Gadgeteer driver plug-in for the InterSense IS-900 tracking system via
the RS-232 interface. This plug-in should generally not be used. Use
gadgeteer-driver-intersense-api instead.

%package -n gadgeteer-driver-joydev
Summary: Linux game controller driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_joydev_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-joydev
The Gadgeteer driver plug-in for accessing game controllers on Linux via
the joydev device.

%package -n gadgeteer-driver-motionstar
Summary: Ascension MotionStar driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_motionstar_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-motionstar
The Gadgeteer driver plug-in for the Ascension MotionStar Wireless tracking
system.

%if %have_libusb
%package -n gadgeteer-driver-p5glove
Summary: Essential Reality P5 Glove driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_p5glove_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
BuildRequires: libusb-devel
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-p5glove
The Gadgeteer driver plug-in for the Essential Reality P5 Glove input device.
%endif

%package -n gadgeteer-driver-pinchglove
Summary: Fakespace Pinch Glove driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_pinchglove_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-pinchglove
The Gadgeteer driver plug-in for the Fakespace Pinch Glove input device.

%package -n gadgeteer-driver-serial-encoder
Summary: U.S. Digital Serial Encoder driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_serial_encoder_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-serial-encoder
The Gadgeteer driver plug-in for U.S. Digital Serial Encoder devices.

%package -n gadgeteer-driver-spaceball
Summary: 3Dconnexion SpaceBall driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_spaceball_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-spaceball
The Gadgeteer driver plug-in for 3Dconnexion SpaceBall devices.

%package -n gadgeteer-driver-threedmouse
Summary: Logitech 3D Mouse driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_threedmouse_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-threedmouse
The Gadgeteer driver plug-in for the Logitech 3D Mouse input device.

%package -n gadgeteer-driver-trackd
Summary: TrackD communication driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_trackd_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-trackd
The Gadgeteer driver plug-in for commnicating with VRCO TrackD.

%if %have_trackdapi
%package -n gadgeteer-driver-trackd-api
Summary: TrackD API driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_trackd_api_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-trackd
The Gadgeteer driver plug-in for commnicating with the TrackD API from VRCO.
%endif

%if %have_vrpn
%package -n gadgeteer-driver-vrpn
Summary: VRPN driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_vrpn_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-vrpn
The Gadgeteer driver plug-in for VRPN.
%endif

%package -n gadgeteer-driver-wanda
Summary: Ascension Wanda driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_wanda_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-wanda
The Gadgeteer driver plug-in for the Ascension Wanda input device.

%if %have_ftd2xx && %have_libusb
%package -n gadgeteer-driver-x-ist
Summary: noDNA X-IST driver plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_x_ist_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-driver-x-ist
The Gadgeteer driver plug-in for noDNA X-IST glove device.
%endif

%package -n gadgeteer-plugin-application-data-manager
Summary: Application Data Manager cluster plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_app_data_mgr_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-plugin-application-data-manager
The Gadgeteer cluster plug-in for the Application Data Manager.

%package -n gadgeteer-plugin-remote-input-manager
Summary: Remote Input Manager cluster plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_rim_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-plugin-remote-input-manager
The Gadgeteer cluster plug-in for the Remote Input Manager. This will be
needed for all clustered VR Juggler applications.

%package -n gadgeteer-plugin-start-barrier
Summary: Start barrier cluster plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_rim_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-plugin-start-barrier
The Gadgeteer Start Barrier cluster plug-in. This is used to ensure that all
nodes of a cluster start rendering on the same frame. Thsi will be needed for
all clustered VR Juggler applications.

%package -n gadgeteer-plugin-swap-lock-tcp
Summary: TCP swap lock cluster plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_rim_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-plugin-swap-lock-tcp
The Gadgeteer cluster plug-in for swap locking using TCP/IP. This will be
needed for all clustered VR Juggler applications when hardware swap locking
is not available.

%package -n gadgeteer-plugin-swap-lock-wired
Summary: Parallel port swap lock cluster plug-in for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_rim_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
BuildRequires: gmtl >= 0.4.5
Requires: gadgeteer = %{gadgeteer_version}

%description -n gadgeteer-plugin-swap-lock-wired
The Gadgeteer cluster plug-in for swap locking using custom parallel port
hardware. In general, most people will not need or be able to use this
plug-in.

%package -n vrjuggler-devel
Summary: The VR Juggler Headers
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/C++
Requires: flagpoll >= 0.9.1
Requires: vpr-devel = %{vpr_version}
Requires: jccl-c++-devel = %{jccl_version}
Requires: sonix-devel = %{sonix_version}
Requires: gadgeteer-devel = %{gadgeteer_version}
Requires: vrjuggler = %{vrjuggler_version}

%description -n vrjuggler-devel
The header files and libraries needed for developing VR applications using
VR Juggler.

%package -n vrjuggler-opengl
Summary: The VR Juggler OpenGL Draw Manager
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/C++
Requires: libGL.so.1
Requires: libGLU.so.1
Requires: libXxf86vm.so.1
Requires: libXext.so.6
Requires: vrjuggler = %{vrjuggler_version}

%description -n vrjuggler-opengl
The VR Juggler OpenGL Draw Manager, which is needed for running applications
based on OpenGL, OpenSG, and Open Scene Graph.

%package -n vrjuggler-opengl-devel
Summary: The VR Juggler OpenGL Draw Manager Headers
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/C++
Requires: flagpoll >= 0.9.1
Requires: vrjuggler-opengl = %{vrjuggler_version}
Requires: vrjuggler-devel = %{vrjuggler_version}

%description -n vrjuggler-opengl-devel
The header files and libraries needed for compiling VR Juggler applications
that use the VR Juggler OpenGL Draw Manager. Such applications use OpenGL,
OpenSG, or Open Scene Graph.

%package -n vrjuggler-performer
Summary: The VR Juggler OpenGL Performer Draw Manager
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/C++
Requires: vrjuggler = %{vrjuggler_version}

%description -n vrjuggler-performer
The VR Juggler OpenGL Performer Draw Manager, which is needed for running
applications based on OpenGL Performer.

%package -n vrjuggler-performer-devel
Summary: The VR Juggler OpenGL Performer Draw Manager Headers
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/C++
Requires: flagpoll >= 0.9.1
Requires: vrjuggler-performer = %{vrjuggler_version}
Requires: vrjuggler-devel = %{vrjuggler_version}

%description -n vrjuggler-performer-devel
The header files and libraries needed for compiling VR Juggler applications
that use the VR Juggler OpenGL Performer Draw Manager. Such applications use
OpenGL Performer.

%package -n vrjconfig
Summary: VRJConfig, the VR Juggler configuration editor
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/Java
%java_req
Requires: tweek-java = %{tweek_version}
Requires: jccl-java = %{jccl_version}
Requires: jccl-rtrc-plugin-java = %{jccl_version}

%description -n vrjconfig
VRJConfig is a Java-based GUI for editing VR Juggler configurations. It
is actually a JavaBean that plugs into the Tweek Java GUI.

%package -n vrjuggler-perf-plugin-c++
Summary: The VR Juggler Remote Run-Time Performance Monitoring C++ Plug-in
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/C++
Requires: tweek-c++ = %{tweek_version}
Requires: vrjuggler = %{vrjuggler_version}

%description -n vrjuggler-perf-plugin-c++
A plug-in to the VR Juggler Performance Mediator written in C++ that provides
the functionality needed for remote run-time application performance
monitoring through CORBA.

%package -n vrjuggler-perf-plugin-java
Summary: The VR Juggler Remote Run-Time Performance Monitoring JavaBean
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/Java
%java_req
Requires: tweek-java = %{tweek_version}

%description -n vrjuggler-perf-plugin-java
A plug-in to the VR Juggler Performance Mediator written in Java that
provides the functionality needed for remote run-time application performance
monitoring through CORBA.

%package -n juggler
Summary: The Juggler Suite
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
Group: Development/C++
License: LGPL
URL: http://www.vrjuggler.org/
Requires: vpr = %{vpr_version}
Requires: jccl-c++ = %{jccl_version}
Requires: gadgeteer = %{gadgeteer_version}
Requires: sonix = %{sonix_version}
Requires: vrjuggler = %{vrjuggler_version}
Requires: vrjuggler-opengl = %{vrjuggler_version}

%description -n juggler
The Juggler Suite provides virtual reality (VR) software developers with a
collection of application programming interfaces (APIs) that abstract, and
hence simplify, all interface aspects of their program including the display
surfaces, object tracking, selection and navigation, graphics rendering
engines, and graphical user interfaces. An application written with the
Juggler Suite is essentially independent of device, computer platform, and
VR system. Applications based on the Juggler Suite may be run with any
combination of immersive technologies and computational hardware.

%package -n juggler-devel
Summary: The Juggler Suite C++ developer interface
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
Group: Development/C++
License: LGPL
URL: http://www.vrjuggler.org/
Requires: vpr-devel = %{vpr_version}
Requires: jccl-c++-devel = %{jccl_version}
Requires: gadgeteer-devel = %{gadgeteer_version}
Requires: sonix-devel = %{sonix_version}
Requires: vrjuggler-devel = %{vrjuggler_version}
Requires: vrjuggler-opengl-devel = %{vrjuggler_version}
Requires: doozer >= 2.1.6

%description -n juggler-devel
The header files and libraries needed for developing VR applications using
the Juggler Suite.

%package -n juggler-java
Summary: The Juggler Suite Java software
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
Group: Development/Java
License: LGPL
URL: http://www.vrjuggler.org/
Requires: tweek-java = %{tweek_version}
Requires: jccl-java = %{jccl_version}
Requires: jccl-rtrc-plugin-java = %{jccl_version}
Requires: vrjconfig = %{vrjuggler_version}
Requires: vrjuggler-perf-plugin-java = %{vrjuggler_version}

%description -n juggler-java
The Java software from the Juggler Suite including VRJConfig.

%prep
rm -rf %{buildroot}
# For now don't keep unpacking
%setup -q -n %{name}-%{global_version}-%{global_release}-src
#%setup -DT -q -n %{name}-%{global_version}-%{global_release}-src
./configure.pl --with-boost=/usr --with-boost-includes=/usr/include     \
               --with-gmtl=/usr --with-openal=/usr --with-audiere=/usr  \
               --prefix=%{_prefix} %{abi_option} %{?_with_vrpn}         \
               %{?_with_ftd2xx} %{?_with_trackdapi}

%build
make %{?_smp_mflags} BUILD_PROF_C=N BUILD_PROF_CXX=N build

%install
make BUILD_PROF_C=N BUILD_PROF_CXX=N install prefix=%{buildroot}%{_prefix}
# Just in case... stupid scons
find %{buildroot}%{_prefix} -name .sconsign -exec rm {} \;

%clean
rm -rf %{buildroot}

%post -n vpr
/sbin/ldconfig

%post -n tweek-c++
/sbin/ldconfig

%post -n jccl-c++
/sbin/ldconfig

%post -n sonix
/sbin/ldconfig

%post -n gadgeteer
/sbin/ldconfig

%post -n vrjuggler
/sbin/ldconfig

%post -n vrjuggler-opengl
/sbin/ldconfig

%post -n vrjuggler-performer
/sbin/ldconfig

%postun -n vpr
/sbin/ldconfig

%postun -n tweek-c++
/sbin/ldconfig

%postun -n jccl-c++
/sbin/ldconfig

%postun -n sonix
/sbin/ldconfig

%postun -n gadgeteer
/sbin/ldconfig

%postun -n vrjuggler
/sbin/ldconfig

%postun -n vrjuggler-opengl
/sbin/ldconfig

%postun -n vrjuggler-performer
/sbin/ldconfig

%files -n vpr
%defattr(-, root, root)
%{_libdir}/libvpr*.so
%dir %{_libdir}/%{vj_arch}/
%dir %{_libdir}/%{vj_arch}/debug/
%dir %{_libdir}/%{vj_arch}/opt/
%dir %{_libdir}/%{vj_arch}/profiled/
%{_libdir}/%{vj_arch}/libvpr*.so
%{_libdir}/%{vj_arch}/*/libvpr*.so
%doc COPYING.txt modules/vapor/ChangeLog

%files -n vpr-devel
%defattr(-, root, root)
%{_includedir}/vpr-%{vpr_version_dist}/
%{_libdir}/libvpr*.a
%{_libdir}/%{vj_arch}/libvpr*.a
%{_libdir}/%{vj_arch}/*/libvpr*.a
%{_libdir}/flagpoll/boost*.fpc
%{_libdir}/flagpoll/vpr*.fpc
%{_prefix}/share/aclocal/vpr.m4
%{_prefix}/share/aclocal/vrj-helpers.m4
%{_prefix}/share/vpr-%{vpr_version_dist}

%if %have_omniorb
%files -n tweek-c++
%defattr(-, root, root)
%{_libdir}/libtweek*.so
%dir %{_libdir}/%{vj_arch}/
%dir %{_libdir}/%{vj_arch}/debug/
%dir %{_libdir}/%{vj_arch}/opt/
%dir %{_libdir}/%{vj_arch}/profiled/
%{_libdir}/%{vj_arch}/libtweek*.so
%{_libdir}/%{vj_arch}/*/libtweek*.so
%dir %{_prefix}/share/tweek-%{tweek_version_dist}/
%{_prefix}/share/tweek-%{tweek_version_dist}/data
%doc COPYING.txt modules/tweek/ChangeLog

%files -n tweek-c++-devel
%defattr(-, root, root)
%{_includedir}/tweek-%{tweek_version_dist}
%{_libdir}/libtweek*.a
%{_libdir}/%{vj_arch}/libtweek*.a
%{_libdir}/%{vj_arch}/*/libtweek*.a
%{_libdir}/flagpoll/tweek-%{tweek_version_dist}*.fpc
%{_libdir}/flagpoll/tweek-python-%{tweek_version_dist}*.fpc
%{_prefix}/share/aclocal/tweek.m4
%{_prefix}/share/tweek-%{tweek_version_dist}/test
%{_prefix}/share/tweek-%{tweek_version_dist}/tweek.appdefs.mk
%endif

%if %have_java
%files -n tweek-java-jni
%defattr(-, root, root)
%{_prefix}/share/tweek-%{tweek_version_dist}/java/%{tweek_arch}

%files -n tweek-java
%defattr(-, root, root)
%{_bindir}/idl
%{_bindir}/idl.jar
%{_bindir}/logkit-1.2.jar
%{_bindir}/tweek
%{_bindir}/tweek-base.sh
%{_libdir}/flagpoll/tweek-java-%{tweek_version_dist}*.fpc
%{_prefix}/share/tweek-%{tweek_version_dist}/beans
%dir %{_prefix}/share/tweek-%{tweek_version_dist}/java/
%{_prefix}/share/tweek-%{tweek_version_dist}/java/*.jar
%{_prefix}/share/tweek-%{tweek_version_dist}/java/*.txt
%{_prefix}/share/tweek-%{tweek_version_dist}/LICENSE
%{_prefix}/share/tweek-%{tweek_version_dist}/README.source
%doc COPYING.txt modules/tweek/ChangeLog
%endif

%files -n jccl-c++
%defattr(-, root, root)
%{_libdir}/libjccl*.so
%dir %{_libdir}/%{vj_arch}/
%dir %{_libdir}/%{vj_arch}/debug/
%dir %{_libdir}/%{vj_arch}/opt/
%dir %{_libdir}/%{vj_arch}/profiled/
%{_libdir}/%{vj_arch}/libjccl*.so
%{_libdir}/%{vj_arch}/*/libjccl*.so
%dir %{_prefix}/share/jccl-%{jccl_version_dist}/
%{_prefix}/share/jccl-%{jccl_version_dist}/data
%{_prefix}/share/jccl-%{jccl_version_dist}/tools
%doc COPYING.txt modules/jackal/ChangeLog

%files -n jccl-c++-devel
%defattr(-, root, root)
%{_includedir}/jccl-%{jccl_version_dist}
%{_libdir}/libjccl*.a
%{_libdir}/%{vj_arch}/libjccl*.a
%{_libdir}/%{vj_arch}/*/libjccl*.a
%{_libdir}/flagpoll/jccl*.fpc
%{_prefix}/share/aclocal/jccl.m4
%{_prefix}/share/jccl-%{jccl_version_dist}/test

%if %have_omniorb
%files -n jccl-rtrc-plugin-c++
%defattr(-, root, root)
%{_libdir}/jccl-%{jccl_version_dist}
%endif

%if %have_java
%files -n jccl-java
%defattr(-, root, root)
%dir %{_prefix}/share/jccl-%{jccl_version_dist}/beans/
%{_prefix}/share/jccl-%{jccl_version_dist}/beans/jccl_config.*
%{_prefix}/share/jccl-%{jccl_version_dist}/beans/jccl_editors.*
%{_prefix}/share/jccl-%{jccl_version_dist}/java
%doc COPYING.txt modules/jackal/ChangeLog

%files -n jccl-rtrc-plugin-java
%defattr(-, root, root)
%{_prefix}/share/jccl-%{jccl_version_dist}/beans/jccl_rtrc.*
%endif

%files -n sonix
%defattr(-, root, root)
%{_libdir}/libsonix*.so
%dir %{_libdir}/%{vj_arch}/
%dir %{_libdir}/%{vj_arch}/debug/
%dir %{_libdir}/%{vj_arch}/opt/
%dir %{_libdir}/%{vj_arch}/profiled/
%{_libdir}/%{vj_arch}/libsonix*.so
%{_libdir}/%{vj_arch}/*/libsonix*.so
%dir %{_libdir}/sonix-%{sonix_version_dist}/
%dir %{_libdir}/sonix-%{sonix_version_dist}/plugins/
%dir %{_libdir}/sonix-%{sonix_version_dist}/plugins/dbg/
%dir %{_libdir}/sonix-%{sonix_version_dist}/plugins/opt/
%{_prefix}/share/sonix-%{sonix_version_dist}/data
%doc COPYING.txt modules/sonix/ChangeLog

%files -n sonix-devel
%defattr(-, root, root)
%{_includedir}/sonix-%{sonix_version_dist}
%{_libdir}/libsonix*.a
%{_libdir}/%{vj_arch}/libsonix*.a
%{_libdir}/%{vj_arch}/*/libsonix*.a
%{_libdir}/flagpoll/sonix*.fpc
%{_prefix}/share/aclocal/sonix.m4
%{_prefix}/share/sonix-%{sonix_version_dist}/samples

%if %have_audiere
%files -n sonix-plugin-audiere
%defattr(-, root, root)
%{_libdir}/sonix-%{sonix_version_dist}/plugins/*/Audiere_snd.so
%endif

%if %have_openal
%files -n sonix-plugin-openal
%defattr(-, root, root)
%{_libdir}/sonix-%{sonix_version_dist}/plugins/*/OpenAL_snd.so
%endif

%files -n gadgeteer
%defattr(-, root, root)
%{_libdir}/libgadget*.so
%dir %{_libdir}/%{vj_arch}/
%dir %{_libdir}/%{vj_arch}/debug/
%dir %{_libdir}/%{vj_arch}/opt/
%dir %{_libdir}/%{vj_arch}/profiled/
%{_libdir}/%{vj_arch}/libgadget*.so
%{_libdir}/%{vj_arch}/*/libgadget*.so
%dir %{_libdir}/gadgeteer-%{gadgeteer_version_dist}/
%dir %{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/
%dir %{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/
%dir %{_libdir}/gadgeteer-%{gadgeteer_version_dist}/plugins/
%dir %{_libdir}/gadgeteer-%{gadgeteer_version_dist}/plugins/debug/
%{_prefix}/share/gadgeteer-%{gadgeteer_version_dist}/data
%{_prefix}/share/gadgeteer-%{gadgeteer_version_dist}/tools
%doc COPYING.txt modules/gadgeteer/ChangeLog

%files -n gadgeteer-devel
%defattr(-, root, root)
%{_includedir}/gadgeteer-%{gadgeteer_version_dist}/cluster
%{_includedir}/gadgeteer-%{gadgeteer_version_dist}/gadget
%{_includedir}/gadgeteer-%{gadgeteer_version_dist}/plugins
%{_libdir}/libgadget*.a
%{_libdir}/%{vj_arch}/libgadget*.a
%{_libdir}/%{vj_arch}/*/libgadget*.a
%{_libdir}/flagpoll/gadgeteer*.fpc
%{_prefix}/share/aclocal/gadget.m4
%{_prefix}/share/gadgeteer-%{gadgeteer_version_dist}/*.mk
%{_prefix}/share/gadgeteer-%{gadgeteer_version_dist}/samples
%{_prefix}/share/gadgeteer-%{gadgeteer_version_dist}/test

%files -n gadgeteer-driver-5dt-dataglove
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/DataGlove_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/DataGlove_drv.so

%files -n gadgeteer-driver-dtrack
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/DTrack_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/DTrack_drv.so

%files -n gadgeteer-driver-etherio24
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/ElexolEther24_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/ElexolEther24_drv.so

%files -n gadgeteer-driver-flock
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/Flock_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/Flock_drv.so

%files -n gadgeteer-driver-fastrak
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/Fastrak_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/Fastrak_drv.so

%files -n gadgeteer-driver-ibox
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/IBox_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/IBox_drv.so

%files -n gadgeteer-driver-intersense-api
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/IntersenseAPI_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/IntersenseAPI_drv.so

%files -n gadgeteer-driver-is900
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/IS900_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/IS900_drv.so

%files -n gadgeteer-driver-joydev
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/LinuxJoydev_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/LinuxJoydev_drv.so

%files -n gadgeteer-driver-motionstar
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/MotionStar_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/MotionStar_drv.so

%if %have_libusb
%files -n gadgeteer-driver-p5glove
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/P5Glove_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/P5Glove_drv.so
%endif

%files -n gadgeteer-driver-pinchglove
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/PinchGlove_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/PinchGlove_drv.so

%files -n gadgeteer-driver-serial-encoder
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/SerialEncoder_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/SerialEncoder_drv.so

%files -n gadgeteer-driver-spaceball
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/SpaceBall_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/SpaceBall_drv.so

%files -n gadgeteer-driver-threedmouse
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/ThreeDMouse_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/ThreeDMouse_drv.so

%files -n gadgeteer-driver-trackd
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/Trackd_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/Trackd_drv.so

%if %have_trackdapi
%files -n gadgeteer-driver-trackd-api
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/TrackdAPI_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/TrackdAPI_drv.so
%endif

%if %have_vrpn
%files -n gadgeteer-driver-vrpn
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/VRPN_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/VRPN_drv.so
%endif

%files -n gadgeteer-driver-wanda
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/Wanda_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/Wanda_drv.so

%if %have_ftd2xx && %have_libusb
%files -n gadgeteer-driver-x-ist
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/X-IST_drv.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/drivers/debug/X-IST_drv.so
%endif

%files -n gadgeteer-plugin-application-data-manager
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/plugins/ApplicationDataManager.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/plugins/debug/ApplicationDataManager.so

%files -n gadgeteer-plugin-remote-input-manager
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/plugins/RIMPlugin.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/plugins/debug/RIMPlugin.so

%files -n gadgeteer-plugin-start-barrier
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/plugins/StartBarrierPlugin.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/plugins/debug/StartBarrierPlugin.so

%files -n gadgeteer-plugin-swap-lock-tcp
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/plugins/SwapLockTCPPlugin.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/plugins/debug/SwapLockTCPPlugin.so

%files -n gadgeteer-plugin-swap-lock-wired
%defattr(-, root, root)
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/plugins/SwapLockWiredPlugin.so
%{_libdir}/gadgeteer-%{gadgeteer_version_dist}/plugins/debug/SwapLockWiredPlugin.so

%files
%defattr(-, root, root)
%{_libdir}/libvrj-*.so
%dir %{_libdir}/%{vj_arch}/
%dir %{_libdir}/%{vj_arch}/debug/
%dir %{_libdir}/%{vj_arch}/opt/
%dir %{_libdir}/%{vj_arch}/profiled/
%{_libdir}/%{vj_arch}/libvrj-*.so
%{_libdir}/%{vj_arch}/*/libvrj-*.so
%dir %{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/data
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/tools
%doc COPYING.txt INSTALL.html INSTALL.txt README.html README.txt modules/vrjuggler/ChangeLog modules/vrjuggler/RELEASE_NOTES.txt

%files -n vrjuggler-devel
%defattr(-, root, root)
%dir %{_includedir}/vrjuggler-%{vrjuggler_version_dist}/vrj/
%{_includedir}/vrjuggler-%{vrjuggler_version_dist}/vrj/*.h
%{_includedir}/vrjuggler-%{vrjuggler_version_dist}/vrj/Display
%dir %{_includedir}/vrjuggler-%{vrjuggler_version_dist}/vrj/Draw/
%{_includedir}/vrjuggler-%{vrjuggler_version_dist}/vrj/Draw/*.h
%{_includedir}/vrjuggler-%{vrjuggler_version_dist}/vrj/Kernel
%{_includedir}/vrjuggler-%{vrjuggler_version_dist}/vrj/Performance
%{_includedir}/vrjuggler-%{vrjuggler_version_dist}/vrj/Sound
%{_includedir}/vrjuggler-%{vrjuggler_version_dist}/vrj/Test
%{_includedir}/vrjuggler-%{vrjuggler_version_dist}/vrj/Util
%{_libdir}/libvrj-*.a
%{_libdir}/%{vj_arch}/libvrj-*.a
%{_libdir}/%{vj_arch}/*/libvrj-*.a
%{_libdir}/flagpoll/vrjuggler-%{vrjuggler_version}*.fpc
%{_prefix}/share/aclocal/vrj.m4
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/*.mk
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/samples
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/test

%files -n vrjuggler-opengl
%defattr(-, root, root)
%{_libdir}/libvrj_ogl*.so
%{_libdir}/%{vj_arch}/libvrj_ogl*.so
%{_libdir}/%{vj_arch}/*/libvrj_ogl*.so

%files -n vrjuggler-opengl-devel
%defattr(-, root, root)
%{_includedir}/vrjuggler-%{vrjuggler_version_dist}/vrj/Draw/OGL
%{_includedir}/vrjuggler-%{vrjuggler_version_dist}/vrj/Draw/OSG
%{_includedir}/vrjuggler-%{vrjuggler_version_dist}/vrj/Draw/OpenSG
%{_libdir}/libvrj_ogl*.a
%{_libdir}/%{vj_arch}/libvrj_ogl*.a
%{_libdir}/%{vj_arch}/*/libvrj_ogl*.a
%{_libdir}/flagpoll/vrjuggler-opengl-%{vrjuggler_version}*.fpc

%if %have_performer
%files -n vrjuggler-performer
%defattr(-, root, root)
%{_libdir}/libvrj_pf*.so
%{_libdir}/%{_arch}/libvrj_pf*.so
%{_libdir}/%{_arch}/*/libvrj_pf*.so

%files -n vrjuggler-performer-devel
%defattr(-, root, root)
%{_includedir}/vrjuggler-%{vrjuggler_version_dist}/vrj/Draw/Pf
%{_libdir}/libvrj_pf*.a
%{_libdir}/%{vj_arch}/libvrj_pf*.a
%{_libdir}/%{vj_arch}/*/libvrj_pf*.a
%{_libdir}/flagpoll/vrjuggler-performer-%{vrjuggler_version}*.fpc
%endif

%if %have_java
%files -n vrjconfig
%defattr(-, root, root)
%{_bindir}/vrjconfig
%dir %{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/beans/
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/beans/customeditors
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/beans/wizards
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/beans/VRJConfig.*
%dir %{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/java/
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/java/CommonEditors.jar
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/java/jgraphaddons.jar
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/java/jgraph.jar
%doc COPYING.txt modules/vrjuggler/vrjconfig/LICENSE-java-laf.txt
%endif

%if %have_omniorb
%files -n vrjuggler-perf-plugin-c++
%defattr(-, root, root)
%{_libdir}/vrjuggler-%{vrjuggler_version_dist}
%endif

%if %have_java
%files -n vrjuggler-perf-plugin-java
%defattr(-, root, root)
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/beans/PerformanceMonitor.*
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/java/gnujaxp.jar
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/java/jcommon.jar
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/java/jfreechart.jar
%{_prefix}/share/vrjuggler-%{vrjuggler_version_dist}/java/servlet.jar
%endif

%files -n juggler
%defattr(-, root, root)
# Empty RPM

%files -n juggler-devel
%defattr(-, root, root)
# Empty RPM

%if %have_java
%files -n juggler-java
%defattr(-, root, root)
# Empty RPM
%endif

%changelog
* Fri Aug 10 2007 Patrick Hartling <patrick at infiscape dot com>
- Package Gadgeteer drivers and plug-ins individually.

* Wed Apr 25 2007 Patrick Hartling <patrick at infiscape dot com>
- Improve Java version requirement.
- Add jccl-rtrc-plugin-java as a requirement for vrjconfig.

* Thu Mar 15 2007 Patrick Hartling <patrick at infiscape dot com>
- Depend on jdk instead of j2sdk.

* Fri Aug 25 2006 Patrick Hartling
- Removed the creation of an RPM for Doozer. Doozer 2.1.1 is the minimum
  required version now, and it has its own RPM.

* Thu Apr 14 2006 Patrick Hartling
- Split into sub-packages.
- Allow for optional sub-packages depending on the build environment.
- Added support for multi-architecture builds.

* Tue Jun 06 2005 Aron Bierbaum
- Initial version.
