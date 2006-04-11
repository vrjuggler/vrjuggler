This directory contains a collection of XSLT stylesheets that can be
used for updating XML-based VR Juggler/JCCL configuration files.  The
update process is iterative with each step corresponding to one release
of VR Juggler.

At some point, we intend to automate the update process inside VRJConfig
using XSLT.  VRJConfig will detect the format of the loaded file and
perform an update to the contents if necessary.  This will only be possible
for configuration element versions where XSLT stylesheets are provided for
performing the update.

2.0a1-2.0a2.xsl
---------------
   An XSLT stylesheet that provides the upgrade path for configuration
   files created for VR Juggler 2.0 Alpha 1 to VR Juggler 2.0 Alpha 2.
   Simply use this stylesheet with an XSLT processor to update old
   configuration files.
   
   To use this stylesheet with the xsltproc command, run xsltproc as
   follows:

      xsltproc -o <output.desc> 2.0a1-2.0a2.xsl <input.desc>

   To use it with Xalan, run the xalan wrapper (shell script or batch
   file) as follows:

      xalan -in <input.desc> -out <output.desc> -xsl 2.0a1-2.0a2.xsl

   Fill in your input filename and output filename for appropriately.

   Once you have performed the update, you should load the generated
   configuration file using VRJConfig and review your configuration.
   Some additions to the XML content will be made, and generic identifiers
   are often used for these additions.  Using VRJConfig, you can make
   customizations for your specific configuration.

Patrick Hartling <patrick@vrjuggler.org>
$Id$
