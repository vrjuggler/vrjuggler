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

   This stylesheet provides a way to switch references to the numeric value
   of a keyboard key, mouse button, or mouse motion to the symbolic value.
   This only works with VR Juggler 2.0 configurations and is intended to aid
   with the migration to VR Juggler 2.2 and beyond.

      xsltproc -o new-file.jconf key_sym_2_0.xsl old-file.jconf

   or

      xalan -in old-file.jconf -xsl key_sym_2_0.xsl -out new-file.jconf

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

   <xsl:template match="jconf:include">
      <xsl:copy-of select="." />
   </xsl:template>

   <xsl:template match="jconf:elements">
      <xsl:copy>
         <xsl:apply-templates />
      </xsl:copy>
   </xsl:template>

   <xsl:template match="jconf:modifier_key">
      <xsl:variable name="modifier">
         <xsl:value-of select="." />
      </xsl:variable>
      <xsl:element name="modifier_key">
         <xsl:call-template name="changeModifier">
            <xsl:with-param name="modifier" select="$modifier" />
         </xsl:call-template>
      </xsl:element>
   </xsl:template>

   <xsl:template match="jconf:key">
      <xsl:variable name="key">
         <xsl:value-of select="." />
      </xsl:variable>
      <xsl:element name="key">
         <xsl:call-template name="changeKey">
            <xsl:with-param name="key" select="$key" />
         </xsl:call-template>
      </xsl:element>
   </xsl:template>

   <xsl:template match="jconf:lock_key">
      <xsl:variable name="key">
         <xsl:value-of select="." />
      </xsl:variable>
      <xsl:element name="lock_key">
         <xsl:call-template name="changeKey">
            <xsl:with-param name="key" select="$key" />
         </xsl:call-template>
      </xsl:element>
   </xsl:template>

