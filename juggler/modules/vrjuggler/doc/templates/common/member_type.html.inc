<!--
  Print the type and visibility of the current member in typewriter font.
  This is a general purpose file that can be used anywhere necessary.

  $Id$
 -->

<TT>
{if mprotected}
  protected
{endif}
{else}
  {if mprivate}
    private
  {endif}
  {else}
    {if mfriend}
      friend
    {endif}
    {else}
      private
    {endelse}
  {endelse}
{endelse}
{type}
</TT>
