<!--
  Entry for a method in an index of class methods.  It links to a more
  detailed description of the method.
  This file is specific to the VR Juggler documentation but could be modified
  to be used elsewhere.

  $Id$
 -->

<TT>
<A HREF="#{memberef}" TARGET="class_desc"><B>{mname}</B></A>({args})
{if const} const {endif}
{if throws}
  <FONT COLOR="#ff0000">throws {throwclass}({throwargs})</FONT>
{endif}
</TT>
<BR>
{if brief}
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{brief}
  <BR>
{endif}
