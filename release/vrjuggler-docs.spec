# Spec file for the Juggler documentation.
# Run with the environment variable DOCBOOK_ROOT set to the DocBook
# distribution needed for rendering this documentation. This package can be
# downloaded from the following page:
#
#    http://www.vrjuggler.org/view/Juggler/WritingDocumentationUsingDocBook

%define name vrjuggler-docs
%define global_version 2.3.11
%define global_release 1
%define srcdir vrjuggler-%{global_version}-%{global_release}-src
%define file_name %{srcdir}.tar.bz2

%define vpr_version 2.1.4
%define vpr_release %{global_release}
%define tweek_version 1.3.2
%define tweek_release %{global_release}
%define jccl_version 1.3.3
%define jccl_release %{global_release}
%define sonix_version 1.3.2
%define sonix_release %{global_release}
%define gadgeteer_version 1.3.12
%define gadgeteer_release %{global_release}
%define vrjuggler_version %{global_version}
%define vrjuggler_release %{global_release}

%define have_java %(if [ -x $JDK_HOME/bin/javac ] ; then echo 1; else echo 0; fi)

Name: vrjuggler-docs
Summary: Documentation for VR Juggler, the virtual platform for VR application development
Version: %{global_version}
Release: %{global_release}
Source: %{file_name}
URL: http://www.vrjuggler.org/
Group: Development/Libraries
BuildRoot: %{_tmppath}/%{name}-%{global_version}-%{global_release}-buildroot
License: GNU Free Documentation License
BuildArch: noarch
BuildRequires: doxygen
BuildRequires: graphviz

%description
VR Juggler provides virtual reality (VR) software developers with a suite of
application programming interfaces (APIs) that abstract, and hence simplify,
all interface aspects of their program including the display surfaces, object
tracking, selection and navigation, graphics rendering engines, and graphical
user interfaces. An application written with VR Juggler is essentially
independent of device, computer platform, and VR system. VR Juggler may be run
with any combination of immersive technologies and computational hardware.

%package -n vpr-docs
Summary: Documentation for VPR, the VR Juggler Portable Runtime
Version: %{vpr_version}
Release: %{vpr_release}
URL: http://www.vrjuggler.org/vapor/
Group: Development/C++
BuildArch: noarch
BuildRequires: doxygen
BuildRequires: graphviz

%description -n vpr-docs
The VR Juggler Portable Runtime (VPR) provides an object-oriented
operating system abstraction layer that allows for easy development of
cross-platform C++ applications.

%package -n tweek-c++-docs
Summary: Documentation for the Tweek C++ API
Version: %{tweek_version}
Release: %{tweek_release}
URL: http://www.vrjuggler.org/tweek/
Group: Development/C++
BuildArch: noarch
BuildRequires: doxygen
BuildRequires: graphviz

%description -n tweek-c++-docs
The Tweek C++ API provides a wrapper around CORBA for writing applications
based on the distributed model/view/controller system. In the scope of
VR Juggler, such applications are usually immersive worlds that have a
GUI that communicates with the immersive application through a network
connection.

%package -n tweek-java-docs
Summary: Documentation for the Tweek Java API and GUI
Version: %{tweek_version}
Release: %{tweek_release}
URL: http://www.vrjuggler.org/tweek/
Group: Development/Java
BuildArch: noarch
BuildRequires: jdk

%description -n tweek-java-docs
The Tweek Java API provides a wrapper around CORBA for writing applications
based on the distributed model/view/controller system. In the scope of
VR Juggler, such applications are usually immersive worlds that have a
GUI that communicates with the immersive application through a network
connection. The Tweek Java GUI provides an framework for creating such
GUIs. Application programmers write JavaBeans that plug into the Tweek
Java GUI.

%package -n jccl-c++-docs
Summary: Documentation for the JCCL C++ API
Version: %{jccl_version}
Release: %{jccl_release}
URL: http://www.vrjuggler.org/jccl/
Group: Development/C++
BuildArch: noarch
BuildRequires: doxygen
BuildRequires: graphviz

%description -n jccl-c++-docs
The JCCL C++ API provides a dynamic configuration system based on XML.

%package -n jccl-java-docs
Summary: The JCCL Java API
Version: %{jccl_version}
Release: %{jccl_release}
URL: http://www.vrjuggler.org/jccl/
Group: Development/Java
BuildArch: noarch
BuildRequires: jdk

%description -n jccl-java-docs
The JCCL Java API provides a collection of JavaBeans that are used for
reading and editing JCCL configuration (.jconf) files and config
definition (.jdef) files. The JavaBeans are designed for use with and
depend on the Tweek Java GUI.

