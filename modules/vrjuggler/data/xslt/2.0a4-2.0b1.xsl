<?xml version="1.0" encoding="UTF-8"?>

<!-- *********** <auto-copyright.pl BEGIN do not edit this line> *********** -->
<!--
 VR Juggler is (C) Copyright 1998-2008 by Iowa State University

 Original Authors:
   Allen Bierbaum, Christopher Just,
   Patrick Hartling, Kevin Meinert,
   Carolina Cruz-Neira, Albert Baker

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public
 License along with this library; if not, write to the
 Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 Boston, MA 02111-1307, USA.
 -->
<!-- ************ <auto-copyright.pl END do not edit this line> ************ -->

<!--

   This stylesheet provides an upgrade path for VR Juggler 2.0 Alpha 4
   configuration files to changes made for VR Juggler 2.0 Beta 1.

      xsltproc -o new-file.jconf 2.0a4-2.0b1.xsl old-file.jconf

   or

      xalan -in old-file.jconf -xsl 2.0a4-2.0b1.xsl -out new-file.jconf

-->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xmlns:jconf="http://www.vrjuggler.org/jccl/xsd/3.0/configuration">
   <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

   <!-- Define a handy way to insert newlines when necessary. -->
   <xsl:variable name="newline">
<xsl:text>
</xsl:text>
   </xsl:variable>

   <!-- The namespace for JCCL configuration files. -->
   <xsl:variable name="jconf_schema">
      <xsl:text>http://www.vrjuggler.org/jccl/xsd/3.0/configuration</xsl:text>
   </xsl:variable>

   <!-- The schema for JCCL configuration files. -->
   <xsl:variable name="jconf_schema_file">
      <xsl:text>http://www.vrjuggler.org/jccl/xsd/3.0/configuration.xsd</xsl:text>
   </xsl:variable>

   <xsl:template match="/">
     <!-- Add the file format version information. -->
     <xsl:processing-instruction name="org-vrjuggler-jccl-settings">configuration.version="3.0"</xsl:processing-instruction>
      <xsl:value-of select="$newline"/>

      <xsl:apply-templates />

      <xsl:message>
         <xsl:text>Processing is complete.  You should now use VRJConfig</xsl:text>
         <xsl:value-of select="$newline"/>
         <xsl:text>to verify the results and make any necessary changes.</xsl:text>
         <xsl:value-of select="$newline"/>
      </xsl:message>
   </xsl:template>

   <xsl:template match="jconf:configuration">
      <xsl:copy>

         <!-- XXX: Why doesn't xsl:copy copy these two attributes? -->
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="xsl:schemaLocation"
                        namespace="http://www.w3.org/2001/XMLSchema-instance">
            <xsl:value-of select="@xsi:schemaLocation"/>
         </xsl:attribute>

         <xsl:apply-templates />
      </xsl:copy>
   </xsl:template>

   <xsl:template match="jconf:include">
      <xsl:copy-of select="." />
   </xsl:template>

   <xsl:template match="jconf:elements">
      <xsl:copy>
         <xsl:apply-templates />
      </xsl:copy>
   </xsl:template>

   <!-- Renames the RemoteInputManager plugin to RIMPlugin. -->
   <xsl:template match="jconf:cluster_manager">
      <xsl:element name="cluster_manager">
         <xsl:attribute name="name">
            <xsl:value-of select="@name" />
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:value-of select="@version" />
         </xsl:attribute>

         <xsl:copy-of select="./jconf:plugin_path" />

         <xsl:for-each select="./jconf:plugin">
            <xsl:variable name="plugin_name">
               <xsl:value-of select="." />
            </xsl:variable>
            <xsl:choose>
               <xsl:when test="$plugin_name = 'RemoteInputManager'">
                  <xsl:element name="plugin">
                     <xsl:text>RIMPlugin</xsl:text>
                  </xsl:element>
               </xsl:when>
               <xsl:otherwise>
                  <xsl:copy-of select="." />
               </xsl:otherwise>
            </xsl:choose>
         </xsl:for-each>

         <xsl:copy-of select="./jconf:cluster_node" />
      </xsl:element>
   </xsl:template>

   <xsl:template match="jconf:display_window">
      <xsl:choose>
         <xsl:when test="@version = '2'">
            <xsl:call-template name="check-event-source">
               <xsl:with-param name="display_window" select="."/>
            </xsl:call-template>
            <xsl:copy-of select="." />
         </xsl:when>
         <xsl:when test="@version = '3'">
            <xsl:call-template name="check-event-source">
               <xsl:with-param name="display_window" select="."/>
            </xsl:call-template>
            <xsl:copy-of select="." />
         </xsl:when>
         <xsl:when test="@version = '4'">
            <xsl:message>
               <xsl:text>Found a display window (</xsl:text>
               <xsl:value-of select="@name" />
               <xsl:text>) that is already at version 4.</xsl:text>
               <xsl:value-of select="$newline" />
               <xsl:text>We will attempt to convert it to the "new" version 4.</xsl:text>
               <xsl:value-of select="$newline" />
               <xsl:text>Be sure to verify the changed settings in VRJConfig.</xsl:text>
               <xsl:value-of select="$newline" />
               <xsl:value-of select="$newline" />
            </xsl:message>

            <xsl:element name="display_window">
               <xsl:attribute name="name">
                  <xsl:value-of select="@name" />
               </xsl:attribute>
               <xsl:attribute name="version">
                  <xsl:text>4</xsl:text>
               </xsl:attribute>

               <xsl:copy-of select="./jconf:origin[1]" />
               <xsl:copy-of select="./jconf:origin[2]" />
               <xsl:copy-of select="./jconf:size[1]" />
               <xsl:copy-of select="./jconf:size[2]" />
               <xsl:copy-of select="./jconf:pipe" />
               <xsl:copy-of select="./jconf:frame_buffer_config" />
               <xsl:copy-of select="./jconf:stereo" />
               <xsl:copy-of select="./jconf:border" />
               <xsl:copy-of select="./jconf:hide_mouse" />
               <xsl:copy-of select="./jconf:active" />

               <xsl:if test="count(./jconf:simulator_viewports) &gt; 0">
                  <xsl:copy-of select="./jconf:simulator_viewports" />
               </xsl:if>
               <xsl:if test="count(./jconf:surface_viewports) &gt; 0">
                  <xsl:copy-of select="./jconf:surface_viewports" />
               </xsl:if>

               <xsl:element name="keyboard_mouse_device_name" />
               <xsl:element name="lock_key">
                  <xsl:text>KEY_NONE</xsl:text>
               </xsl:element>
               <xsl:element name="start_locked">
                  <xsl:text>false</xsl:text>
               </xsl:element>
               <xsl:element name="sleep_time">
                  <xsl:text>75</xsl:text>
               </xsl:element>
            </xsl:element>
         </xsl:when>
         <xsl:otherwise>
            <xsl:copy-of select="." />
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <xsl:template match="jconf:event_window">
      <xsl:variable name="km_dev_name">
         <xsl:value-of select="@name"/>
         <xsl:text> KM Dev</xsl:text>
      </xsl:variable>

      <xsl:element name="keyboard_mouse_device">
         <xsl:attribute name="name">
            <xsl:value-of select="$km_dev_name" />
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>

         <xsl:element name="mouse_sensitivity">
            <xsl:value-of select="./jconf:mouse_sensitivity" />
         </xsl:element>
         <xsl:element name="device_host">
            <xsl:value-of select="./jconf:device_host" />
         </xsl:element>
      </xsl:element>

      <xsl:element name="input_window">
         <xsl:attribute name="name">
            <xsl:value-of select="@name" />
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>

         <xsl:element name="origin">
            <xsl:value-of select="./jconf:origin[1]" />
         </xsl:element>
         <xsl:element name="origin">
            <xsl:value-of select="./jconf:origin[2]" />
         </xsl:element>

         <xsl:choose>
            <xsl:when test="@version = '1'">
               <xsl:element name="size">
                  <xsl:value-of select="./jconf:width" />
               </xsl:element>
               <xsl:element name="size">
                  <xsl:value-of select="./jconf:height" />
               </xsl:element>
            </xsl:when>
            <xsl:otherwise>
               <xsl:element name="size">
                  <xsl:value-of select="./jconf:size[1]" />
               </xsl:element>
               <xsl:element name="size">
                  <xsl:value-of select="./jconf:size[2]" />
               </xsl:element>
            </xsl:otherwise>
         </xsl:choose>

         <xsl:element name="display_number">
            <xsl:value-of select="./jconf:display_number" />
         </xsl:element>
         <xsl:element name="keyboard_mouse_device_name">
            <xsl:value-of select="$km_dev_name" />
         </xsl:element>
         <xsl:element name="lock_key">
            <xsl:value-of select="./jconf:lock_key" />
         </xsl:element>
         <xsl:element name="start_locked">
            <xsl:value-of select="./jconf:start_locked" />
         </xsl:element>
         <xsl:element name="sleep_time">
            <xsl:value-of select="./jconf:sleep_time" />
         </xsl:element>
      </xsl:element>
   </xsl:template>

   <xsl:template match="jconf:event_window_proxy">
      <xsl:element name="keyboard_mouse_proxy">
         <xsl:attribute name="name">
            <xsl:value-of select="@name" />
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:variable name="dev_name">
            <xsl:value-of select="./jconf:device" />
         </xsl:variable>
         <xsl:element name="device">
            <xsl:choose>
               <xsl:when test="contains($dev_name, '/event_window_device/')">
                  <xsl:call-template name="strip-path">
                     <xsl:with-param name="original" select="$dev_name" />
                     <xsl:with-param name="path_sep"><xsl:text>/</xsl:text></xsl:with-param>
                  </xsl:call-template>
                  </xsl:when>
                  <xsl:otherwise>
                     <xsl:value-of select="./jconf:device" />
                  </xsl:otherwise>
               </xsl:choose>
            <xsl:text> KM Dev</xsl:text>
         </xsl:element>
      </xsl:element>
   </xsl:template>

   <!-- Rename machine_specific to cluster_node.  -->
   <xsl:template match="jconf:machine_specific">
      <xsl:element name="cluster_node">
         <xsl:attribute name="name">
            <xsl:value-of select="@name" />
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>

         <xsl:copy-of select="./jconf:display_system" />

         <xsl:for-each select="./jconf:display_windows">
            <xsl:copy>
               <xsl:apply-templates />
            </xsl:copy>
         </xsl:for-each>

         <xsl:copy-of select="./jconf:listen_port" />
         <xsl:copy-of select="./jconf:host_name" />
      </xsl:element>
   </xsl:template>

   <!--
      Handle config files that already have machine_specific renamed to
      cluster_node.  They may have embedded display_window elements that
      need to be updated.
   -->
   <xsl:template match="jconf:cluster_node">
      <xsl:copy>
         <xsl:attribute name="name">
            <xsl:value-of select="@name" />
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:value-of select="@version" />
         </xsl:attribute>

         <xsl:copy-of select="./jconf:display_system" />

         <xsl:for-each select="./jconf:display_windows">
            <xsl:copy>
               <xsl:apply-templates />
            </xsl:copy>
         </xsl:for-each>

         <xsl:copy-of select="./jconf:listen_port" />
         <xsl:copy-of select="./jconf:host_name" />
      </xsl:copy>
   </xsl:template>

   <xsl:template match="jconf:sound_manager">
      <xsl:element name="sound_manager_sonix">
         <xsl:attribute name="name">
            <xsl:value-of select="@name" />
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>

         <xsl:copy-of select="./jconf:api" />

         <!-- The following copies all elements of the named type. -->
         <xsl:copy-of select="./jconf:listener_position" />
         <xsl:copy-of select="./jconf:file_search_path" />
         <xsl:copy-of select="./jconf:sound" />
      </xsl:element>
   </xsl:template>


