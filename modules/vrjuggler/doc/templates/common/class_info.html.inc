<!--
  Common class/struct/union information file.  First, the name of the
  class/struct/union is printed in large, bold text (preceded by the path
  to its definition if available).  Then, the name is printed again in
  smaller text followed by a list of parents if it is a class, then a brief
  descrpition (if available), then a detailed description (if available),
  and finally any extra information.
  This can typically be used at the start of a class/struct/union description
  page.

  $Id$
 -->

<FONT SIZE="-1"><B>{path}</B></FONT><BR>
<FONT SIZE="+3"><B>
  {include ../common/class_name.html.inc}
</B></FONT>

{if children}
  <P>

  <B>Known direct subclasses:</B>
  {foreach child}
    <A HREF="{name}.html">{name}</A>{if !last}, {endif}
  {next child}
{endif}

<P>
<HR>
<P>

{include ../common/class_name.html.inc}

{if class}
  {if parents}
    <BR>
    Sub class of: {parents}
  {endif}
{endif}

{if brief}
  <P>
  {brief}
{endif}

{if detail}
  <P>
  {detail}
{endif}

{if italic}
  <P>
  <I>{italic}</I>
{endif}

{if NOTE}
  <P>
  <B>NOTE:</B> {NOTE}
{endif}
