<!--
  Method listing (both inherited and non-inherited).  If this is inserted into
  a foreach loop of class members, each method (including inherited methods if
  looping over all members) will be listed with arguments and a link to that
  method's description.
  This can be used anywhere looping over members is done.
 -->

{if func}
  {if !inherited}
    <NOBR>
    &nbsp;&nbsp;&nbsp;&nbsp;<A HREF="{class}_desc.html#{memberef}">{mname}</A>({args})
    </NOBR>
    <BR>
  {endif}
  {else}
    {if !overridden}
      {nolink true}
      <NOBR>
      &nbsp;&nbsp;&nbsp;&nbsp;<A HREF="{defclass}_desc.html#{memberef}">{mname}</A>({args})
      </NOBR>
      <BR>
      {nolink false}
    {endif}
  {endelse}
{endif}