<!-- EVERYTHING ELSE ======================================================= -->
   <!--
      Copy everything that hasn't already been matched.
   -->
   <xsl:template match="*">
      <xsl:copy-of select="." />
   </xsl:template>


<!-- Helpers =============================================================== -->
   <!--
      Identifies when a display_window is configured to act as an event
      source.  If it is conifgured thusly, then a new keyboard_mouse_device
      element is created to stand in for the formerly embedded event_window
      config element.
   -->
   <xsl:template name="check-event-source">
      <xsl:param name="display_window"/>
      <xsl:variable name="is_event_source">
         <xsl:value-of select="$display_window/jconf:act_as_event_source"/>
      </xsl:variable>
      <xsl:choose>
         <xsl:when test="$is_event_source = 'true'">
            <xsl:call-template name="create-km-dev">
               <xsl:with-param name="event_window" select="$display_window/jconf:event_window_device/jconf:event_window"/>
            </xsl:call-template>
         </xsl:when>
         <xsl:when test="$is_event_source = '1'">
            <xsl:call-template name="create-km-dev">
               <xsl:with-param name="event_window" select="$display_window/jconf:event_window_device/jconf:event_window"/>
            </xsl:call-template>
         </xsl:when>
      </xsl:choose>
   </xsl:template>

   <!--
      Creates a new keyboard_mouse_device config element from the given
      event_window element.
   -->
   <xsl:template name="create-km-dev">
      <xsl:param name="event_window"/>

      <xsl:element name="keyboard_mouse_device">
         <xsl:attribute name="name">
            <xsl:value-of select="$event_window/@name"/><xsl:text> KM Dev</xsl:text>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:copy-of select="$event_window/jconf:mouse_sensitivity"/>
         <xsl:copy-of select="$event_window/jconf:device_host"/>
      </xsl:element>
   </xsl:template>

   <!--
      Removes the path information from a full path, leaving only the last
      element (normally a file name).  This is based on code from the XSLT
      book.
   -->
   <xsl:template name="strip-path">
      <xsl:param name="original"/>
      <xsl:param name="path_sep"/>
      <xsl:variable name="last">
         <xsl:choose>
            <xsl:when test="contains($original, $path_sep)">
               <xsl:choose>
                  <xsl:when test="contains(substring-after($original, $path_sep), $path_sep)">
                     <xsl:call-template name="strip-path">
                        <xsl:with-param name="original"><xsl:value-of select="substring-after($original, $path_sep)"/></xsl:with-param>
                        <xsl:with-param name="path_sep"><xsl:value-of select="$path_sep"/></xsl:with-param>
                     </xsl:call-template>
                  </xsl:when>
                  <xsl:otherwise>
                     <xsl:value-of select="substring-after($original, $path_sep)"/>
                  </xsl:otherwise>
               </xsl:choose>
            </xsl:when>
            <xsl:otherwise>
               <xsl:text></xsl:text>
            </xsl:otherwise>
         </xsl:choose>
      </xsl:variable>
      <xsl:value-of select="$last"/>
   </xsl:template>

</xsl:stylesheet>
