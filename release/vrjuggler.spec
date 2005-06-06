# Spec file for vrjuggler.
%define name    vrjuggler
%define version	2.0
%define release	beta3

Name: %{name}
Summary: VR Juggler
Version: %{version}
Release: %{release}
Source: %{name}-%{version}-%{release}.src.tar.bz2
URL: http://www.vrjuggler.org
Group: System Environment/Libraries
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot
License: LGPL

Requires: cppdom >= 0.3.3
Requires: cppdom-devel >= 0.3.3
Requires: gmtl >= 0.4.5
Requires: boost >= 1.32
Requires: omniORB >= 4.0.6-2
#Requires: e2fsprogs-devel
BuildRequires: omniORB >= 4.0.6-2
BuildRequires: omniORB-devel >= 4.0.6-2
BuildRequires: e2fsprogs-devel
BuildRequires: glibc-devel
BuildRequires: boost-devel >= 1.32
BuildRequires: cppdom-devel >= 0.3.3
BuildRequires: gmtl >= 0.4.5
BuildRequires: j2sdk >= 1.4.2
Provides: vrjuggler = %{version}-%{release}
AutoReqProv: no
Prefix: /usr

%description
VR Juggler provides virtual reality (VR) software developers with a suite of
application programming interfaces (APIs) that abstract, and hence simplify,
all interface aspects of their program including the display surfaces, object
tracking, selection and navigation, graphics rendering engines, and graphical
user interfaces. An application written with VR Juggler is essentially
independent of device, computer platform, and VR system. VR Juggler may be run
with any combination of immersive technologies and computational hardware.

%package devel
Summary: The VR Juggler Headers
Group: System Environment/Libraries
Requires: vrjuggler = %{version}-%{release}
Requires: boost-devel >= 1.32
Provides: vrjuggler-devel = %{version}-%{release}

%description devel
The header files and libraries needed for developing programs using VR Juggler.

%prep
rm -rf $RPM_BUILD_ROOT
# For now don't keep unpacking
%setup -q -n %{name}-%{version}-%{release}.src
#%setup -DT -q -n %{name}-%{version}-%{release}.src

%build
# Don't need to run autogen with most tarballs since we run it before compressing it.
#./autogen.sh
./configure.pl --with-boost=/usr --with-boost-includes=/usr/include --with-boost-fs-lib=boost_filesystem-gcc-mt --with-gmtl=/usr --with-gmtl-includes=/usr/include --prefix=$RPM_BUILD_ROOT/usr
gmake instprefix=/usr NO_CLEAN=1 release

%install
#gmake install

%clean
rm -rf $RPM_BUILD_ROOT

%pre

%post

%preun

%postun

%files
%defattr(-, root, root)
/usr/bin
%{_libdir}
/usr/share

%files devel
%defattr(-, root, root)
/usr/include

%doc COPYING.txt INSTALL.html INSTALL.txt README.html README.txt modules/vrjuggler/ChangeLog modules/vrjuggler/RELEASE_NOTES.txt

%changelog
