<?xml version="1.0" encoding="UTF-8"?>

<!-- *********** <auto-copyright.pl BEGIN do not edit this line> *********** -->
<!--
 VR Juggler is (C) Copyright 1998-2011 by Iowa State University

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
 Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
 -->
<!-- ************ <auto-copyright.pl END do not edit this line> ************ -->

<!--

   This stylesheet provides an upgrade path for VR Juggler 2.0 Alpha 2 or
   Alpha 3 configuration files to changes made for VR Juggler 2.0 Alpha 4.

      xsltproc -o new-file.jconf 2.0a2-2.0a4.xsl old-file.config

   or

      xalan -in old-file.config -xsl 2.0a2-2.0a4.xsl -out new-file.jconf

-->

<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
   <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

   <!-- Define a handy way to insert newlines when necessary. -->
   <xsl:variable name="newline">
<xsl:text>
</xsl:text>
   </xsl:variable>

   <!-- The namespace for JCCL configuration files. -->
   <xsl:variable name="cfg_namespace">
      <xsl:text>http://www.vrjuggler.org/jccl/xsd/3.0/configuration</xsl:text>
   </xsl:variable>

   <!-- The schema for JCCL configuration files. -->
   <xsl:variable name="cfg_schema">
      <xsl:text>http://www.vrjuggler.org/jccl/xsd/3.0/configuration.xsd</xsl:text>
   </xsl:variable>

   <xsl:template match="/">
     <!-- Add the new version information. -->
     <xsl:processing-instruction name="org-vrjuggler-jccl-settings">configuration.version="3.0"</xsl:processing-instruction>
      <xsl:value-of select="$newline"/>

      <xsl:variable name="include_count">
         <xsl:value-of select="count(processing-instruction('org-vrjuggler-jccl-include'))" />
      </xsl:variable>

      <!-- XXX: Need to deal with includes here. -->
      <!-- Create the new XML tree. -->
      <xsl:element name="configuration">
         <!-- Fill in the attributes for the root node. -->
         <!--
            XXX: A good default value for this would be the name of the file.
            How do we get that from the XSLT processor?
         -->
         <xsl:attribute name="name">
            <xsl:text>Configuration</xsl:text>
         </xsl:attribute>
         <xsl:attribute name="xmlns:xsi">
            <xsl:text>http://www.w3.org/2001/XMLSchema-instance</xsl:text>
         </xsl:attribute>
         <xsl:attribute name="xsi:schemaLocation">
            <xsl:value-of select="$cfg_namespace" />
            <xsl:text> </xsl:text>
            <xsl:value-of select="$cfg_schema" />
         </xsl:attribute>
         <xsl:attribute name="xmlns">
            <xsl:value-of select="$cfg_namespace" />
         </xsl:attribute>
         <xsl:value-of select="$newline"/>

         <xsl:call-template name="for-loop">
            <xsl:with-param name="i"         select="1" />
            <xsl:with-param name="increment" select="1" />
            <xsl:with-param name="operator"  select="'&lt;='" />
            <xsl:with-param name="testValue" select="$include_count" />
         </xsl:call-template>

         <!-- Create the tree of elements. -->
         <xsl:element name="elements">
            <xsl:value-of select="$newline"/>
            <xsl:for-each select="ConfigChunkDB/*">
               <xsl:apply-templates select="."/>
            </xsl:for-each>
         </xsl:element>
         <xsl:value-of select="$newline"/>
      </xsl:element>

      <xsl:message>
         <xsl:text>Processing is complete.  You should now use VRJConfig</xsl:text>
         <xsl:value-of select="$newline"/>
         <xsl:text>to verify the results and make any necessary changes.</xsl:text>
         <xsl:value-of select="$newline"/>
      </xsl:message>
   </xsl:template>


<!-- ThreeDMouse =========================================================== -->
   <!-- Rename ThreeDMouse to three_d_mouse. -->
   <xsl:template match="ThreeDMouse">
      <xsl:element name="three_d_mouse">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!-- ThreeDMouse property "deviceHost". -->
   <xsl:template match="ThreeDMouse/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>


<!-- AnaProxy ============================================================== -->
   <!-- Rename AnaProxy to analog_proxy. -->
   <xsl:template match="AnaProxy">
      <xsl:element name="analog_proxy">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- SimAnalog ============================================================= -->
   <!-- Rename SimAnalog to simulated_analog_device. -->
   <xsl:template match="SimAnalog">
      <xsl:element name="simulated_analog_device">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- SimAnalog property "eventWindowProxy". -->
   <xsl:template match="SimAnalog/eventWindowProxy">
      <xsl:element name="event_window_proxy">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- SimAnalog property "keyPairsInc". -->
   <xsl:template match="SimAnalog/keyPairsInc">
      <xsl:element name="increment_keypress">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- SimAnalog property "keyPairsDec". -->
   <xsl:template match="SimAnalog/keyPairsDec">
      <xsl:element name="decrement_keypress">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- SimAnalog property "anastep". -->
   <xsl:template match="SimAnalog/anastep">
      <xsl:element name="delta">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- AppData =============================================================== -->
   <!-- Rename AppData to application_data. -->
   <xsl:template match="AppData">
      <xsl:element name="application_data">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- AwSoundEngine ========================================================= -->
   <!-- Rename AwSoundEngine to audioworks_sound_engine. -->
   <xsl:template match="AwSoundEngine">
      <!-- Drop these.  They are no longer used. -->
   </xsl:template>


