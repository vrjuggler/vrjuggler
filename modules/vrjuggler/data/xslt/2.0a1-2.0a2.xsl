<?xml version="1.0" encoding="UTF-8"?>

<!-- *********** <auto-copyright.pl BEGIN do not edit this line> *********** -->
<!--
 VR Juggler is (C) Copyright 1998-2005 by Iowa State University

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
  This stylesheet provides an upgrade path for VR Juggler 2.0 Alpha 1
  configuration files to changes made for VR Juggler 2.0 Alpha 2.

     xsltproc -o new-file.config 2.0a1-2.0a2.xsl old-file.config

  or

     xalan -in old-file.config -xsl 2.0a1-2.0a2.xsl -out new-file.config

 -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

  <!-- Define a handy way to insert newlines when necessary. -->
  <xsl:variable name="newline">
<xsl:text>
</xsl:text>
  </xsl:variable>

  <xsl:template match="/">

    <!-- Create the new XML tree. -->
    <xsl:element name="ConfigChunkDB">
      <xsl:value-of select="$newline"/>
      <xsl:for-each select="ConfigChunkDB/*">
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

  <!-- Rename Keyboard elements to EventWindow. -->
  <xsl:template match="Keyboard">
    <xsl:element name="EventWindow">
      <xsl:attribute name="name">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
      <xsl:value-of select="$newline"/>
      <xsl:for-each select="./*">
        <xsl:copy-of select="."/>
        <xsl:value-of select="$newline"/>
      </xsl:for-each>
    </xsl:element>
    <xsl:value-of select="$newline"/>
  </xsl:template>

  <!-- Rename KeyboardProxy elements to EventWindowProxy. -->
  <xsl:template match="KeyboardProxy">
    <xsl:element name="EventWindowProxy">
      <xsl:attribute name="name">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
      <xsl:value-of select="$newline"/>
      <xsl:for-each select="./*">
        <xsl:copy-of select="."/>
        <xsl:value-of select="$newline"/>
      </xsl:for-each>
    </xsl:element>
    <xsl:value-of select="$newline"/>
  </xsl:template>

  <!--
    Lots of changes have been made to displayWindow.  This template handles
    all of them.
  -->
  <xsl:template match="displayWindow">
    <xsl:element name="displayWindow">
      <xsl:attribute name="name">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
      <xsl:value-of select="$newline"/>

      <!--
        Check to see if we have any sim_viewport properties.  If we do, then
        we have to enable this window so that it acts as an event source.
        The test for that is done below.
      -->
      <xsl:variable name="sim_viewport_count">
        <xsl:value-of select="count(sim_viewports)"/>
      </xsl:variable>

      <!--
        Loop over all the properties.  We will look for specific properies
        that have to change.  All others are left untouched.
      -->
      <xsl:for-each select="./*">
        <xsl:choose>
          <!-- act_as_keyboard_device is now act_as_event_source. -->
          <xsl:when test="name() = 'act_as_keyboard_device'">
            <xsl:element name="act_as_event_source">
              <xsl:choose>
                <!--
                  If we have 1 or more sim_viewports properties, this
                  displayWindow has to act as an event source.
                -->
                <xsl:when test="$sim_viewport_count &gt; 0">
                  <xsl:text>true</xsl:text>
                </xsl:when>
                <!-- Otherwise, we leave the setting as it was. -->
                <xsl:otherwise>
                  <xsl:value-of select="."/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:element>
            <xsl:value-of select="$newline"/>
          </xsl:when>
          <!-- keyboard_device_chunk is now event_window_device. -->
          <xsl:when test="name() = 'keyboard_device_chunk'">
            <xsl:element name="event_window_device">
              <xsl:value-of select="$newline"/>

              <!-- event_window_device needs to contain an EventWindow. -->
              <xsl:apply-templates select="./Keyboard"/>
            </xsl:element>
            <xsl:value-of select="$newline"/>
          </xsl:when>
          <!--
            The structure of simViewport elements has changed, so we need to
            apply the simViewport template to each one that we have.
          -->
          <xsl:when test="name() = 'sim_viewports'">
            <xsl:element name="sim_viewports">
              <xsl:value-of select="$newline"/>
              <xsl:apply-templates select="./simViewport"/>
            </xsl:element>
            <xsl:value-of select="$newline"/>
          </xsl:when>
          <!-- All other properties are left untouched. -->
          <xsl:otherwise>
            <xsl:copy-of select="."/>
            <xsl:value-of select="$newline"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:for-each>
    </xsl:element>
    <xsl:value-of select="$newline"/>
  </xsl:template>

  <!--
    This handles updating simViewport elements so that they define a
    simulator plug-in.  This just moves elements around so that the
    simPlugIn property is filled in with a default_simulator element.
  -->
  <xsl:template match="simViewport">
    <xsl:element name="simViewport">
      <xsl:attribute name="name">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
      <xsl:value-of select="$newline"/>

      <!-- Copy what does not need to be moved into default_simulator -->
      <xsl:copy-of select="origin[1]"/>
      <xsl:value-of select="$newline"/>
      <xsl:copy-of select="origin[2]"/>
      <xsl:value-of select="$newline"/>
      <xsl:copy-of select="size[1]"/>
      <xsl:value-of select="$newline"/>
      <xsl:copy-of select="size[2]"/>
      <xsl:value-of select="$newline"/>
      <xsl:copy-of select="view"/>
      <xsl:value-of select="$newline"/>
      <xsl:copy-of select="user"/>
      <xsl:value-of select="$newline"/>
      <xsl:copy-of select="active"/>
      <xsl:value-of select="$newline"/>
      <xsl:copy-of select="vert_fov"/>
      <xsl:value-of select="$newline"/>

      <!--
        Add the simPlugIn property and fill it in using the existing values
        from the old structure.
      -->
      <xsl:element name="simPlugIn">
        <xsl:value-of select="$newline"/>

        <xsl:element name="default_simulator">
          <xsl:attribute name="name">
            <xsl:text>simPlugIn</xsl:text>
          </xsl:attribute>
          <xsl:value-of select="$newline"/>

          <xsl:copy-of select="cameraPos"/>
          <xsl:value-of select="$newline"/>
          <xsl:copy-of select="wandPos"/>
          <xsl:value-of select="$newline"/>
          <xsl:copy-of select="drawProjections"/>
          <xsl:value-of select="$newline"/>
          <xsl:copy-of select="surfaceColor[1]"/>
          <xsl:value-of select="$newline"/>
          <xsl:copy-of select="surfaceColor[2]"/>
          <xsl:value-of select="$newline"/>
          <xsl:copy-of select="surfaceColor[3]"/>
          <xsl:value-of select="$newline"/>

          <!-- These have migrated into default_simulator from apiPerformer. -->
          <xsl:element name="simHeadModel">
            <xsl:text>${VJ_BASE_DIR}/share/vrjuggler/data/models/head.flt</xsl:text>
          </xsl:element>
          <xsl:value-of select="$newline"/>

          <xsl:element name="simWandModel">
            <xsl:text>${VJ_BASE_DIR}/share/vrjuggler/data/models/wand.flt</xsl:text>
          </xsl:element>
          <xsl:value-of select="$newline"/>
        </xsl:element>
        <xsl:value-of select="$newline"/>
      </xsl:element>
      <xsl:value-of select="$newline"/>
    </xsl:element>
    <xsl:value-of select="$newline"/>
  </xsl:template>

  <!-- apiPerformer is no longer used. -->
  <xsl:template match="apiPerformer">
  </xsl:template>

  <!-- The plug-in handling in ClusterManager changed. -->
  <xsl:template match="ClusterManager">
    <xsl:variable name="barrier_node">
      <xsl:value-of select="barrier_master"/>
    </xsl:variable>

    <xsl:element name="ClusterManager">
      <xsl:attribute name="name">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
      <xsl:value-of select="$newline"/>

      <xsl:element name="barrier_master">
        <xsl:value-of select="barrier_master"/>
      </xsl:element>
      <xsl:value-of select="$newline"/>

      <xsl:for-each select="./cluster_nodes">
        <xsl:copy-of select="."/>
        <xsl:value-of select="$newline"/>
      </xsl:for-each>

      <xsl:for-each select="./clusterPlugin">
        <xsl:variable name="plugin">
          <xsl:value-of select="."/>
        </xsl:variable>

        <xsl:element name="clusterPlugin">
          <xsl:text>${VJ_BASE_DIR}/lib/gadgeteer/plugins/lib</xsl:text>
          <xsl:value-of select="."/>
          <xsl:text>.so</xsl:text>
        </xsl:element>
        <xsl:value-of select="$newline"/>
      </xsl:for-each>
    </xsl:element>
    <xsl:value-of select="$newline"/>

    <xsl:element name="SwapLockTCPPlugin">
      <xsl:attribute name="name">
        <xsl:text>SwapLockTCPPlugin</xsl:text>
      </xsl:attribute>
      <xsl:value-of select="$newline"/>

      <xsl:element name="sync_server">
        <xsl:value-of select="$barrier_node"/>
      </xsl:element>
      <xsl:value-of select="$newline"/>

      <xsl:element name="listen_port">
        <xsl:text>8001</xsl:text>
      </xsl:element>
      <xsl:value-of select="$newline"/>
    </xsl:element>
    <xsl:value-of select="$newline"/>

    <xsl:element name="StartBarrierPlugin">
      <xsl:attribute name="name">
        <xsl:text>StartBarrierPlugin</xsl:text>
      </xsl:attribute>
      <xsl:value-of select="$newline"/>

      <xsl:element name="start_master">
        <xsl:value-of select="$barrier_node"/>
      </xsl:element>
      <xsl:value-of select="$newline"/>
    </xsl:element>
    <xsl:value-of select="$newline"/>
  </xsl:template>

  <!--
    XXX: We can't match everything, but we don't have anything specific to
    match because all the elements are user-defined.
  -->
  <xsl:template match="*">
    <!--
      We have to handle elements with keyboardProxy properties differently
      than all other elements.  Those had their keyboardProxy property
      renamed to eventWindowProxy.
    -->
    <xsl:choose>
      <xsl:when test="count(./keyboardProxy) &gt; 0">
        <!--
          Create a new configuration element with the same name as the
          existing element.  Tricky, huh?  :)
        -->
        <xsl:element name="{name()}">
          <xsl:attribute name="name">
            <xsl:value-of select="@name"/>
          </xsl:attribute>
          <xsl:value-of select="$newline"/>

          <!-- Now iterate over the properties, looking for keyboardProxy. -->
          <xsl:for-each select="./*">
            <xsl:choose>
              <!-- Rename keyboardProxy to eventWindowProxy. -->
              <xsl:when test="name() = 'keyboardProxy'">
                <xsl:element name="eventWindowProxy">
                  <xsl:value-of select="."/>
                </xsl:element>
                <xsl:value-of select="$newline"/>
              </xsl:when>
              <!-- Copy everything other property without modification. -->
              <xsl:otherwise>
                <xsl:copy-of select="."/>
                <xsl:value-of select="$newline"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </xsl:element>
        <xsl:value-of select="$newline"/>
      </xsl:when>
      <!--
        Configuration elements that do not have a keyboardProxy property
        are left untouched.
      -->
      <xsl:otherwise>
        <xsl:copy-of select="."/>
        <xsl:value-of select="$newline"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
