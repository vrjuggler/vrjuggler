<!--
  Entry for an inherited method in an index of class methods.  It links to
  a more detailed description of the method.
  This file is specific to the VR Juggler documentation but could be modified
  to be used elsewhere.
 -->

<TT>
{nolink true}
<A
    HREF="{defclass}.html#{memberef}"
    TARGET="Body"><B>{mname}</B></A>({args})
{nolink false}
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