%package -n sonix-docs
Summary: Documentation for Sonix
Version: %{sonix_version}
Release: %{sonix_release}
URL: http://www.vrjuggler.org/sonix/
Group: Development/C++
BuildArch: noarch
BuildRequires: doxygen
BuildRequires: graphviz

%description -n sonix-docs
Sonix is a dynamically reconfigurable wrapper around audio APIs.

%package -n gadgeteer-docs
Summary: Documentation for Gadgeteer
Version: %{gadgeteer_version}
Release: %{gadgeteer_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
BuildArch: noarch
BuildRequires: doxygen
BuildRequires: graphviz

%description -n gadgeteer-docs
Gadgeteer is an extensible, highly configurable, cluster-ready input
device management for use with virtual reality applications. Users can
write Device drivers that are loaded dynamically.

%package -n vrjconfig-docs
Summary: Documentation for VRJConfig, the VR Juggler configuration editor
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/Java
BuildArch: noarch
BuildRequires: jdk

%description -n vrjconfig-docs
VRJConfig is a Java-based GUI for editing VR Juggler configurations. It
is actually a JavaBean that plugs into the Tweek Java GUI.

%prep
rm -rf %{buildroot}
# For now don't keep unpacking
%setup -q -n %{srcdir}
./configure.pl

%build
%if %{have_java}
make DIRS="modules/tweek modules/jackal modules/jackal/plugins"
make -f Makefile.docs TWEEK_BASE_DIR=`pwd`/instlinks TWEEK_VERSION=%{tweek_version} JCCL_BASE_DIR=`pwd`/instlinks JCCL_VERSION=%{jccl_version} all
%else
make -f Makefile.docs JAVADOC_SUBDIR= all
%endif

%install
%if %{have_java}
make -f Makefile.docs webroot=%{buildroot} install
%else
make -f Makefile.docs JAVADOC_SUBDIR= webroot=%{buildroot} install
%endif
# This file is not part of the documentation, it is for the website.
rm -f %{buildroot}/tweek/1.1/beanlist.xsd
# Ignore the Juggler build system documentation.
rm -rf %{buildroot}/docs
mkdir -p %{buildroot}%{_docdir}
mv %{buildroot}/vapor/2.1 %{buildroot}%{_docdir}/vpr-%{vpr_version}
mv %{buildroot}/tweek/1.3 %{buildroot}%{_docdir}/tweek-%{tweek_version}
mv %{buildroot}/jccl/1.3 %{buildroot}%{_docdir}/jccl-%{jccl_version}
mv %{buildroot}/sonix/1.3 %{buildroot}%{_docdir}/sonix-%{sonix_version}
mv %{buildroot}/gadgeteer/1.3 %{buildroot}%{_docdir}/gadgeteer-%{gadgeteer_version}
mv %{buildroot}/vrjuggler/2.3 %{buildroot}%{_docdir}/vrjuggler-%{vrjuggler_version}

%clean
rm -rf %{buildroot}

%files -n vpr-docs
%defattr(-, root, root)
%{_docdir}/vpr-%{vpr_version}

%files -n tweek-c++-docs
%defattr(-, root, root)
%{_docdir}/tweek-%{tweek_version}/programmer.guide
%{_docdir}/tweek-%{tweek_version}/programmer.reference.cxx

%if %have_java
%files -n tweek-java-docs
%defattr(-, root, root)
%{_docdir}/tweek-%{tweek_version}/programmer.reference.java
%endif

%files -n jccl-c++-docs
%defattr(-, root, root)
%{_docdir}/jccl-%{jccl_version}/programmer.reference

%if %have_java
%files -n jccl-java-docs
%{_docdir}/jccl-%{jccl_version}/programmer.reference.java
%endif

%files -n sonix-docs
%defattr(-, root, root)
%{_docdir}/sonix-%{sonix_version}

%files -n gadgeteer-docs
%defattr(-, root, root)
%{_docdir}/gadgeteer-%{gadgeteer_version}

%files
%defattr(-, root, root)
%{_docdir}/vrjuggler-%{vrjuggler_version}/getting.started.guide
%{_docdir}/vrjuggler-%{vrjuggler_version}/programmer.guide
%{_docdir}/vrjuggler-%{vrjuggler_version}/programmer.reference

%if %have_java
%files -n vrjconfig-docs
%defattr(-, root, root)
%{_docdir}/vrjuggler-%{vrjuggler_version}/programmer.reference.java
%endif

%changelog
* Wed Apr 18 2007 Patrick Hartling <patrick@infiscape.com> 2.3.0-1
- Roll versions after VR Juggler 2.2 branch creation.

* Thu Mar 15 2007 Patrick Hartling
- Depend on jdk instead of j2sdk.

* Sat Apr 22 2006 Patrick Hartling
- Initial version.