<!-- ClusterManager ======================================================== -->
   <!-- Rename ClusterManager to cluster_manager. -->
   <xsl:template match="ClusterManager">
      <xsl:element name="cluster_manager">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>2</xsl:text>
         </xsl:attribute>

         <!--
            If there are one or more plug-ins listed, get the path information
            from the first.  This will be used for the new <plugin_path>
            element.
            XXX: This does not handle the case when there are different paths
            used in the configuration.
         -->
         <xsl:if test="count(./clusterPlugin) &gt; 0">
            <xsl:element name="plugin_path">
               <xsl:call-template name="get-path">
                  <xsl:with-param name="original" select="./clusterPlugin"/>
                  <xsl:with-param name="path_sep"><xsl:text>/</xsl:text></xsl:with-param>
               </xsl:call-template>
            </xsl:element>
            <xsl:value-of select="$newline"/>
         </xsl:if>

         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- Drop ClusterManager property "barrier_master"; it is not used. -->
   <xsl:template match="ClusterManager/barrier_master" />

   <!-- ClusterManager property "cluster_nodes". -->
   <xsl:template match="ClusterManager/cluster_nodes">
      <xsl:element name="cluster_node">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- ClusterManager property "clusterPlugin". -->
   <xsl:template match="ClusterManager/clusterPlugin">
      <xsl:variable name="no_ext">
         <xsl:choose>
            <xsl:when test="contains(., '.dll')">
               <xsl:value-of select="substring-before(., '.dll')"/>
            </xsl:when>
            <xsl:when test="contains(., '.dylib')">
               <xsl:value-of select="substring-before(., '.dylib')"/>
            </xsl:when>
            <xsl:otherwise>
               <xsl:value-of select="substring-before(., '.so')"/>
            </xsl:otherwise>
         </xsl:choose>
      </xsl:variable>

      <xsl:variable name="plugin_name">
         <xsl:call-template name="strip-path">
            <xsl:with-param name="original" select="$no_ext"/>
            <xsl:with-param name="path_sep"><xsl:text>/</xsl:text></xsl:with-param>
         </xsl:call-template>
      </xsl:variable>

      <xsl:element name="plugin">
         <xsl:choose>
            <xsl:when test="starts-with($plugin_name, 'lib')">
               <xsl:value-of select="substring-after($plugin_name, 'lib')"/>
            </xsl:when>
            <xsl:otherwise>
               <xsl:value-of select="$plugin_name"/>
            </xsl:otherwise>
         </xsl:choose>
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>


<!-- CyberGlove ============================================================ -->
   <!-- Rename CyberGlove to cyber_glove. -->
   <xsl:template match="CyberGlove">
      <xsl:element name="cyber_glove">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- CyberGlove property "baud". -->
   <xsl:template match="CyberGlove/baud">
      <xsl:element name="baud_rate">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- CyberGlove property "calDir". -->
   <xsl:template match="CyberGlove/calDir">
      <xsl:element name="calibration_dir">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- CyberGlove property "glovePos". -->
   <xsl:template match="CyberGlove/glovePos">
      <xsl:element name="glove_position">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- CyberGlove property "deviceHost". -->
   <xsl:template match="CyberGlove/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- DTK =================================================================== -->
   <!-- Rename DTK to dtk. -->
   <xsl:template match="DTK">
      <xsl:element name="dtk">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- DTKsegment ============================================================ -->
   <!-- Rename DTKsegment to dtk_segment. -->
   <xsl:template match="DTKsegment">
      <xsl:element name="dtk_segment">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- DTKsegment property "segmentName". -->
   <xsl:template match="DTKsegment/segmentName">
      <xsl:element name="segment_name">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- DTKsegment property "remoteHost". -->
   <xsl:template match="DTKsegment/remoteHost">
      <xsl:element name="host_name">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- DTKsegment property "dataType". -->
   <xsl:template match="DTKsegment/dataType">
      <xsl:element name="data_type">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- DTKsegment property "itemCount". -->
   <xsl:template match="DTKsegment/itemCount">
      <xsl:element name="item_count">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- DTKsegment property "inputType". -->
   <xsl:template match="DTKsegment/inputType">
      <xsl:element name="input_type">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- DataGlove ============================================================= -->
   <!-- Rename DataGlove to data_glove. -->
   <xsl:template match="DataGlove">
      <xsl:element name="data_glove">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- DataGlove property "glovePos". -->
   <xsl:template match="DataGlove/glovePos">
      <xsl:element name="glove_position">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- DataGlove property "deviceHost". -->
   <xsl:template match="DataGlove/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- Default Simulator ===================================================== -->
   <!-- Keep the name the same, but add version information. -->
   <xsl:template match="default_simulator">
      <xsl:element name="default_simulator">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>3</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- default_simulator property "cameraPos". -->
   <xsl:template match="default_simulator/cameraPos">
      <xsl:element name="camera_pos">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- default_simulator property "wandPos". -->
   <xsl:template match="default_simulator/wandPos">
      <xsl:element name="wand_pos">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- default_simulator property "drawProjections". -->
   <xsl:template match="default_simulator/drawProjections">
      <xsl:element name="draw_projections">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- default_simulator property "surfaceColor". -->
   <xsl:template match="default_simulator/surfaceColor">
      <xsl:element name="surface_color">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- default_simulator property "simHeadModel". -->
   <xsl:template match="default_simulator/simHeadModel">
      <xsl:element name="head_model">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- default_simulator property "simWandModel". -->
   <xsl:template match="default_simulator/simWandModel">
      <xsl:element name="wand_model">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- DigProxy ============================================================== -->
   <!-- Rename DigProxy to digital_proxy. -->
   <xsl:template match="DigProxy">
      <xsl:element name="digital_proxy">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- SimDigital ============================================================ -->
   <!-- Rename SimDigital to simulated_digital_device. -->
   <xsl:template match="SimDigital">
      <xsl:element name="simulated_digital_device">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- SimDigital property "eventWindowProxy". -->
   <xsl:template match="SimDigital/eventWindowProxy">
      <xsl:element name="event_window_proxy">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- SimDigital property "keyPairs". -->
   <xsl:template match="SimDigital/keyPairs">
      <xsl:element name="key_pair">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- displaySystem ========================================================= -->
   <!-- Rename displaySystem to display_system. -->
   <xsl:template match="displaySystem">
      <xsl:element name="display_system">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- displaySystem property "numpipes". -->
   <xsl:template match="displaySystem/numpipes">
      <xsl:element name="number_of_pipes">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- displaySystem property "xpipes". -->
   <xsl:template match="displaySystem/xpipes">
      <xsl:element name="x11_pipes">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- displayWindow ========================================================= -->
   <!-- Rename displayWindow to display_window. -->
   <xsl:template match="displayWindow">
      <xsl:element name="display_window">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>2</xsl:text>
         </xsl:attribute>
         <xsl:value-of select="$newline"/>
         <xsl:apply-templates select="./*" />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!-- displayWindow property "frameBufferConfig". -->
   <xsl:template match="displayWindow/frameBufferConfig">
      <xsl:element name="frame_buffer_config">
         <xsl:value-of select="$newline"/>
         <xsl:apply-templates select="./*" />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!--
      displayWindow property "event_window_device" which contains a child
      element that has been renamed.
   -->
   <xsl:template match="displayWindow/event_window_device">
      <xsl:element name="event_window_device">
         <xsl:value-of select="$newline"/>
         <xsl:apply-templates select="./*" />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!-- displayWindow property "sim_viewports". -->
   <xsl:template match="displayWindow/sim_viewports">
      <xsl:element name="simulator_viewports">
         <xsl:value-of select="$newline"/>
         <xsl:apply-templates select="./*" />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!--
      displayWindow property "surface_viewports" which contains a child
      element that has been renamed.
   -->
   <xsl:template match="displayWindow/surface_viewports">
      <xsl:element name="surface_viewports">
         <xsl:value-of select="$newline"/>
         <xsl:apply-templates select="./*" />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>