<!-- EVERYTHING ELSE ====================================================== -->
   <!--
      Perform a recursive copy everything that hasn't already been matched.
      We do this one level at a time.
   -->
   <xsl:template match="*">
      <xsl:copy select=".">
         <!-- Copy any and all attributes. -->
         <xsl:copy-of select="./@*" />
         <!-- Process children. -->
         <xsl:apply-templates />
      </xsl:copy>
   </xsl:template>

   <xsl:template name="changeKey">
      <xsl:param name="key" />

      <xsl:choose>
         <xsl:when test="$key = '0'">
            <xsl:text>KEY_NONE</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '1'">
            <xsl:text>KEY_UP</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '2'">
            <xsl:text>KEY_DOWN</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '3'">
            <xsl:text>KEY_LEFT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '4'">
            <xsl:text>KEY_RIGHT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '5'">
            <xsl:text>KEY_CTRL</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '6'">
            <xsl:text>KEY_SHIFT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '7'">
            <xsl:text>KEY_ALT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '8'">
            <xsl:text>KEY_1</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '9'">
            <xsl:text>KEY_2</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '10'">
            <xsl:text>KEY_3</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '11'">
            <xsl:text>KEY_4</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '12'">
            <xsl:text>KEY_5</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '13'">
            <xsl:text>KEY_6</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '14'">
            <xsl:text>KEY_7</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '15'">
            <xsl:text>KEY_8</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '16'">
            <xsl:text>KEY_9</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '17'">
            <xsl:text>KEY_0</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '18'">
            <xsl:text>KEY_A</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '19'">
            <xsl:text>KEY_B</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '20'">
            <xsl:text>KEY_C</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '21'">
            <xsl:text>KEY_D</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '22'">
            <xsl:text>KEY_E</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '23'">
            <xsl:text>KEY_F</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '24'">
            <xsl:text>KEY_G</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '25'">
            <xsl:text>KEY_H</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '26'">
            <xsl:text>KEY_I</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '27'">
            <xsl:text>KEY_J</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '28'">
            <xsl:text>KEY_K</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '29'">
            <xsl:text>KEY_L</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '30'">
            <xsl:text>KEY_M</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '31'">
            <xsl:text>KEY_N</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '32'">
            <xsl:text>KEY_O</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '33'">
            <xsl:text>KEY_P</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '34'">
            <xsl:text>KEY_Q</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '35'">
            <xsl:text>KEY_R</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '36'">
            <xsl:text>KEY_S</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '37'">
            <xsl:text>KEY_T</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '38'">
            <xsl:text>KEY_U</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '39'">
            <xsl:text>KEY_V</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '40'">
            <xsl:text>KEY_W</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '41'">
            <xsl:text>KEY_X</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '42'">
            <xsl:text>KEY_Y</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '43'">
            <xsl:text>KEY_Z</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '44'">
            <xsl:text>KEY_ESC</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '45'">
            <xsl:text>MOUSE_POSX</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '46'">
            <xsl:text>MOUSE_NEGX</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '47'">
            <xsl:text>MOUSE_POSY</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '48'">
            <xsl:text>MOUSE_NEGY</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '49'">
            <xsl:text>MOUSE_BUT1</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '50'">
            <xsl:text>MOUSE_BUT2</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '51'">
            <xsl:text>MOUSE_BUT3</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '52'">
            <xsl:text>MOUSE_BUT4</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '53'">
            <xsl:text>MOUSE_BUT5</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '54'">
            <xsl:text>NO_MBUTTON</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '55'">
            <xsl:text>KEY_TAB</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '56'">
            <xsl:text>KEY_BACKTAB</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '57'">
            <xsl:text>KEY_BACKSPACE</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '58'">
            <xsl:text>KEY_RETURN</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '59'">
            <xsl:text>KEY_ENTER</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '60'">
            <xsl:text>KEY_INSERT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '61'">
            <xsl:text>KEY_DELETE</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '62'">
            <xsl:text>KEY_PAUSE</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '63'">
            <xsl:text>KEY_PRINT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '64'">
            <xsl:text>KEY_SYSREQ</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '65'">
            <xsl:text>KEY_HOME</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '66'">
            <xsl:text>KEY_END</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '67'">
            <xsl:text>KEY_PRIOR</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '68'">
            <xsl:text>KEY_NEXT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '69'">
            <xsl:text>KEY_CAPS_LOCK</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '70'">
            <xsl:text>KEY_NUM_LOCK</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '71'">
            <xsl:text>KEY_SCROLL_LOCK</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '72'">
            <xsl:text>KEY_F1</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '73'">
            <xsl:text>KEY_F2</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '74'">
            <xsl:text>KEY_F3</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '75'">
            <xsl:text>KEY_F4</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '76'">
            <xsl:text>KEY_F5</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '77'">
            <xsl:text>KEY_F6</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '78'">
            <xsl:text>KEY_F7</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '79'">
            <xsl:text>KEY_F8</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '80'">
            <xsl:text>KEY_F9</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '81'">
            <xsl:text>KEY_F10</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '82'">
            <xsl:text>KEY_F11</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '83'">
            <xsl:text>KEY_F12</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '84'">
            <xsl:text>KEY_F13</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '85'">
            <xsl:text>KEY_F14</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '86'">
            <xsl:text>KEY_F15</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '87'">
            <xsl:text>KEY_F16</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '88'">
            <xsl:text>KEY_F17</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '89'">
            <xsl:text>KEY_F18</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '90'">
            <xsl:text>KEY_F19</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '91'">
            <xsl:text>KEY_F20</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '92'">
            <xsl:text>KEY_F21</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '93'">
            <xsl:text>KEY_F22</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '94'">
            <xsl:text>KEY_F23</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '95'">
            <xsl:text>KEY_F24</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '96'">
            <xsl:text>KEY_F25</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '97'">
            <xsl:text>KEY_F26</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '98'">
            <xsl:text>KEY_F27</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '99'">
            <xsl:text>KEY_F28</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '100'">
            <xsl:text>KEY_F29</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '101'">
            <xsl:text>KEY_F30</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '102'">
            <xsl:text>KEY_F31</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '103'">
            <xsl:text>KEY_F32</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '104'">
            <xsl:text>KEY_F33</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '105'">
            <xsl:text>KEY_F34</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '106'">
            <xsl:text>KEY_F35</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '107'">
            <xsl:text>KEY_SUPER_L</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '108'">
            <xsl:text>KEY_SUPER_R</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '109'">
            <xsl:text>KEY_MENU</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '110'">
            <xsl:text>KEY_HYPER_L</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '111'">
            <xsl:text>KEY_HYPER_R</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '112'">
            <xsl:text>KEY_HELP</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '113'">
            <xsl:text>KEY_SPACE</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '114'">
            <xsl:text>KEY_ANY</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '115'">
            <xsl:text>KEY_EXCLAM</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '116'">
            <xsl:text>KEY_QUOTE_DBL</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '117'">
            <xsl:text>KEY_NUMBER_SIGN</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '118'">
            <xsl:text>KEY_DOLLAR</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '119'">
            <xsl:text>KEY_PERCENT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '120'">
            <xsl:text>KEY_AMPERSAND</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '121'">
            <xsl:text>KEY_APOSTROPHE</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '122'">
            <xsl:text>KEY_PAREN_LEFT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '123'">
            <xsl:text>KEY_PAREN_RIGHT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '124'">
            <xsl:text>KEY_ASTERISK</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '125'">
            <xsl:text>KEY_PLUS</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '126'">
            <xsl:text>KEY_COMMA</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '127'">
            <xsl:text>KEY_MINUS</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '128'">
            <xsl:text>KEY_PERIOD</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '129'">
            <xsl:text>KEY_SLASH</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '130'">
            <xsl:text>KEY_COLON</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '131'">
            <xsl:text>KEY_SEMICOLON</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '132'">
            <xsl:text>KEY_LESS</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '133'">
            <xsl:text>KEY_EQUAL</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '134'">
            <xsl:text>KEY_GREATER</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '135'">
            <xsl:text>KEY_QUESTION</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '136'">
            <xsl:text>KEY_AT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '137'">
            <xsl:text>KEY_BRACKET_LEFT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '138'">
            <xsl:text>KEY_BACKSLASH</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '139'">
            <xsl:text>KEY_BRACKET_RIGHT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '140'">
            <xsl:text>KEY_ASCII_CIRCUM</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '141'">
            <xsl:text>KEY_UNDERSCORE</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '142'">
            <xsl:text>KEY_QUOTE_LEFT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '143'">
            <xsl:text>KEY_BRACE_LEFT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '144'">
            <xsl:text>KEY_BAR</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '145'">
            <xsl:text>KEY_BRACE_RIGHT</xsl:text>
         </xsl:when>
         <xsl:when test="$key = '146'">
            <xsl:text>KEY_ASCII_TILDE</xsl:text>
         </xsl:when>
         <xsl:otherwise>
            <xsl:value-of select="$key" />
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <xsl:template name="changeModifier">
      <xsl:param name="modifier" />

      <xsl:choose>
         <xsl:when test="$modifier = '0'">
            <xsl:text>NONE</xsl:text>
         </xsl:when>
         <xsl:when test="$modifier = '5'">
            <xsl:text>SHIFT</xsl:text>
         </xsl:when>
         <xsl:when test="$modifier = '6'">
            <xsl:text>CTRL</xsl:text>
         </xsl:when>
         <xsl:when test="$modifier = '7'">
            <xsl:text>ALT</xsl:text>
         </xsl:when>
         <xsl:when test="$modifier = '-1'">
            <xsl:text>ANY</xsl:text>
         </xsl:when>
         <xsl:otherwise>
            <xsl:value-of select="$modifier" />
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>
</xsl:stylesheet>
