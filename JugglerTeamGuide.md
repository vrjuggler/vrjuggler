# Juggler Project Team Guide #

Do you want to contribute to a cool project? Or do you want to learn more about VR, cross-platform techniques, and system abstractions? Then this is your guide. This book is for those people who wish to go further with their VR Juggler work by contributing to the project and even becoming full-fledged developers with commit access.

For those people interested in joining the VR Juggler developer team, you should read Part I, "Introduction" and Part II, "Compiling and Installing". Once you have read those and decide you would like to get commit access, refer to the section called "How to Become a VR Juggler Committer".

# Required Reading #

By now you should have read the _Getting Started Guide_ so that you know about environment variables and about compiling VR Juggler applications. It is expected that you already know about compilers and linkers and shells and most everything else developers of UNIX and Windows software deal with on a regular basis.

  * Our [coding standard](VrJugglerCppCodingStandard.md).
  * _Design Patterns_ (Book: ISBN 0-201-63361-2) and Design Patterns in general
  * Autoconf documentation.
  * Automake documentation. (Try executing **info automake**).
  * GNU make documentation. (Try doing a **man** or **info** on `make` or `gmake`. There is a good book on using make by O'Reilly and Associates).
  * Perl documentation. (Try doing a man on perl. There are several good books on Perl by O'Reilly and Associates).
  * Python documentation.
  * Doxygen documentation (used in the various modules comprising VR Juggler to auto-generate documentation for C++ classes).
  * Subversion documentation (used for revision control).

# Introduction #

## VR Juggler Virtual Platform: Mission Statement ##

VR technology as a whole is stagnating with its closed source solutions. This means that anyone new to VR may need to develop a new solution, often inferior than others. Releasing an open VR software system is vital to the development of VR. Instead of reimplementing VR software, people need to focus on implementing new VR ideas and methods so that VR technology can mature. Now, because of VR Juggler, people can focus on implementing these new VR ideas and methods for the future because this technology is readily available to all, free from cost or legal ties.

VR Juggler is a platform for virtual reality applications. The name comes from the delicate balance that must be struck between flexibility, performance, and ease of use. You develop your application once for VR Juggler, then anywhere that VR Juggler is supported, your application is also supported.

VR Juggler's design addresses several key requirements for a virtual reality development system:

  * Performance
  * Flexibility
  * Rapid application development
  * Ease of use
  * Extensibility
  * Portability

Flexibility is achieved by placing common abstractions over I/O devices. New devices can be added easily, and existing devices can be reconfigured or replaced, even while an application is running. To allow optimal performance, applications are given direct access to graphics APIs (currently including OpenGL, OpenGL Performer, OpenSceneGraph, and OpenSG). VR Juggler includes built-in support for performance monitoring of applications and graphics subsystems. It supports multiple-processor machines and will support distributing applications across multiple machines.

Using VR Juggler to develop applications is kept as simple as possible. Small base classes provide a skeleton for application development, while the abstractions of I/O devices simplify programming. During run-time, any VR Juggler application can be controlled or reconfigured by a Java-based graphical interface.

VR Juggler is designed to support a wide array of VR hardware on a variety of architectures. Several tracking systems, gloves, and input devices are already supported. VR Juggler supports projection- based displays such as the CAVE&trade;, and includes support for head-mounted devices. VR Juggler 2.0 is known to run on IRIX, Linux, Windows, Solaris, FreeBSD, HP-UX, and Mac OS X.

  * Compiling and installing

# Programming #

  * [C++ coding standard](VrJugglerCppCodingStandard.md)
  * VersionBumping

# Documentation #

  * [Writing documentation using DocBook](WritingDocumentationUsingDocBook.md)
  * [Writing documentation using Doxygen](WritingDocumentationUsingDoxygen.md)

# Contributing #

## Joining the Team ##

  * 