<!-- EventWindow =========================================================== -->
   <!-- Rename EventWindow to event_window. -->
   <xsl:template match="EventWindow">
      <xsl:element name="event_window">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- EventWindow property "msens". -->
   <xsl:template match="EventWindow/msens">
      <xsl:element name="mouse_sensitivity">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- EventWindow property "deviceHost". -->
   <xsl:template match="EventWindow/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- EventWindowProxy ====================================================== -->
   <!-- Rename EventWindowProxy to event_window_proxy. -->
   <xsl:template match="EventWindowProxy">
      <xsl:element name="event_window_proxy">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- Fastrak =============================================================== -->
   <!-- Rename Fastrak to fastrak. -->
   <xsl:template match="Fastrak">
      <xsl:element name="fastrak">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Rec1". -->
   <xsl:template match="Fastrak/Rec1">
      <xsl:element name="rec1">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Rec2". -->
   <xsl:template match="Fastrak/Rec2">
      <xsl:element name="rec2">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Rec3". -->
   <xsl:template match="Fastrak/Rec3">
      <xsl:element name="rec3">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Rec4". -->
   <xsl:template match="Fastrak/Rec4">
      <xsl:element name="rec4">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Tip1". -->
   <xsl:template match="Fastrak/Tip1">
      <xsl:element name="tip1">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Tip2". -->
   <xsl:template match="Fastrak/Tip2">
      <xsl:element name="tip2">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Tip3". -->
   <xsl:template match="Fastrak/Tip3">
      <xsl:element name="tip3">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Tip4". -->
   <xsl:template match="Fastrak/Tip4">
      <xsl:element name="tip4">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Inc1". -->
   <xsl:template match="Fastrak/Inc1">
      <xsl:element name="inc1">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Inc2". -->
   <xsl:template match="Fastrak/Inc2">
      <xsl:element name="inc2">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Inc3". -->
   <xsl:template match="Fastrak/Inc3">
      <xsl:element name="inc3">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Inc4". -->
   <xsl:template match="Fastrak/Inc4">
      <xsl:element name="inc4">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Hem1". -->
   <xsl:template match="Fastrak/Hem1">
      <xsl:element name="hem1">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Hem2". -->
   <xsl:template match="Fastrak/Hem2">
      <xsl:element name="hem2">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Hem3". -->
   <xsl:template match="Fastrak/Hem3">
      <xsl:element name="hem3">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "Hem4". -->
   <xsl:template match="Fastrak/Hem4">
      <xsl:element name="hem4">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "ARF1". -->
   <xsl:template match="Fastrak/ARF1">
      <xsl:element name="arf1">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "ARF2". -->
   <xsl:template match="Fastrak/ARF2">
      <xsl:element name="arf2">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "ARF3". -->
   <xsl:template match="Fastrak/ARF3">
      <xsl:element name="arf3">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "ARF4". -->
   <xsl:template match="Fastrak/ARF4">
      <xsl:element name="arf4">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "TMF1". -->
   <xsl:template match="Fastrak/TMF1">
      <xsl:element name="tmf1">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "TMF2". -->
   <xsl:template match="Fastrak/TMF2">
      <xsl:element name="tmf2">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "TMF3". -->
   <xsl:template match="Fastrak/TMF3">
      <xsl:element name="tmf3">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "TMF4". -->
   <xsl:template match="Fastrak/TMF4">
      <xsl:element name="tmf4">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Fastrak property "deviceHost". -->
   <xsl:template match="Fastrak/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- Flock ================================================================= -->
   <!-- Rename Flock to flock. -->
   <xsl:template match="Flock">
      <xsl:element name="flock">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- Flock property "num". -->
   <xsl:template match="Flock/num">
      <xsl:element name="number_of_birds">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Flock property "calfile". -->
   <xsl:template match="Flock/calfile">
      <xsl:element name="calibration_file">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Flock property "hemi". -->
   <xsl:template match="Flock/hemi">
      <xsl:element name="hemisphere">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Flock property "filt". -->
   <xsl:template match="Flock/filt">
      <xsl:element name="filter">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Flock property "sync". -->
   <xsl:template match="Flock/sync">
      <xsl:element name="sync_style">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Flock property "transmitter". -->
   <xsl:template match="Flock/transmitter">
      <xsl:element name="transmitter_id">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Flock property "extendedRange". -->
   <xsl:template match="Flock/extendedRange">
      <xsl:element name="extended_range">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Flock property "deviceHost". -->
   <xsl:template match="Flock/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Flock property "position_filters". -->
   <xsl:template match="Flock/position_filters">
      <xsl:element name="position_filters">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- gadget_logger ========================================================= -->
   <!-- Keep the name the same, but add version information. -->
   <xsl:template match="gadget_logger">
      <xsl:element name="gadget_logger">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- GestureProxy ========================================================== -->
   <!-- Rename GestureProxy to gesture_proxy. -->
   <xsl:template match="GestureProxy">
      <xsl:element name="gesture_proxy">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- SimGloveGesture ======================================================= -->
   <!-- Rename SimGloveGesture to simulated_glove_gesture. -->
   <xsl:template match="SimGloveGesture">
      <xsl:element name="simulated_glove_gesture">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>2</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- SimGloveGesture property "eventWindowProxy". -->
   <xsl:template match="SimGloveGesture/eventWindowProxy">
      <xsl:element name="event_window_proxy">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- SimGloveGesture property "keyPairs". -->
   <xsl:template match="SimGloveGesture/keyPairs">
      <xsl:element name="key_presses">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- SimGloveGesture property "trainedFilename". -->
   <xsl:template match="SimGloveGesture/trainedFilename">
      <xsl:element name="trained_filename">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- SimGloveGesture property "glovePos". -->
   <xsl:template match="SimGloveGesture/glovePos">
      <xsl:element name="glove_position">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- GloveProxy =========================================================== -->
   <!-- Rename GloveProxy to glove_proxy. -->
   <xsl:template match="GloveProxy">
      <xsl:element name="glove_proxy">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- IBox ================================================================== -->
   <!-- Rename IBox to ibox. -->
   <xsl:template match="IBox">
      <xsl:element name="ibox">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- IBox property "deviceHost". -->
   <xsl:template match="IBox/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- InputManager ========================================================== -->
   <!-- Rename InputManager to input_manager. -->
   <xsl:template match="InputManager">
      <xsl:element name="input_manager">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>2</xsl:text>
         </xsl:attribute>

         <!--
            If there are one or more drivers listed, get the path information
            from the first.  This will be used for the new <driver_path>
            element.
            XXX: This does not handle the case when there are different paths
            used in the configuration.
         -->
         <xsl:if test="count(./driver) &gt; 0">
            <xsl:element name="driver_path">
               <xsl:call-template name="get-path">
                  <xsl:with-param name="original" select="./driver"/>
                  <xsl:with-param name="path_sep"><xsl:text>/</xsl:text></xsl:with-param>
               </xsl:call-template>
            </xsl:element>
            <xsl:value-of select="$newline"/>
         </xsl:if>

         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <xsl:template match="InputManager/driver">
      <xsl:variable name="no_ext">
         <xsl:choose>
            <xsl:when test="contains(., '.dll')">
               <xsl:value-of select="substring-before(., '.dll')"/>
            </xsl:when>
            <xsl:when test="contains(., '.dylib')">
               <xsl:value-of select="substring-before(., '.dylib')"/>
            </xsl:when>
            <xsl:otherwise>
               <xsl:value-of select="substring-before(., '.so')"/>
            </xsl:otherwise>
         </xsl:choose>
      </xsl:variable>

      <xsl:element name="driver">
         <xsl:call-template name="strip-path">
            <xsl:with-param name="original" select="$no_ext"/>
            <xsl:with-param name="path_sep"><xsl:text>/</xsl:text></xsl:with-param>
         </xsl:call-template>
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!-- InputManger property "driverDirectory". -->
   <xsl:template match="InputManger/driverDirectory">
      <xsl:element name="driver_scan_path">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- Intersense ============================================================ -->
   <!-- Rename Intersense to intersense. -->
   <xsl:template match="Intersense">
      <xsl:element name="intersense">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- Intersense property "position_filters". -->
   <xsl:template match="Intersense/position_filters">
      <xsl:element name="position_filters">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- Intersense property "stations". -->
   <xsl:template match="Intersense/stations">
      <xsl:element name="stations">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- Intersense property "deviceHost". -->
   <xsl:template match="Intersense/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- IntersenseAPI ========================================================= -->
   <!-- Rename IntersenseAPI to intersense_api. -->
   <xsl:template match="IntersenseAPI">
      <xsl:element name="intersense_api">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- IntersenseAPI property "position_filters". -->
   <xsl:template match="IntersenseAPI/position_filters">
      <xsl:element name="position_filters">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- IntersenseAPI property "stations". -->
   <xsl:template match="IntersenseAPI/stations">
      <xsl:element name="stations">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- IntersenseAPI property "deviceHost". -->
   <xsl:template match="IntersenseAPI/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- KeyModPair ============================================================ -->
   <!-- Rename KeyModPair to key_modifier_pair. -->
   <xsl:template match="KeyModPair">
      <xsl:element name="key_modifier_pair">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- KeyModPair property "modKey". -->
   <xsl:template match="KeyModPair/modKey">
      <xsl:element name="modifier_key">
         <xsl:call-template name="convert-modifier-key">
            <xsl:with-param name="modifier_key">
               <xsl:value-of select="." />
            </xsl:with-param>
         </xsl:call-template>
      </xsl:element>
   </xsl:template>


