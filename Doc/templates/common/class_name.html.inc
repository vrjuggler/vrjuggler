<!--
  This is a general-purpose include file that simply prints the name of the
  current class/struct/union.  The actual name is printed in bold text.
 -->

{if templ}
  template
{endif}
{if struct}
  struct
{endif}
{else}
  {if union}
    union
  {endif}
  {else}
    class
  {endelse}
{endelse}
<B>{classlinked}{templ}</B>
