<!--
  This filles in the table with the description (both brief and detailed) of
  the current member.  The layout is for the VR Juggler documentation, but it
  could be modified to be used elsewhere.  It includes a note about what
  method is overridden if applicable.

  $Id$
 -->

{if brief}
  <TR>
    <TD>
      {brief}
    </TD>
  </TR>
{endif}
{if detail}
  <TR>
    <TD>
      {detail}
    </TD>
  </TR>
{endif}
{if overrides}
  <TR>
    <TD>
      Overrides <B>{mname}</B> in
      <A HREF="{override_parent}.html">{override_parent}</A>.
    </TD>
  </TR>
{endif}