<!-- MachineSpecific ======================================================= -->
   <!-- Rename MachineSpecific to machine_specific. -->
   <xsl:template match="MachineSpecific">
      <xsl:element name="machine_specific">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- MachineSpecific property "serialPort". -->
   <xsl:template match="MachineSpecific/serialPort">
      <xsl:element name="port">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- MachineSpecific property "serialBaud". -->
   <xsl:template match="MachineSpecific/serialBaud">
      <xsl:element name="baud">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- MachineSpecific property "display_windows". -->
   <xsl:template match="MachineSpecific/display_windows">
      <xsl:element name="display_windows">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- MachineSpecific property "display_system". -->
   <xsl:template match="MachineSpecific/display_system">
      <xsl:element name="display_system">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- MotionStar ============================================================ -->
   <!-- Rename MotionStar to motion_star. -->
   <xsl:template match="MotionStar">
      <xsl:element name="motion_star">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- MotionStar property "serverPort". -->
   <xsl:template match="MotionStar/serverPort">
      <xsl:element name="server_port">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- MotionStar property "serverType". -->
   <xsl:template match="MotionStar/serverType">
      <xsl:element name="server_type">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- MotionStar property "position_filters". -->
   <xsl:template match="MotionStar/position_filters">
      <xsl:element name="position_filters">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- MotionStar property "num". -->
   <xsl:template match="MotionStar/num">
      <xsl:element name="number_of_birds">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- MotionStar property "bformat". -->
   <xsl:template match="MotionStar/bformat">
      <xsl:element name="data_format">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- MotionStar property "reportRate". -->
   <xsl:template match="MotionStar/reportRate">
      <xsl:element name="report_rate">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- MotionStar property "measurementRate". -->
   <xsl:template match="MotionStar/measurementRate">
      <xsl:element name="measurement_rate">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- MotionStar property "deviceHost". -->
   <xsl:template match="MotionStar/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- OpenGLFBConfig ======================================================== -->
   <!-- Rename OpenGLFBConfig to opengl_frame_buffer_config. -->
   <xsl:template match="OpenGLFBConfig">
      <xsl:element name="opengl_frame_buffer_config">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:value-of select="$newline"/>
         <xsl:if test="count(./visualID) = 0">
            <xsl:element name="visual_id">
               <xsl:text>-1</xsl:text>
            </xsl:element>
         </xsl:if>
         <xsl:if test="count(./fsaaEnable) = 0">
            <xsl:element name="fsaa_enable">
               <xsl:text>false</xsl:text>
            </xsl:element>
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!-- OpenGLFBConfig property "visualID". -->
   <xsl:template match="OpenGLFBConfig/visualID">
      <xsl:element name="visual_id">
         <xsl:value-of select="." />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!-- OpenGLFBConfig property "redSize". -->
   <xsl:template match="OpenGLFBConfig/redSize">
      <xsl:element name="red_size">
         <xsl:value-of select="." />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!-- OpenGLFBConfig property "greenSize". -->
   <xsl:template match="OpenGLFBConfig/greenSize">
      <xsl:element name="green_size">
         <xsl:value-of select="." />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!-- OpenGLFBConfig property "blueSize". -->
   <xsl:template match="OpenGLFBConfig/blueSize">
      <xsl:element name="blue_size">
         <xsl:value-of select="." />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!-- OpenGLFBConfig property "alphaSize". -->
   <xsl:template match="OpenGLFBConfig/alphaSize">
      <xsl:element name="alpha_size">
         <xsl:value-of select="." />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!-- OpenGLFBConfig property "depthBufferSize". -->
   <xsl:template match="OpenGLFBConfig/depthBufferSize">
      <xsl:element name="depth_buffer_size">
         <xsl:value-of select="." />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!-- OpenGLFBConfig property "fsaaEnable". -->
   <xsl:template match="OpenGLFBConfig/fsaaEnable">
      <xsl:element name="fsaa_enable">
         <xsl:value-of select="." />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>


