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
      <P>
	{ARGS}
      </P>
    {endif}
    {if RETURNS}
      <P>
	{RETURNS}
      </P>
    {endif}
  </BLOCKQUOTE>
</BLOCKQUOTE>
