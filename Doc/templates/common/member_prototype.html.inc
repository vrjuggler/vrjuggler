<!--
  This fills in the prototype for a class member prototype for the current
  member.  The layout is for the VR Juggler documentation, but it could be
  modified to be used elsewhere.
 -->

{include ../common/member_type.html.inc}
<TT>
<B>{mname}</B>({args}) {if const} const {endif}
{if throws}
<FONT COLOR="#ff0000">throws {throwclass}({throwargs})</FONT>
{endif}
</TT>
