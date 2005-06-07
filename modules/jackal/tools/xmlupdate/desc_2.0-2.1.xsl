<?xml version="1.0" encoding="UTF-8" ?>

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
  This stylesheet provides an upgrade path for JCCL chunk description
  files using version 2.0 of the file format.  Such files can be updated to
  version 2.1 using an XSLT processor, as follows:

     xsltproc -o new-chunks.desc desc_2.0-2.1.xsl old-chunks.desc

  or

     xalan -in old-chunks.desc -xsl desc_2.0-2.1.xsl -out new-chunks.desc

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
      This stylesheet is only for use with version 2.0 of the chunk
      description file format.
     -->
    <xsl:if test="not(contains($version, 'desc.db.version=&quot;2.0&quot;'))">
      <xsl:message terminate="yes">
        <xsl:value-of select="$newline"/>
        <xsl:text>Cannot determine version number or description </xsl:text>
        <xsl:text>version is not 2.0.</xsl:text>
        <xsl:value-of select="$newline"/>
        <xsl:text>Version = '</xsl:text>
        <xsl:value-of select="$version"/>
        <xsl:text>'</xsl:text>
        <xsl:value-of select="$newline"/>
      </xsl:message>
    </xsl:if>

    <!-- Add the new version information. -->
    <xsl:processing-instruction name="org-vrjuggler-jccl-settings">desc.db.version="2.1"</xsl:processing-instruction>
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
  <xsl:template match="ChunkDesc">
    <xsl:element name="ChunkDesc">
      <xsl:attribute name="token">
        <xsl:value-of select="@token"/>
      </xsl:attribute>
      <xsl:attribute name="name">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
      <xsl:value-of select="$newline"/>

      <xsl:choose>
        <!-- Add a category for this if it does not already have one. -->
        <xsl:when test="count(category) &lt; 1">
          <xsl:element name="category">
            <xsl:text>Unassigned</xsl:text>
          </xsl:element>
          <xsl:value-of select="$newline"/>
        </xsl:when>
        <!-- Otherwise, copy in the existing category(ies). -->
        <xsl:otherwise>
          <xsl:for-each select="category">
            <xsl:copy-of select="."/>
            <xsl:value-of select="$newline"/>
          </xsl:for-each>
        </xsl:otherwise>
      </xsl:choose>

      <!-- Add help information if there is any. -->
      <xsl:element name="help">
        <xsl:value-of select="help"/>
      </xsl:element>
      <xsl:value-of select="$newline"/>

      <xsl:for-each select="PropertyDesc">
        <xsl:apply-templates select="."/>
      </xsl:for-each>
    </xsl:element>
  </xsl:template>

  <xsl:template match="PropertyDesc">
    <xsl:variable name="is_variable">
      <xsl:choose>
        <xsl:when test="@num = 'variable'">
          <xsl:text>1</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>0</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="default_value">
      <xsl:choose>
        <xsl:when test="@type = 'string'">
          <xsl:text></xsl:text>
        </xsl:when>
        <xsl:when test="@type = 'bool'">
          <xsl:text>0</xsl:text>
        </xsl:when>
        <xsl:when test="@type = 'int'">
          <xsl:text>0</xsl:text>
        </xsl:when>
        <xsl:when test="@type = 'float'">
          <xsl:text>0.0</xsl:text>
        </xsl:when>
        <xsl:when test="@type = 'chunk'">
          <xsl:text></xsl:text>
        </xsl:when>
<!--
        <xsl:when test="@type = 'embeddedchunk'">
          <xsl:text></xsl:text>
        </xsl:when>
