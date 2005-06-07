This directory contains a collection of tools for updating XML-based
VR Juggler/JCCL configuration files.  The update process is iterative
with each step corresponding to one version of the XML file format.

At some point, we intend to automate the update process inside VRJConfig
using XSLT.  VRJConfig will detect the format of the loaded file and
perform an update to the format if necessary.  This will only be possible
for file format versions where XSLT stylesheets are provided for
performing the update.

xmlupdate.py
------------
   Between August 2001 and April 2002, the first iteration on XML-based
   configuration files was under way for VR Juggler (and later JCCL).  In
   May 2002, the second iteration was implemented, and the format used
   between August 2001 and April 2002 is no longer supported.  Files using
   that format can be updated using this Python script.  Simply run this
   script and pass one or more config files on the command line:

      ./xmlupdate.py file1.config file2.config
      ./xmlupdate.py *.config

   The new files will have the same name as the original but with ".new"
   appended.  For example, the updated version of file.config will be
   file.config.new.

   It is strongly recommended that the newly generated file be loaded with
   VRJConfig.  As of this writing (June 9. 2002), the JCCL C++ code does
   not support evaluating symbolic enumeration strings.  In other words,
   strings such as MOUSE_NEGX that may appear as property values have no
   meaning.  The procedure to work around this is as follows:

      1. Open the new file with VRJConfig.
      2. Double-click on each chunk.
      3. Press the "OK" button.
      4. Save the file.

   We expect that a future version of JCCL will support symbolic values
   again, and these instructions will be updated at that time.

   This script does not update chunk descriptions.  At this time, it only
   handles those documents whose root is <ConfigChunkDB>.

