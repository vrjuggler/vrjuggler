<!--
  This fills in the description (both brief and detailed) of the current
  member.  The layout is for the VR Juggler documentation, but it could be
  modified to be used elsewhere.  It includes a note about what method is
  overridden if applicable.
 -->

{if brief}
  {brief}
{endif}
{if detail}
  {detail}
{endif}
{if overrides}
  Overrides <B>{mname}</B> in
  <A HREF="{override_parent}.html">{override_parent}</A>.
{endif}