-->
      </xsl:choose>
    </xsl:variable>

    <!--
      Define the total count for <item> children.  For variable-count
      properties, this is always 1.  Otherwise, it is the value previously
      held in the 'num' attribute.
     -->
    <xsl:variable name="total_item_count">
      <xsl:choose>
        <xsl:when test="$is_variable = 1">
          <xsl:text>1</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="@num"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!--
      Print the opening PropertyDesc element.  This contains the key
      change between version 2.0 and 2.1: removing the 'num' attribute
      and adding the boolean-valued 'variable' attribute.  The value
      previously held by the 'num' attribute is now represented by the
      number of <item> children.  See below for the handling of <item>
      children.
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
        <xsl:value-of select="$is_variable"/>
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
        Add any missing <item> elements.  This is the second major change
        between version 2.0 and 2.1 of the chunk description format.
      
        We use the for loop emulator template to do the addition of elements.
        The below is the equivalent of the following C-style statement:

          for ( i = cur_item_count; i < total_item_count; ++i )
          {
             /* Add <item> element... */ ;
          }
     -->
      <xsl:call-template name="for-loop">
        <xsl:with-param name="type"         select="@type"/>
        <xsl:with-param name="label"        select="@name"/>
        <xsl:with-param name="defaultValue" select="$default_value"/>
        <xsl:with-param name="i"            select="count(item)"/>
        <xsl:with-param name="increment"    select="1"/>
        <xsl:with-param name="operator"     select="'&lt;'"/>
        <xsl:with-param name="testValue"    select="$total_item_count"/>
      </xsl:call-template>

      <xsl:for-each select="enumeration">
        <xsl:copy-of select="."/>
        <xsl:value-of select="$newline"/>
      </xsl:for-each>

    </xsl:element>
    <xsl:value-of select="$newline"/>
  </xsl:template>

  <!-- Generator of <item> elements. -->
  <xsl:template name="itemGenerate">
    <xsl:param name="type"/>
    <xsl:param name="label"/>
    <xsl:param name="defaultValue" select="''"/>

    <xsl:element name="item">
      <xsl:attribute name="label">
        <xsl:value-of select="$label"/>
      </xsl:attribute>

      <!-- We do not set default values for embedded chunk items. -->
      <xsl:if test="$type != 'embeddedchunk'">
        <xsl:attribute name="defaultvalue">
          <xsl:value-of select="$defaultValue"/>
        </xsl:attribute>
      </xsl:if>
    </xsl:element>

    <xsl:value-of select="$newline"/>
  </xsl:template>

  <!--
    This template emulates the behavior of a traditional for loop.  It
    comes from Chapter 4 of _Mastering XML Transformations_ (pp. 85-89)
    by Doug Tidwell.
   -->
  <xsl:template name="for-loop">
    <xsl:param name="type"/>
    <xsl:param name="label"/>
    <xsl:param name="defaultValue"/>
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
      <xsl:variable name="temp_label_val">
        <xsl:value-of select="$label"/>
        <xsl:text> (</xsl:text>
        <xsl:value-of select="$i"/>
        <xsl:text>)</xsl:text>
      </xsl:variable>
      <xsl:call-template name="itemGenerate">
        <xsl:with-param name="type"         select="$type"/>
        <xsl:with-param name="label"        select="$temp_label_val"/>
        <xsl:with-param name="defaultValue" select="$defaultValue"/>
      </xsl:call-template>

      <!-- Your logic should end here; don't change the rest of this        -->
      <!-- template!                                                        -->

      <!-- Now for the important part: we increment the index variable and  -->
      <!-- loop.  Notice that we're passing the incremented value, not      -->
      <!-- changing the variable itself.                                    -->

      <xsl:call-template name="for-loop">
        <xsl:with-param name="type"         select="$type"/>
        <xsl:with-param name="label"        select="$label"/>
        <xsl:with-param name="defaultValue" select="$defaultValue"/>
        <xsl:with-param name="i"            select="$i + $increment"/>
        <xsl:with-param name="increment"    select="$increment"/>
        <xsl:with-param name="operator"     select="$operator"/>
        <xsl:with-param name="testValue"    select="$testValue"/>
        <xsl:with-param name="iteration"    select="$iteration + 1"/>
      </xsl:call-template>
    </xsl:if> 
  </xsl:template>
</xsl:stylesheet>