<!-- PinchGlove ============================================================ -->
   <!-- Rename PinchGlove to pinch_glove. -->
   <xsl:template match="PinchGlove">
      <xsl:element name="pinch_glove">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- PinchGlove property "glovePos". -->
   <xsl:template match="PinchGlove/glovePos">
      <xsl:element name="glove_position">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- PinchGlove property "deviceHost". -->
   <xsl:template match="PinchGlove/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- PosXformFilter ======================================================== -->
   <!-- Rename PosXformFilter to position_transform_filter. -->
   <xsl:template match="PosXformFilter">
      <xsl:element name="position_transform_filter">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- PosXformFilter property "dev_units". -->
   <xsl:template match="PosXformFilter/dev_units">
      <xsl:element name="device_units">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- PosProxy ============================================================== -->
   <!-- Rename PosProxy to position_proxy. -->
   <xsl:template match="PosProxy">
      <xsl:element name="position_proxy">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- PosProxy property "position_filters". -->
   <xsl:template match="PosProxy/position_filters">
      <xsl:element name="position_filters">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- SimPosition =========================================================== -->
   <!-- Rename SimPosition to simulated_positional_device. -->
   <xsl:template match="SimPosition">
      <xsl:element name="simulated_positional_device">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>2</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- SimPosition property "eventWindowProxy". -->
   <xsl:template match="SimPosition/eventWindowProxy">
      <xsl:element name="event_window_proxy">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- SimPosition property "keyPairs". -->
   <xsl:template match="SimPosition/keyPairs">
      <xsl:element name="key_pair">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- SimPosition property "initialPos". -->
   <xsl:template match="SimPosition/initialPos">
      <xsl:element name="initial_position">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- SimPosition property "initialRot". -->
   <xsl:template match="SimPosition/initialRot">
      <xsl:element name="initial_rotation">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- SimPosition property "dtrans". -->
   <xsl:template match="SimPosition/dtrans">
      <xsl:element name="translation_delta">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- SimPosition property "drot". -->
   <xsl:template match="SimPosition/drot">
      <xsl:element name="rotation_delta">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- SimPosition property "transCoordSystem". -->
   <xsl:template match="SimPosition/transCoordSystem">
      <xsl:element name="translation_coordinate_system">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- SimPosition property "rotCoordSystem". -->
   <xsl:template match="SimPosition/rotCoordSystem">
      <xsl:element name="rotation_coordinate_system">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- SimPosition property "position_filters". -->
   <xsl:template match="SimPosition/position_filters">
      <xsl:element name="position_filters">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- proxyAlias ============================================================ -->
   <!-- Rename proxyAlias to alias. -->
   <xsl:template match="proxyAlias">
      <xsl:element name="alias">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- proxyAlias property "aliasName" which has been removed. -->
   <xsl:template match="proxyAlias/aliasName">
   </xsl:template>

   <!-- proxyAlias property "proxyPtr". -->
   <xsl:template match="proxyAlias/proxyPtr">
      <xsl:element name="proxy">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- SimDigitalGlove ======================================================= -->
   <!-- Rename SimDigitalGlove to simulated_digital_glove. -->
   <xsl:template match="SimDigitalGlove">
      <xsl:element name="simulated_digital_glove">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>2</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- SimDigitalGlove property "eventWindowProxy". -->
   <xsl:template match="SimDigitalGlove/eventWindowProxy">
      <xsl:element name="event_window_proxy">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- SimDigitalGlove property "keyPairs". -->
   <xsl:template match="SimDigitalGlove/keyPairs">
      <xsl:element name="key_pair">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- SimDigitalGlove property "glovePos". -->
   <xsl:template match="SimDigitalGlove/glovePos">
      <xsl:element name="left_glove_position">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- SimDigitalGlove property "rightGlovePos". -->
   <xsl:template match="SimDigitalGlove/rightGlovePos">
      <xsl:element name="right_glove_position">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- SimRelativePosition =================================================== -->
   <!-- Rename SimRelativePosition to simulated_relative_position. -->
   <xsl:template match="SimRelativePosition">
      <xsl:element name="simulated_relative_position">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- SimSetablePosition ==================================================== -->
   <!-- Rename SimSetablePosition to simulated_setable_position. -->
   <xsl:template match="SimSetablePosition">
      <xsl:element name="simulated_setable_position">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- simViewport =========================================================== -->
   <!-- Rename simViewport to simulator_viewport. -->
   <xsl:template match="simViewport">
      <xsl:element name="simulator_viewport">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>3</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- simViewport property "vert_fov". -->
   <xsl:template match="simViewport/vert_fov">
      <xsl:element name="vertical_fov">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!--
      simViewport property "view" which had its enumeration symbols change.
   -->
   <xsl:template match="simViewport/view">
      <xsl:element name="view">
         <xsl:call-template name="rename-view">
            <xsl:with-param name="view">
               <xsl:value-of select="."/>
            </xsl:with-param>
         </xsl:call-template>
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!-- simViewport property "simPlugIn". -->
   <xsl:template match="simViewport/simPlugIn">
      <xsl:element name="simulator_plugin">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- Sound ================================================================= -->
   <!-- Rename Sound to sound. -->
   <xsl:template match="Sound">
      <xsl:element name="sound">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- Sound property "pitchbend". -->
   <xsl:template match="Sound/pitchbend">
      <xsl:element name="pitch_bend">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- StartBarrierPlugin ==================================================== -->
   <!-- Rename StartBarrierPlugin to start_barrier_plugin. -->
   <xsl:template match="StartBarrierPlugin">
      <xsl:element name="start_barrier_plugin">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- ISenseStation ========================================================= -->
   <!-- Rename ISenseStation to intersense_station. -->
   <xsl:template match="ISenseStation">
      <xsl:element name="intersense_station">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- ISenseStation property "stationIndex". -->
   <xsl:template match="ISenseStation/stationIndex">
      <xsl:element name="station_index">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- ISenseStation property "useDigital". -->
   <xsl:template match="ISenseStation/useDigital">
      <xsl:element name="use_digital">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- ISenseStation property "useAnalog". -->
   <xsl:template match="ISenseStation/useAnalog">
      <xsl:element name="use_analog">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- ISenseStation property "digitalFirst". -->
   <xsl:template match="ISenseStation/digitalFirst">
      <xsl:element name="digital_first">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- ISenseStation property "digitalNum". -->
   <xsl:template match="ISenseStation/digitalNum">
      <xsl:element name="digital_count">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- ISenseStation property "analogFirst". -->
   <xsl:template match="ISenseStation/analogFirst">
      <xsl:element name="analog_first">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- ISenseStation property "analogNum". -->
   <xsl:template match="ISenseStation/analogNum">
      <xsl:element name="analog_count">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- surfaceViewport ======================================================= -->
   <!-- Rename surfaceViewport to surface_viewport. -->
   <xsl:template match="surfaceViewport">
      <xsl:element name="surface_viewport">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!--
      surfaceViewport property "corners" which contains a child element
      whose name has changed.
   -->
   <xsl:template match="surfaceViewport/corners">
      <xsl:element name="corners">
         <xsl:value-of select="$newline"/>
         <xsl:apply-templates select="./*" />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!-- surfaceViewport property "trackerproxy". -->
   <xsl:template match="surfaceViewport/trackerproxy">
      <xsl:element name="tracker_proxy">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!--
      surfaceViewport property "view" which had its enumeration symbols change.
   -->
   <xsl:template match="surfaceViewport/view">
      <xsl:element name="view">
         <xsl:call-template name="rename-view">
            <xsl:with-param name="view">
               <xsl:value-of select="."/>
            </xsl:with-param>
         </xsl:call-template>
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>


