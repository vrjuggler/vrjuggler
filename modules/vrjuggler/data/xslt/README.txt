This directory contains a collection of XSLT stylesheets that can be
used for updating XML-based VR Juggler/JCCL configuration files.  The
update process is iterative with each step corresponding to one release
of VR Juggler.

At some point, we intend to automate the update process inside VRJConfig
using XSLT.  VRJConfig will detect the versions of the contents in the
loaded file and perform an update to the contents if necessary.  This
will only be possible for configuration element versions where XSLT
stylesheets are provided for performing the update.

After you have performed the update, you should load the generated
configuration file using VRJConfig and review your configuration.  Some
additions to the XML content may be made, and generic identifiers are often
used for these additions.  Using VRJConfig, you can make customizations for
your specific configuration.

2.0a1-2.0a2.xsl
---------------
   An XSLT stylesheet that provides the upgrade path for configuration
   files created for VR Juggler 2.0 Alpha 1 to VR Juggler 2.0 Alpha 2.
   Simply use this stylesheet with an XSLT processor to update old
   configuration files.

   To use this stylesheet with the xsltproc command, run xsltproc as
   follows:

      xsltproc -o <output.cfg> 2.0a1-2.0a2.xsl <input.cfg>

   To use it with Xalan, run the xalan wrapper (shell script or batch
   file) as follows:

      xalan -in <input.cfg> -out <output.cfg> -xsl 2.0a1-2.0a2.xsl

   Fill in your input filename and output filename appropriately.

2.0a2-2.0a4.xsl
---------------
   An XSLT stylesheet that provides the upgrade path for configuration
   files created for VR Juggler 2.0 Alpha 2 or VR Juggler 2.0 Alpha 3
   to VR Juggler 2.0 Alpha 4.  Simply use this stylesheet with an XSLT
   processor to update old configuration files.

   To use this stylesheet with the xsltproc command, run xsltproc as
   follows:

      xsltproc -o <output>.jconf 2.0a2-2.0a4.xsl <input.cfg>

   To use it with Xalan, run the xalan wrapper (shell script or batch
   file) as follows:

      xalan -in <input.cfg> -out <output>.jconf -xsl 2.0a2-2.0a4.xsl

   Fill in your input filename and output filename appropriately.  Note
   that the output file should end with the extension ".jconf" for 2.0
   Alpha 4 and beyond.

2.0a4-2.0b1.xsl
---------------
   An XSLT stylesheet that provides the upgrade path for configuration
   files created for VR Juggler 2.0 Alpha 4 to VR Juggler 2.0 Beta 1.
   Simply use this stylesheet with an XSLT processor to update old
   configuration files.

   To use this stylesheet with the xsltproc command, run xsltproc as
   follows:

      xsltproc -o <output>.jconf 2.0a4-2.0b1.xsl <input>.jconf

   To use it with Xalan, run the xalan wrapper (shell script or batch
   file) as follows:

      xalan -in <input>.jconf -out <output>.jconf -xsl 2.0a4-2.0b1.xsl

   Fill in your input filename and output filename appropriately.  Note
   that the output file should end with the extension ".jconf" for 2.0
   Alpha 4 and beyond.

key_sym_2_0.xsl
---------------

   An XSLT stylesheet that provides assistance in migrating from VR Juggler
   2.0 to 2.1 and beyond. VRJConfig has a bug where it will not automatically
   replace numeric values with symbolic values when the config definition
   uses value enumeration. For new configurations, it uses the symbolic
   value, but if a configuration has been migrated forward repeatedly, it may
   contain numeric values. In the case of keyboard keys, mouse buttons, and
   mouse motions, the numeric values changed during VR Juggler 2.1
   development, and as a result, VR Juggler 2.0 configurations using
   keyboard/mouse input may not behave as expected with VR Juggler 2.1 and
   newer. To help work around this, this transform can be applied to VR
   Juggler 2.0 configurations to replace the numeric key/mouse values with
   the symbolic values so that they will migrate forward seamlessly.

   To use this stylesheet with the xsltproc command, run xsltproc as
   follows:

      xsltproc -o <output>.jconf key_sym_2_0.xsl <input>.jconf

   To use it with Xalan, run the xalan wrapper (shell script or batch
   file) as follows:

      xalan -in <input>.jconf -out <output>.jconf -xsl key_sym_2_0.xsl

   Fill in your input filename and output filename appropriately.
   
   In general, this transform should be applied to all VR Juggler 2.0
   configuration files that contain key_modifier_pair, input_window, and
   display_window config elements. After applying the transform, the 2.0
   configuration files will continue to work with VR Juggler 2.0, and they
   will be ready for migration to VR Juggler 2.2 using VRJConfig.

Patrick Hartling <patrick@infiscape.com>
$Id$
