This directory contains a script for updating XML-based VR Juggler/JCCL
configuration files.  Information about the script follows:

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

Patrick Hartling <patrick@vrjuggler.org>
June 2002
