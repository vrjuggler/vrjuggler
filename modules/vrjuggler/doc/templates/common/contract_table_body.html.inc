<!--
  This file is specific to the VR Juggler documentation layout and template
  information describing a method's contract.  It fills in the body of the
  table for the current method's contract (PRE, POST, MODIFIES and NOTE).

  $Id$
 -->

{if MODIFIES}
  <TR>
    <TD ALIGN="RIGHT" WIDTH="10%">
      <B>Modifies</B>
    </TD>
    <TD WIDTH="90%">
      {MODIFIES}
    </TD>
  </TR>
{endif}
{if PRE}
  <TR>
    <TD ALIGN="RIGHT" WIDTH="10%">
      <B>Pre</B>
    </TD>
    <TD WIDTH="90%">
      {PRE}
    </TD>
  </TR>
{endif}
{if POST}
  <TR>
    <TD ALIGN="RIGHT" WIDTH="10%">
      <B>Post</B>
    </TD>
    <TD WIDTH="90%">
      {POST}
    </TD>
  </TR>
{endif}
{if NOTE}
  <TR>
    <TD ALIGN="RIGHT" WIDTH="10%">
      <B>Note</B>
    </TD>
    <TD WIDTH="90%">
      {NOTE}
    </TD>
  </TR>
{endif}
