<?xml version="1.0" encoding="UTF-8"?>

<!-- *********** <auto-copyright.pl BEGIN do not edit this line> *********** -->
<!--
 VR Juggler is (C) Copyright 1998-2003 by Iowa State University

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

 =================================================================
 File:          $RCSfile$
 Date modified: $Date$
 Version:       $Revision$
 =================================================================
 -->
<!-- ************ <auto-copyright.pl END do not edit this line> ************ -->

<!--
   This stylesheet provides an upgrade path for VR Juggler 2.0 Alpha 2
   configuration files to changes made for VR Juggler 2.0 Beta 1. This stylesheet
   assumes that you have already upgraded your old configuration file to the new
   file format using the xmltools in the JCCL package.

      xsltproc -o new-file.config 2.0a2-2.0b1.xsl old-file.config

   or

      xalan -in old-file.config -xsl 2.0a2-2.0b1.xsl -out new-file.config

-->

<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:cfg="http://www.vrjuggler.org/jccl/xsd/3.0/configuration">
   <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

   <!-- Define a handy way to insert newlines when necessary. -->
   <xsl:variable name="newline">
<xsl:text>
</xsl:text>
   </xsl:variable>

   <xsl:template match="/">
      <!-- Create the new XML tree. -->
      <xsl:element name="configuration">
         <!-- Copy all attributes for the root node. -->
         <xsl:for-each select="cfg:configuration/@*">
            <xsl:copy />
         </xsl:for-each>
         <xsl:value-of select="$newline"/>
         <xsl:for-each select="cfg:configuration/*">
            <xsl:apply-templates select="."/>
         </xsl:for-each>
      </xsl:element>

      <xsl:message>
         <xsl:text>Processing is complete.  You should now use VRJConfig</xsl:text>
         <xsl:value-of select="$newline"/>
         <xsl:text>to verify the results and make any necessary changes.</xsl:text>
         <xsl:value-of select="$newline"/>
      </xsl:message>
   </xsl:template>

   <xsl:template match="cfg:configuration/cfg:elements">
      <xsl:element name="elements">
         <xsl:for-each select="./*">
            <xsl:apply-templates select="."/>
         </xsl:for-each>
      </xsl:element>
   </xsl:template>


<!-- ThreeDMouse =========================================================== -->
   <!-- Rename ThreeDMouse to three_d_mouse. -->
   <xsl:template match="ThreeDMouse">
      <xsl:element name="three_d_mouse">
         <xsl:apply-templates select="./*" />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!-- ThreeDMouse property "baud". -->
   <xsl:template match="ThreeDMouse/baud">
      <xsl:element name="baud_rate">
         <xsl:apply-templates select="./*" />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>

   <!-- ThreeDMouse property "deviceHost". -->
   <xsl:template match="ThreeDMouse/deviceHost">
      <xsl:element name="device_host">
         <xsl:apply-templates select="./*" />
      </xsl:element>
      <xsl:value-of select="$newline"/>
   </xsl:template>


<!-- AnaProxy ============================================================== -->
   <!-- Rename AnaProxy to analog_proxy. -->
   <xsl:template match="AnaProxy">
      <xsl:element name="analog_proxy">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- SimAnalog ============================================================= -->
   <!-- Rename SimAnalog to simulated_analog_device. -->
   <xsl:template match="SimAnalog">
      <xsl:element name="simulated_analog_device">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- SimAnalog property "eventWindowProxy". -->
   <xsl:template match="SimAnalog/eventWindowProxy">
      <xsl:element name="event_window_proxy">
         <xsl:apply-templates select="./*" />
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
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- AwSoundEngine ========================================================= -->
   <!-- Rename AwSoundEngine to audioworks_sound_engine. -->
   <xsl:template match="AwSoundEngine">
      <xsl:element name="audioworks_sound_engine">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- AwSoundEngine property "adfConfigFile". -->
   <xsl:template match="AwSoundEngine/adfConfigFile">
      <xsl:element name="adf_config_file">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- ClusterManager ======================================================== -->
   <!-- Rename ClusterManager to cluster_manager. -->
   <xsl:template match="ClusterManager">
      <xsl:element name="cluster_manager">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- ClusterManager property "clusterPlugin". -->
   <xsl:template match="ClusterManager/clusterPlugin">
      <xsl:element name="plugin">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- CyberGlove ============================================================ -->
   <!-- Rename CyberGlove to cyber_glove. -->
   <xsl:template match="CyberGlove">
      <xsl:element name="cyber_glove">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- CyberGlove property "baud". -->
   <xsl:template match="CyberGlove/baud">
      <xsl:element name="baud_rate">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- CyberGlove property "calDir". -->
   <xsl:template match="CyberGlove/calDir">
      <xsl:element name="calibration_dir">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- CyberGlove property "glovePos". -->
   <xsl:template match="CyberGlove/glovePos">
      <xsl:element name="glove_position">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- CyberGlove property "deviceHost". -->
   <xsl:template match="CyberGlove/deviceHoste">
      <xsl:element name="device_host">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- DTK =================================================================== -->
   <!-- Rename DTK to dtk. -->
   <xsl:template match="DTK">
      <xsl:element name="dtk">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- DTKsegment ============================================================ -->
   <!-- Rename DTKsegment to dtk_segment. -->
   <xsl:template match="DTKsegment">
      <xsl:element name="dtk_segment">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- DTKsegment property "segmentName". -->
   <xsl:template match="DTKsegment/segmentName">
      <xsl:element name="segment_name">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- DTKsegment property "remoteHost". -->
   <xsl:template match="DTKsegment/remoteHost">
      <xsl:element name="host_name">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- DTKsegment property "dataType". -->
   <xsl:template match="DTKsegment/dataType">
      <xsl:element name="data_type">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- DTKsegment property "itemCount". -->
   <xsl:template match="DTKsegment/itemCount">
      <xsl:element name="item_count">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- DTKsegment property "inputType". -->
   <xsl:template match="DTKsegment/inputType">
      <xsl:element name="input_type">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- DataGlove ============================================================= -->
   <!-- Rename DataGlove to data_glove. -->
   <xsl:template match="DataGlove">
      <xsl:element name="data_glove">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- DataGlove property "baud". -->
   <xsl:template match="DataGlove/baud">
      <xsl:element name="baud_rate">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- DataGlove property "glovePos". -->
   <xsl:template match="DataGlove/glovePos">
      <xsl:element name="glove_position">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>

   <!-- DataGlove property "deviceHost". -->
   <xsl:template match="DataGlove/deviceHost">
      <xsl:element name="device_host">
         <xsl:apply-templates select="./*" />
      </xsl:element>
   </xsl:template>


<!-- EVERYTHING ELSE ======================================================= -->
   <!--
      Copy everything that hasn't already been matched.
   -->
   <xsl:template match="*">
      <xsl:copy-of select="." />
      <xsl:value-of select="$newline"/>
   </xsl:template>

</xsl:stylesheet>
