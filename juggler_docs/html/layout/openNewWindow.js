// Opens a new widow without decorations.
// size    - 'full' for normal browser, 'minimal', for just a pane, 'medium' for minimal plus a back button
// address - give the url to open in the window
// target  - give the window a title
// width&height - window size in pixels
function openNewWindow( size, address, target, width, height )
{
	if (size == 'full')
	{
		window.open( address,top );
   	} 
	
	else if (size == 'minimal')
	{
		window.open( address,target,'width='+width+',height='+height+',status=0,location=0,resizeable=yes,toolbar=no' );
   	} 
	
	else if (size == 'medium')
	{
		window.open( address,target,'width='+width+',height='+height+',status=0,location=0,resizeable=yes,toolbar=yes' );
   	} 
   
   
	
}