<!-- SwapLockTCPPlugin ===================================================== -->
   <!-- Rename SwapLockTCPPlugin to swap_lock_tcp_plugin. -->
   <xsl:template match="SwapLockTCPPlugin">
      <xsl:element name="swap_lock_tcp_plugin">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- SwapLockWiredPlugin =================================================== -->
   <!-- Rename SwapLockWiredPlugin to swap_lock_wired_plugin. -->
   <xsl:template match="SwapLockWiredPlugin">
      <xsl:element name="swap_lock_wired_plugin">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- TrackdAPIController =================================================== -->
   <!-- Rename TrackdAPIController to trackd_api_controller. -->
   <xsl:template match="TrackdAPIController">
      <xsl:element name="trackd_api_controller">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- TrackdAPIController property "shm_key". -->
   <xsl:template match="TrackdAPIController/shm_key">
      <xsl:element name="shared_memory_key">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- TrackdAPIController property "deviceHost". -->
   <xsl:template match="TrackdAPIController/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- TrackdAPISensor ======================================================= -->
   <!-- Rename TrackdAPISensor to trackd_api_sensor. -->
   <xsl:template match="TrackdAPISensor">
      <xsl:element name="trackd_api_sensor">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- TrackdAPISensor property "shm_key". -->
   <xsl:template match="TrackdAPISensor/shm_key">
      <xsl:element name="shared_memory_key">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- TrackdController ====================================================== -->
   <!-- Rename TrackdController to trackd_controller. -->
   <xsl:template match="TrackdController">
      <xsl:element name="trackd_controller">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- TrackdController property "shm_key". -->
   <xsl:template match="TrackdController/shm_key">
      <xsl:element name="shared_memory_key">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- TrackdController property "deviceHost". -->
   <xsl:template match="TrackdController/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- TrackdSensor ========================================================== -->
   <!-- Rename TrackdSensor to trackd_sensor. -->
   <xsl:template match="TrackdSensor">
      <xsl:element name="trackd_sensor">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- TrackdSensor property "shm_key". -->
   <xsl:template match="TrackdSensor/shm_key">
      <xsl:element name="shared_memory_key">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- TrackdSensor property "position_filters". -->
   <xsl:template match="TrackdSensor/position_filters">
      <xsl:element name="position_filters">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- TrackdSensor property "deviceHost". -->
   <xsl:template match="TrackdSensor/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- TweekAnalogDevice ===================================================== -->
   <!-- Rename TweekAnalogDevice to tweek_analog_device. -->
   <xsl:template match="TweekAnalogDevice">
      <xsl:element name="tweek_analog_device">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- TweekGadget =========================================================== -->
   <!-- Rename TweekGadget to tweek_gadget. -->
   <xsl:template match="TweekGadget">
      <xsl:element name="tweek_gadget">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- TweekGadget property "nsHost". -->
   <xsl:template match="TweekGadget/nsHost">
      <xsl:element name="naming_service_host">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- TweekGadget property "nsPort". -->
   <xsl:template match="TweekGadget/nsPort">
      <xsl:element name="naming_service_port">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- TweekGadget property "iiopVer". -->
   <xsl:template match="TweekGadget/iiopVer">
      <xsl:element name="iiop_version">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- TweekGadget property "positionDevice". -->
   <xsl:template match="TweekGadget/positionDevice">
      <xsl:element name="position_device">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- TweekGadget property "analogDevice". -->
   <xsl:template match="TweekGadget/analogDevice">
      <xsl:element name="analog_device">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- TweekGadget property "digitalDevice". -->
   <xsl:template match="TweekGadget/digitalDevice">
      <xsl:element name="digital_device">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- TweekGadget property "deviceHost". -->
   <xsl:template match="TweekGadget/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- TweekDigitalDevice ==================================================== -->
   <!-- Rename TweekDigitalDevice to tweek_digital_device. -->
   <xsl:template match="TweekDigitalDevice">
      <xsl:element name="tweek_digital_device">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- TweekPositionDevice =================================================== -->
   <!-- Rename TweekPositionDevice to tweek_positional_device. -->
   <xsl:template match="TweekPositionDevice">
      <xsl:element name="tweek_positional_device">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- TweekGadgetGUI ======================================================== -->
   <!-- Rename TweekGadgetGUI to tweek_gadget_gui. -->
   <xsl:template match="TweekGadgetGUI">
      <xsl:element name="tweek_gadget_gui">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- TweekGadgetGUI property "posDevice". -->
   <xsl:template match="TweekGadgetGUI/posDevice">
      <xsl:element name="positional_device">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- TweekGadgetGUI property "digitalDevice". -->
   <xsl:template match="TweekGadgetGUI/digitalDevice">
      <xsl:element name="digital_device">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- TweekGadgetGUI property "analogDevice". -->
   <xsl:template match="TweekGadgetGUI/analogDevice">
      <xsl:element name="analog_device">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- juggler_audio_manager ================================================= -->
   <!-- Rename juggler_audio_manager to sound_manager. -->
   <xsl:template match="juggler_audio_manager">
      <xsl:element name="sound_manager">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- juggler_audio_manager property "Sounds". -->
   <xsl:template match="juggler_audio_manager/Sounds">
      <xsl:element name="sound">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- JugglerUser =========================================================== -->
   <!-- Rename JugglerUser to user. -->
   <xsl:template match="JugglerUser">
      <xsl:element name="user">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- JugglerUser property "headPos". -->
   <xsl:template match="JugglerUser/headPos">
      <xsl:element name="head_position">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>


