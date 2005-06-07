TweekQt
=======

TweekQt is a Tweek-aware graphical user interface (GUI) written in
the Python scripting language using the cross-platform Qt widget set.
It is based on the GPL version of Qt and PyQt, and as such, TweekQt
is also GPL'd software.

TweekQt can communicate with remote applications that use Tweek via
CORBA.  Presently, omniORBpy 2.1 or newer is required for the use of
TweekQt.  It is hoped that, in the future, Fnorb can be used instead
because it is a pure-Python CORBA implementation.  As of this writing
(June 2003), Fnorb does not support all the CORBA features required
for the use of Tweek, and thus it cannot be used as the primary ORB
for TweekQt.

FAQ
===

Q: What bearing does the use of the GNU General Public License have on
   plug-ins written for TweekQt?

A: The plug-ins must also be licensed under the GNU GPL if they will be
   redistributed.

Resources
=========

   Qt -- http://www.trolltech.com/
   PyQt -- http://www.riverbankcomputing.co.uk/pyqt/
   Python -- http://www.python.org/
   omniORB -- http://omniorb.sourceforge.net/
   Fnorb -- http://www.fnorb.org/

$Id$
