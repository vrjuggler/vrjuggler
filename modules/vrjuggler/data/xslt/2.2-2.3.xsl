<?xml version="1.0" encoding="UTF-8"?>

<!-- *********** <auto-copyright.pl BEGIN do not edit this line> *********** -->
<!--
 VR Juggler is (C) Copyright 1998-2007 by Iowa State University

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

   This stylesheet provides an upgrade path for VR Juggler 2.2 configuration
   files to changes made for VR Juggler 2.3.

      xsltproc -o new-file.jconf 2.2-2.3.xsl old-file.jconf

   or

      xalan -in old-file.jconf -xsl 2.2-2.3.xsl -out new-file.jconf

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

   <xsl:template match="jconf:cluster_manager">
      <xsl:choose>
         <xsl:when test="@version = '2'">
            <xsl:element name="cluster_manager">
               <xsl:attribute name="name">
                  <xsl:value-of select="@name" />
               </xsl:attribute>
               <xsl:attribute name="version">
                  <xsl:text>3</xsl:text>
               </xsl:attribute>

               <xsl:for-each select="./jconf:plugin">
                  <xsl:variable name="plugin_name">
                     <xsl:value-of select="." />
                  </xsl:variable>
                  <xsl:choose>
                     <xsl:when test="$plugin_name = 'StartBarrierPlugin'">
                        <xsl:message>
                           <xsl:text>Removing reference to StartBarrierPlugin.</xsl:text>
                        </xsl:message>
                     </xsl:when>
                     <xsl:when test="$plugin_name = 'SwapLockTCPPlugin'">
                        <xsl:message>
                           <xsl:text>Removing reference to SwapLockTCPPlugin.</xsl:text>
                        </xsl:message>
                     </xsl:when>
                     <xsl:when test="$plugin_name = 'SwapLockWiredPlugin'">
                        <xsl:message>
                           <xsl:text>Removing reference to SwapLockWiredPlugin.</xsl:text>
                        </xsl:message>
                     </xsl:when>
                     <xsl:otherwise>
                        <xsl:copy-of select="." />
                        <xsl:value-of select="$newline" />
                     </xsl:otherwise>
                  </xsl:choose>
               </xsl:for-each>

               <xsl:element name="use_software_swap_lock">
                  <xsl:choose>
                     <xsl:when test="count(../jconf:swap_lock_tcp_plugin) &gt; 0">
                        <xsl:text>true</xsl:text>
                     </xsl:when>
                     <xsl:when test="count(../jconf:swap_lock_wired_plugin) &gt; 0">
                        <xsl:text>true</xsl:text>
                     </xsl:when>
                     <xsl:otherwise>
                        <xsl:text>false</xsl:text>
                     </xsl:otherwise>
                  </xsl:choose>
               </xsl:element>
               <xsl:value-of select="$newline" />
            </xsl:element>
         </xsl:when>
         <xsl:otherwise>
            <xsl:copy-of select="."/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!-- Remove config elements for defunct plug-ins. -->
   <xsl:template match="jconf:start_barrier_plugin">
      <xsl:message>
         <xsl:text>Removing start_barrier_plugin element.</xsl:text>
      </xsl:message>
   </xsl:template>

   <xsl:template match="jconf:swap_lock_tcp_plugin">
      <xsl:message>
         <xsl:text>Removing swap_lock_tcp_plugin element.</xsl:text>
      </xsl:message>
   </xsl:template>

   <xsl:template match="jconf:swap_lock_wired_plugin">
      <xsl:message>
         <xsl:text>Removing swap_lock_wired_plugin element.</xsl:text>
      </xsl:message>
   </xsl:template>

   <xsl:template match="jconf:application_data">
      <xsl:message>
         <xsl:text>Removing application_data element.</xsl:text>
      </xsl:message>
   </xsl:template>

<!-- EVERYTHING ELSE ======================================================= -->
   <!--
      Copy everything that hasn't already been matched.
   -->
   <xsl:template match="*">
      <xsl:choose>
         <xsl:when test="count(./jconf:device_host) &gt; 0">
            <xsl:message>
               <xsl:text>Removing device_host property from </xsl:text>
               <xsl:value-of select="@name" />
            </xsl:message>
            <xsl:element name="{name()}">
               <xsl:attribute name="name">
                  <xsl:value-of select="@name" />
               </xsl:attribute>
               <xsl:attribute name="version">
                  <xsl:value-of select="@version" />
               </xsl:attribute>
               <xsl:for-each select="./*">
                  <xsl:variable name="elt_name">
                     <xsl:value-of select="name(.)" />
                  </xsl:variable>
                  <xsl:if test="$elt_name != 'device_host'">
                     <xsl:copy-of select="." />
                     <xsl:value-of select="$newline" />
                  </xsl:if>
               </xsl:for-each>
            </xsl:element>
         </xsl:when>
         <xsl:otherwise>
            <xsl:copy-of select="." />
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>
</xsl:stylesheet>