<!-- Vec3 ================================================================== -->
   <!-- Rename Vec3 to vec3. -->
   <xsl:template match="Vec3">
      <xsl:element name="vec3">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- Vrpn ================================================================== -->
   <!-- Rename Vrpn to vrpn. -->
   <xsl:template match="Vrpn">
      <xsl:element name="vrpn">
         <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:if test="count(./deviceHost) = 0">
            <xsl:element name="device_host" />
         </xsl:if>
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- Vrpn property "trackerServer". -->
   <xsl:template match="Vrpn/trackerServer">
      <xsl:element name="tracker_server">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Vrpn property "buttonServer". -->
   <xsl:template match="Vrpn/buttonServer">
      <xsl:element name="button_server">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Vrpn property "numTrackers". -->
   <xsl:template match="Vrpn/numTrackers">
      <xsl:element name="tracker_count">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Vrpn property "numButtons". -->
   <xsl:template match="Vrpn/numButtons">
      <xsl:element name="button_count">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <!-- Vrpn property "position_filters". -->
   <xsl:template match="Vrpn/position_filters">
      <xsl:element name="position_filters">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- Vrpn property "deviceHost". -->
   <xsl:template match="Vrpn/deviceHost">
      <xsl:element name="device_host">
         <xsl:value-of select="." />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>


<!-- EVERYTHING ELSE ======================================================= -->
   <!--
      Copy everything that hasn't already been matched.
   -->
   <xsl:template match="*">
      <xsl:choose>
         <xsl:when test="name(..) = 'ConfigChunkDB'">
            <xsl:message terminate="no">
               <xsl:text>Updating unknown element type </xsl:text>
               <xsl:value-of select="name(.)" />
               <xsl:text> (this is fine for custom types)</xsl:text>
               <xsl:value-of select="$newline"/>
            </xsl:message>
            <xsl:element name="{name(.)}">
               <xsl:attribute name="name">
                  <xsl:value-of select="@name"/>
               </xsl:attribute>
               <xsl:attribute name="version">
                  <xsl:text>1</xsl:text>
               </xsl:attribute>
               <xsl:value-of select="$newline"/>
               <xsl:copy-of select="./*" />
               <xsl:value-of select="$newline"/>
            </xsl:element>
            <xsl:value-of select="$newline"/>
         </xsl:when>
         <xsl:otherwise>
            <xsl:copy-of select="." />
            <xsl:value-of select="$newline"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>


