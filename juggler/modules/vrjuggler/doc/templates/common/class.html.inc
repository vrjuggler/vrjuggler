<!--
  Main page for each class/struct/union.  It defines the frame layout for
  the page.
  This is intended to be shared between multiple sets of templates.
 -->

<HEAD>
<TITLE>class {classname}{templ}</TITLE>
</HEAD>

<FRAMESET COLS="165,*" >
  <FRAMESET ROWS="190,*">
    <FRAME
	NAME="class_groups" SRC="class_groups.html#{group}" MARGINHEIGHT="5"
	MARGINWIDTH="5" SCROLLING="Auto">
    <FRAME
	NAME="method_list" SRC="{class}_methods.html" MARGINWIDTH="5"
	MARGINHEIGHT="5" SCROLLING="Auto">
  </FRAMESET>

  <FRAMESET ROWS="*,110">
    <FRAME
	NAME="class_desc" SRC="{class}_desc.html" MARGINWIDTH="5"
	MARGINHEIGHT="5" SCROLLING="Auto">
    <FRAME
	NAME="applet" SRC="{class}_appl.html" MARGINHEIGHT="0" MARGINWIDTH="0"
	SCROLLING="No">
  </FRAMESET>
</FRAMESET>
