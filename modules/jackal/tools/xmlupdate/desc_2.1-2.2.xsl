<?xml version="1.0" encoding="UTF-8" ?>

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
  This stylesheet provides an upgrade path for JCCL chunk description
  files using version 2.1 of the file format.  Such files can be updated to
  version 2.2 using an XSLT processor, as follows:

     xsltproc -o new-chunks.desc desc_2.1-2.2.xsl old-chunks.desc

  or

     xalan -in old-chunks.desc -xsl desc_2.1-2.2.xsl -out new-chunks.desc

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
      This stylesheet is only for use with version 2.1 of the chunk
      description file format.
     -->
    <xsl:if test="not(contains($version, 'desc.db.version=&quot;2.1&quot;'))">
      <xsl:message terminate="yes">
        <xsl:value-of select="$newline"/>
        <xsl:text>Cannot determine version number or description </xsl:text>
        <xsl:text>version is not 2.1.</xsl:text>
        <xsl:value-of select="$newline"/>
        <xsl:text>Version = '</xsl:text>
        <xsl:value-of select="$version"/>
        <xsl:text>'</xsl:text>
        <xsl:value-of select="$newline"/>
      </xsl:message>
    </xsl:if>

    <!-- Add the new version information. -->
    <xsl:processing-instruction name="org-vrjuggler-jccl-settings">desc.db.version="2.2"</xsl:processing-instruction>
    <xsl:value-of select="$newline"/>

    <!-- Create the new XML tree. -->
    <xsl:element name="ChunkDescDB">
      <xsl:for-each select="ChunkDescDB/ChunkDesc">
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

  <xsl:template match="processing-instruction()">
    <xsl:copy-of select="."/>
    <xsl:value-of select="$newline"/>
  </xsl:template>

  <!-- Match chunk descriptions. -->
  <!--
    XXX: Is there an easier way to do this?  What I really want is to copy
    the existing <ChunkDesc> element and then work on its <PropertyDesc>
    children.
  -->
  <xsl:template match="ChunkDesc">
    <xsl:element name="ChunkDesc">
      <xsl:attribute name="token">
        <xsl:value-of select="@token"/>
      </xsl:attribute>
      <xsl:attribute name="name">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
      <xsl:value-of select="$newline"/>

      <!-- Retain the existing category(ies). -->
      <xsl:for-each select="category">
        <xsl:copy-of select="."/>
        <xsl:value-of select="$newline"/>
      </xsl:for-each>

      <!-- Add help information if there is any. -->
      <xsl:element name="help">
        <xsl:value-of select="help"/>
      </xsl:element>
      <xsl:value-of select="$newline"/>

      <!-- Here is where the real work begins. -->
      <xsl:for-each select="PropertyDesc">
        <xsl:apply-templates select="."/>
      </xsl:for-each>
    </xsl:element>
  </xsl:template>

  <!-- Match property descriptions. -->
  <xsl:template match="PropertyDesc">
    <!--
      Print the opening PropertyDesc element.  There is no change in this
      formatting between 2.1 and 2.2.
     -->
    <xsl:element name="PropertyDesc">
      <xsl:attribute name="token">
        <xsl:value-of select="@token"/>
      </xsl:attribute>
      <xsl:attribute name="name">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
      <xsl:attribute name="type">
        <xsl:value-of select="@type"/>
      </xsl:attribute>
      <xsl:attribute name="variable">
        <xsl:value-of select="@variable"/>
      </xsl:attribute>
      <xsl:value-of select="$newline"/>

      <!--
        Print help information (if any is available).  We use <xsl:element>
        here to ensure that the resulting <PropertyDesc> element has a
        <help> child, even if the source document does not have help
        information for the current <PropertyDesc>.
       -->
      <xsl:element name="help">
        <xsl:value-of select="help"/>
      </xsl:element>
      <xsl:value-of select="$newline"/>

      <!-- Handle the existing <item> children (there may be zero). -->
      <xsl:for-each select="item">
        <xsl:copy-of select="."/>
        <xsl:value-of select="$newline"/>
      </xsl:for-each>

      <!--
        Handle <enumeration> children.  This is where the format change
        exists.  The allowed chunk types that can be used with properties of
        type 'chunk' or 'embeddedchunk' are no longer specified with the
        enumeration stuff (which was such a hack!).  Instead, there is a
        new child called <allowedtype>.
      -->
      <xsl:for-each select="enumeration">
        <xsl:choose>
          <!-- XXX: A boolean OR here would be better.  Is that possible? -->
          <xsl:when test="../@type = 'chunk'">
            <xsl:element name="allowedtype">
              <xsl:value-of select="@name" />
            </xsl:element>
          </xsl:when>
          <xsl:when test="../@type = 'embeddedchunk'">
            <xsl:element name="allowedtype">
              <xsl:value-of select="@name" />
            </xsl:element>
          </xsl:when>
          <xsl:otherwise>
            <xsl:copy-of select="." />
          </xsl:otherwise>
        </xsl:choose>
        <xsl:value-of select="$newline" />
      </xsl:for-each>

    </xsl:element>
    <xsl:value-of select="$newline"/>
  </xsl:template>
</xsl:stylesheet>
