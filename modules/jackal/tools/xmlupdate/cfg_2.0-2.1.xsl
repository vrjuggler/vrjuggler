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
  This stylesheet provides an upgrade path for JCCL configuration files
  using version 2.0 of the file format.  Such files can be updated to version
  2.1 using an XSLT processor, as follows:

     xsltproc -o new-file.config cfg_2.0-2.1.xsl old-file.config

  or

     xalan -in old-file.config -xsl cfg_2.0-2.1.xsl -out new-file.config

 -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

  <!-- Define a handy way to insert newlines when necessary. -->
  <xsl:variable name="newline">
<xsl:text>
</xsl:text>
  </xsl:variable>

  <xsl:template match="/">
    <xsl:variable name="version">
      <xsl:value-of select="processing-instruction('org-vrjuggler-jccl-settings')"/>
    </xsl:variable>

    <!--
      This stylesheet is only for use with version 2.0 of the configuration
      file format.
     -->
    <xsl:if test="not(contains($version, 'config.db.version=&quot;2.0&quot;'))">
      <xsl:message terminate="yes">
        <xsl:value-of select="$newline"/>
        <xsl:text>Cannot determine version number or format </xsl:text>
        <xsl:text>version is not 2.0.</xsl:text>
        <xsl:value-of select="$newline"/>
        <xsl:text>Version = '</xsl:text>
        <xsl:value-of select="$version"/>
        <xsl:text>'</xsl:text>
        <xsl:value-of select="$newline"/>
      </xsl:message>
    </xsl:if>

    <!-- Add the new version information. -->
    <xsl:processing-instruction name="org-vrjuggler-jccl-settings">config.db.version="2.1"</xsl:processing-instruction>
    <xsl:value-of select="$newline"/>

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

  <!-- Match vjIncludeFile "chunks". -->
  <xsl:template match="vjIncludeFile">
    <xsl:processing-instruction name="org-vrjuggler-jccl-include">
      <xsl:text>file="</xsl:text>
      <xsl:value-of select="@name"/>
      <xsl:text>"</xsl:text>
    </xsl:processing-instruction>
    <xsl:value-of select="$newline"/>
  </xsl:template>

  <!-- Match vjIncludeDescFile "chunks". -->
  <xsl:template match="vjIncludeDescFile">
    <xsl:processing-instruction name="org-vrjuggler-jccl-includedesc">
      <xsl:text>file="</xsl:text>
      <xsl:value-of select="@name"/>
      <xsl:text>"</xsl:text>
    </xsl:processing-instruction>
    <xsl:value-of select="$newline"/>
  </xsl:template>

  <!-- Leave everything else as-is. -->
  <xsl:template match="*">
    <xsl:copy-of select="."/>
    <xsl:value-of select="$newline"/>
  </xsl:template>

</xsl:stylesheet>