<!-- Helpers =============================================================== -->
   <xsl:template name="rename-view">
      <xsl:param name="view" />

      <xsl:choose>
         <xsl:when test="$view = 1 or starts-with($view, 'left_eye')">
            <xsl:text>Left Eye</xsl:text>
         </xsl:when>
         <xsl:when test="$view = 2 or starts-with($view, 'right_eye')">
            <xsl:text>Right Eye</xsl:text>
         </xsl:when>
         <xsl:when test="$view = 3 or starts-with($view, 'stereo')">
            <xsl:text>Stereo</xsl:text>
         </xsl:when>
      </xsl:choose>
   </xsl:template>

   <xsl:template name="convert-modifier-key">
      <xsl:param name="modifier_key" />

      <xsl:choose>
         <xsl:when test="$modifier_key = 0">
            <xsl:text>NONE</xsl:text>
         </xsl:when>
         <xsl:when test="$modifier_key = 5">
            <xsl:text>SHIFT</xsl:text>
         </xsl:when>
         <xsl:when test="$modifier_key = 6">
            <xsl:text>CTRL</xsl:text>
         </xsl:when>
         <xsl:when test="$modifier_key = 7">
            <xsl:text>ALT</xsl:text>
         </xsl:when>
         <xsl:when test="$modifier_key = -1">
            <xsl:text>ANY</xsl:text>
         </xsl:when>
         <xsl:otherwise>
            <xsl:value-of select="." />
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!-- Generator of <include> elements. -->
   <xsl:template name="includeGenerate">
      <xsl:param name="file"/>
      <xsl:param name="label"/>
      <xsl:param name="defaultValue" select="''"/>

      <xsl:element name="include">
         <xsl:value-of select="$file"/>
      </xsl:element>

      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!--
      Returns the directory containing a file, given a full path to the file.
   -->
   <xsl:template name="get-path">
      <xsl:param name="original"/>
      <xsl:param name="path_sep"/>
      <xsl:variable name="driver_name">
         <xsl:call-template name="strip-path">
            <xsl:with-param name="original" select="$original"/>
            <xsl:with-param name="path_sep" select="$path_sep"/>
         </xsl:call-template>
      </xsl:variable>
      <xsl:value-of select="substring-before($original, $driver_name)"/>
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

   <!--
      This template emulates the behavior of a traditional for loop.  It
      comes from Chapter 4 of /Mastering XML Transformations/ (pp. 85-89)
      by Doug Tidwell.
    -->
   <xsl:template name="for-loop">
      <xsl:param name="i"         select="1"/>
      <xsl:param name="increment" select="1"/>
      <xsl:param name="operator"  select="'='"/>
      <xsl:param name="testValue" select="1"/>
      <xsl:param name="iteration" select="1"/>

      <xsl:variable name="testPassed">
         <xsl:choose>
            <xsl:when test="starts-with($operator, '!=')">
               <xsl:if test="$i != $testValue">
                  <xsl:text>true</xsl:text>
               </xsl:if>
            </xsl:when>
            <xsl:when test="starts-with($operator, '&lt;=')">
               <xsl:if test="$i &lt;= $testValue">
                  <xsl:text>true</xsl:text>
               </xsl:if>
            </xsl:when>
            <xsl:when test="starts-with($operator, '&gt;=')">
               <xsl:if test="$i &gt;= $testValue">
                  <xsl:text>true</xsl:text>
               </xsl:if>
            </xsl:when>
            <xsl:when test="starts-with($operator, '=')">
               <xsl:if test="$i = $testValue">
                  <xsl:text>true</xsl:text>
               </xsl:if>
            </xsl:when>
            <xsl:when test="starts-with($operator, '&lt;')">
               <xsl:if test="$i &lt; $testValue">
                  <xsl:text>true</xsl:text>
               </xsl:if>
            </xsl:when>
            <xsl:when test="starts-with($operator, '&gt;')">
               <xsl:if test="$i &gt; $testValue">
                  <xsl:text>true</xsl:text>
               </xsl:if>
            </xsl:when>
            <xsl:otherwise>
               <xsl:message terminate="yes">
                  <xsl:text>Sorry, the for-loop emulator only </xsl:text>
                  <xsl:text>handles six operators </xsl:text>
                  <xsl:value-of select="$newline"/>
                  <xsl:text>(&lt; | &gt; | = | &lt;= | &gt;= | !=).  </xsl:text>
                  <xsl:text>The value </xsl:text>
                  <xsl:value-of select="$operator"/>
                  <xsl:text> is not allowed.</xsl:text>
                  <xsl:value-of select="$newline"/>
               </xsl:message>
            </xsl:otherwise>
         </xsl:choose>
      </xsl:variable>

      <xsl:if test="$testPassed='true'">
         <xsl:variable name="file_param">
            <xsl:value-of select="processing-instruction('org-vrjuggler-jccl-include')[$i]" />
         </xsl:variable>

         <!--
            The magic number 7 here is to truncate trailing characters
            including the closing quote on the file name.
         -->
         <xsl:variable name="file_end">
            <xsl:value-of select="string-length($file_param) - 7" />
         </xsl:variable>

         <xsl:call-template name="includeGenerate">
            <!--
               The magic number 7 here is to skip characters up to and
               including the opening quote on the file name.
               XXX: Is there a better way to extract the file name string
               from $file_param?
            -->
            <xsl:with-param name="file"
                            select="substring($file_param, 7, $file_end)"/>
         </xsl:call-template>

         <!-- Your logic should end here; don't change the rest of this     -->
         <!-- template!                                                     -->

         <!-- Now for the important part: we increment the index variable   -->
         <!-- and loop.  Notice that we're passing the incremented value,   -->
         <!-- not changing the variable itself.                             -->

         <xsl:call-template name="for-loop">
            <xsl:with-param name="i"         select="$i + $increment"/>
            <xsl:with-param name="increment" select="$increment"/>
            <xsl:with-param name="operator"  select="$operator"/>
            <xsl:with-param name="testValue" select="$testValue"/>
            <xsl:with-param name="iteration" select="$iteration + 1"/>
         </xsl:call-template>
      </xsl:if> 
   </xsl:template>

</xsl:stylesheet>
