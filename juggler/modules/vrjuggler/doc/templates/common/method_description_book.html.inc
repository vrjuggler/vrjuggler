<BLOCKQUOTE>
  {include ../common/member_prototype.html.inc}

  <BLOCKQUOTE>
    {private false}
      {if descr}
	<P>
	  {include ../common/descr.html.inc}
	</P>
      {endif}
      {else}
	{if overrides}
	  <P>
	    {include ../common/descr.html.inc}
	  </P>
	{endif}
      {endelse}

      <P>
	<B>Contract</B>
      </P>

      <BLOCKQUOTE>
	{include ../common/contract.html.inc}
      </BLOCKQUOTE>
    {endprivate}

    {if ARGS}
      {ARGS}
    {endif}
    {if RETURNS}
      {RETURNS}
    {endif}
  </BLOCKQUOTE>
</BLOCKQUOTE>
