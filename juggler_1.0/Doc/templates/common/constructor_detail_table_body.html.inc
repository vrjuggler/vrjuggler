<!--
  This file is specific to the VR Juggler documentation layout and is used
  for filling in the body of the table describing each class constructor.

  $Id$
 -->

<TR>
  <TD>
    <TT>
    {if mprivate}
      private
    {endif}
    {else}
      public
    {endelse}
    <B>{mname}</B>({args}) {if const} const {endif}
    {if throws}
      <FONT COLOR="#ff0000">throws {throwclass}({throwargs})</FONT>
    {endif}
    </TT>
  </TD>
</TR>
{private false}
  {if descr}
    <TR>
      <TD>
	{include ../common/descr_table.html.inc}
      </TD>
    </TR>
  {endif}
  <TR>
    <TD>
      {include ../common/contract_table.html.inc}
    </TD>
  </TR>
{endprivate}
{if ARGS}
  <TR>
    <TD>
      {ARGS}
    </TD>
  </TR>
{endif}
{if RETURNS}
  <TR>
    <TD>
      {RETURNS}
    </TD>
  </TR>
{endif}