desc_2.0-2.1.xsl
----------------
   An XSLT stylesheet that provides the upgrade path from version 2.0 of
   the configuration definition file format to version 2.1.  Simply use
   this stylesheet with an XSLT processor to update old configuration
   definition files.
   
   The old files must have the following processing instruction:

      <?org-vrjuggler-jccl-settings desc.db.version="2.0" ?>

   The stylesheet uses this instruction to be sure that its input is
   the correct version.  Version 2.0 was in use between May 24, 2002,
   and July 17, 2002.  Any definition files created using VjControl
   between that time qualify as using version 2.0 of the file format,
   though they may not include the above processing instruction.  In
   that case, the processing instruction should be added as the second
   line of the source input definition file(s).

   To use this stylesheet with the xsltproc command, run xsltproc as
   follows:

      xsltproc -o <output.desc> desc_2.0-2.1.xsl <input.desc>

   To use it with Xalan, run the xalan wrapper (shell script or batch
   file) as follows:

      xalan -in <input.desc> -out <output.desc> -xsl desc_2.0-2.1.xsl

   Fill in your input filename and output filename appropriately.

   Alternatively, you may use the makefile found in this directory as
   follows:

     make INPUT_FILE=<input.desc> OUTPUT_FILE=<output.desc> desc-update-2.0-2.1

   This makefile will work with any implementation of make (including
   Microsoft's nmake utility).  You may need to edit the makefile to
   provide a path to the 'xsltproc' or 'xalan' executables.

   Once you have updated the file, you should load the generated
   definition file using VRJConfig and review your configuration
   definitions.  Some additions to the XML content will be made, and
   generic identifiers are often used for these additions.  Using
   VRJConfig, you can make customizations for your specific
   configuration types.

desc_2.1-2.2.xsl
----------------
   An XSLT stylesheet that provides the upgrade path from version 2.1 of
   the configuration definition file format to version 2.2.  Simply use
   this stylesheet with an XSLT processor to update old configuration
   definition files.
   
   The old files must have the following processing instruction:

      <?org-vrjuggler-jccl-settings desc.db.version="2.1" ?>

   The stylesheet uses this instruction to be sure that its input is
   the correct version.  Version 2.1 was in use between July 17, 2002,
   and January 15, 2003.  Any definition files created using VRJConfig
   between that time qualify as using version 2.1 of the file format,
   though they may not include the above processing instruction.  In
   that case, the processing instruction should be added as the second
   line of the source input definition file(s).

   To use this stylesheet with the xsltproc command, run xsltproc as
   follows:

      xsltproc -o <output.desc> desc_2.1-2.2.xsl <input.desc>

   To use it with Xalan, run the xalan wrapper (shell script or batch
   file) as follows:

      xalan -in <input.desc> -out <output.desc> -xsl desc_2.1-2.2.xsl

   Fill in your input filename and output filename appropriately.

   Alternatively, you may use the makefile found in this directory as
   follows:

     make INPUT_FILE=<input.desc> OUTPUT_FILE=<output.desc> desc-update-2.1-2.2

   This makefile will work with any implementation of make (including
   Microsoft's nmake utility).  You may need to edit the makefile to
   provide a path to the 'xsltproc' or 'xalan' executables.

   Once you have updated the file, you should load the generated
   definition file using VRJConfig and review your configuration
   definitions.  Some additions to the XML content will be made, and
   generic identifiers are often used for these additions.  Using
   VRJConfig, you can make customizations for your specific
   configuration types.

desc_2.2-2.3.xsl
----------------
   An XSLT stylesheet that provides the upgrade path from version 2.2 of
   the configuration definition file format to version 2.3.  Simply use
   this stylesheet with an XSLT processor to update old configuration
   definition files.
   
   The old files must have the following processing instruction:

      <?org-vrjuggler-jccl-settings desc.db.version="2.2" ?>

   The stylesheet uses this instruction to be sure that its input is
   the correct version.  Version 2.2 was in use between January 15, 2003,
   and February 27, 2003.  Any definition files created using VRJConfig
   between that time qualify as using version 2.2 of the file format,
   though they may not include the above processing instruction.  In
   that case, the processing instruction should be added as the second
   line of the source input definition file(s).

   To use this stylesheet with the xsltproc command, run xsltproc as
   follows:

      xsltproc -o <output.desc> desc_2.2-2.3.xsl <input.desc>

   To use it with Xalan, run the xalan wrapper (shell script or batch
   file) as follows:

      xalan -in <input.desc> -out <output.desc> -xsl desc_2.2-2.3.xsl

   Fill in your input filename and output filename appropriately.

   Alternatively, you may use the makefile found in this directory as
   follows:

     make INPUT_FILE=<input.desc> OUTPUT_FILE=<output.desc> desc-update-2.2-2.3

   This makefile will work with any implementation of make (including
   Microsoft's nmake utility).  You may need to edit the makefile to
   provide a path to the 'xsltproc' or 'xalan' executables.

   Once you have updated the file, you should load the generated
   definition file using VRJConfig and review your configuration
   definitions.  Some additions to the XML content will be made, and
   generic identifiers are often used for these additions.  Using
   VRJConfig, you can make customizations for your specific
   configuration types.

desc_2.3-3.0.xsl
----------------
   An XSLT stylesheet that provides the upgrade path from version 2.3 of
   the configuration definition file format to version 3.0.  Simply use
   this stylesheet with an XSLT processor to update old configuration
   definition files.
   
   The old files must have the following processing instruction:

      <?org-vrjuggler-jccl-settings desc.db.version="2.3" ?>

   The stylesheet uses this instruction to be sure that its input is
   the correct version.  Version 2.3 was in use between February 27, 2003,
   and July 10, 2003.  Any definition files created using VRJConfig between
   that time qualify as using version 2.3 of the file format, though they
   may not include the above processing instruction.  In that case, the
   processing instruction should be added as the second line of the source
   input definition file(s).

   To use this stylesheet with the xsltproc command, run xsltproc as
   follows:

      xsltproc -o junk desc_2.3-3.0.xsl <input.desc>

   To use it with Xalan, run the xalan wrapper (shell script or batch
   file) as follows:

      xalan -in <input.desc> -out junk -xsl desc_2.3-3.0.xsl

   Fill in your input filename appropriately.  The output file name is
   ignored with this transform because new files are named based on the
   config definition type.  The new files are generated in a subdirectory
   named 'definitions'.

   Alternatively, you may use the makefile found in this directory as
   follows:

     make INPUT_FILE=<input.desc> OUTPUT_FILE=junk desc-update-2.3-3.0

   This makefile will work with any implementation of make (including
   Microsoft's nmake utility).  You may need to edit the makefile to
   provide a path to the 'xsltproc' or 'xalan' executables.

   Once you have updated the file, you should load the generated
   definition files using VRJConfig and review them.  Many changes to the
   XML structure will be made, and generic identifiers may be used for
   new additions.  Using VRJConfig, you can make customizations for your
   specific configuration types.

jdef_3.0-3.1.xsl
----------------
   An XSLT stylesheet that provides the upgrade path from version 3.0 of
   the configuration definition file format to version 3.1.  Simply use
   this stylesheet with an XSLT processor to update old configuration
   definition (.jdef) files.
   
   The old files must have the following processing instruction:

      <?org-vrjuggler-jccl-settings definition.version="3.0" ?>

   The stylesheet uses this instruction to be sure that its input is
   the correct version.  Version 3.0 was in use between July 10, 2003,
   and September 17, 2004.  Any definition files created using VRJConfig
   between that time qualify as using version 3.0 of the file format,
   though they may not include the above processing instruction.  In that
   case, the processing instruction should be added as the second line of
   the source input definition file(s).

   To use this stylesheet with the xsltproc command, run xsltproc as
   follows:

      xsltproc -o <output.jdef> jdef_3.0-3.1.xsl <input.jdef>

   To use it with Xalan, run the xalan wrapper (shell script or batch
   file) as follows:

      xalan -in <input.jdef> -out <output.jdef> -xsl desc_3.0-3.1.xsl

   Fill in your input and output file names appropriately.

   Alternatively, you may use the makefile found in this directory as
   follows:

     make INPUT_FILE=<input.jdef> OUTPUT_FILE=<output.jdef> jdef-update-3.0-3.1

   This makefile will work with any implementation of make (including
   Microsoft's nmake utility).  You may need to edit the makefile to
   provide a path to the 'xsltproc' or 'xalan' executables.

   Once you have updated the file, you should load the generated
   definition file using VRJConfig and review it.  The changes made to
   the file structure are slight, affecting only enumerations of allowed
   values for properties.  Using VRJConfig, you can make customizations
   for your specific configuration types.

cfg_2.0-2.1.xsl
---------------
   An XSLT stylesheet that provides the upgrade path from version 2.0 of
   the configuration file format to version 2.1.  Simply use this
   stylesheet with an XSLT processor to update old configuration files.
   
   The old files must have the following processing instruction:

      <?org-vrjuggler-jccl-settings config.db.version="2.0" ?>

   The stylesheet uses this instruction to be sure that its input is
   the correct version.  Version 2.0 was in use between May 24, 2002,
   and September 11, 2002.  Any configuration files created using
   VjControl or VRJConfig between that time qualify as using version
   2.0 of the file format, though they may not include the above
   processing instruction.  In that case, the processing instruction
   should be added as the second line of the source input definition
   file(s).

   To use this stylesheet with the xsltproc command, run xsltproc as
   follows:

      xsltproc -o <output.desc> cfg_2.0-2.1.xsl <input.desc>

   To use it with Xalan, run the xalan wrapper (shell script or batch
   file) as follows:

      xalan -in <input.desc> -out <output.desc> -xsl cfg_2.0-2.1.xsl

   Fill in your input filename and output filename appropriately.

   Alternatively, you may use the makefile found in this directory as
   follows:

     make INPUT_FILE=<input.desc> OUTPUT_FILE=<output.desc> cfg-update-2.0-2.1

   This makefile will work with any implementation of make (including
   Microsoft's nmake utility).  You may need to edit the makefile to
   provide a path to the 'xsltproc' or 'xalan' executables.

   Once you have updated the file, you should load the generated
   config file using VRJConfig and review your configuration(s).  The only
   changes should be to XML elements that are used to reference external
   files.  There may also be some whitespace differences depending on the
   XSLT processor used.

Patrick Hartling <patrick@vrjuggler.org>
$Id$
