<?xml version="1.0" encoding="UTF-8"?>
<ChunkDescDB>
  <ChunkDesc token="vjcontrol" name="VjControl Configuration">
    <help>Configuration information for the VjControl GUI itself</help>
    <PropertyDesc token="looknfeel" name="Look and Feel" type="string" num="1">
        <help>The visual 'Look and Feel' to use for the GUI</help>
        <item label="Look and Feel" defaultvalue="Java"/>
        <enumeration name="Java"/>
        <enumeration name="Motif"/>
        <enumeration name="Windows"/>
    </PropertyDesc>
    <PropertyDesc token="host" name="Default Host" type="string" num="1">
        <help>A remote host that VjControl should try to connect to</help>
        <item label="Hostname" defaultvalue="localhost"/>
    </PropertyDesc>
    <PropertyDesc token="port" name="Default Port" type="int" num="1">
        <help>Network Port that VjControl should try to connect to</help>
        <item label="Port Number" defaultvalue="4450"/>
    </PropertyDesc>
    <PropertyDesc token="descfiles" name="Preload ChunkDesc Files" type="string" num="variable">
        <help>The names of Chunk Description files that VjControl should load at startup</help>
        <item label="Preload Chunk Description Files" defaultvalue=""/>
    </PropertyDesc>
    <PropertyDesc token="chunkfiles" name="Preload ConfigChunk Files" type="string" num="variable">
        <help>The names of ConfigChunk files that VjControl should load at startup</help>
        <item label="Preload Config Chunk Files" defaultvalue=""/>
    </PropertyDesc>
    <PropertyDesc token="defaultchunkfiles" name="Default ConfigChunk Files" type="string" num="variable">
        <help>Files that contain default values for new ConfigChunks</help>
        <item label="Default Config Chunk Files" defaultvalue=""/>
    </PropertyDesc>
    <PropertyDesc token="fontname" name="Font Name" type="string" num="1">
        <help>Name of the font to use for the GUI</help>
        <item label="Font Name" defaultvalue=""/>
        <enumeration name="Courier"/>
        <enumeration name="Dialog"/>
        <enumeration name="DialogInput"/>
        <enumeration name="Helvetica"/>
        <enumeration name="TimesRoman"/>
    </PropertyDesc>
    <PropertyDesc token="fontsize" name="Font Size" type="int" num="1">
        <help>Font size to use for GUI</help>
        <item label="Font Size (pts)" defaultvalue="0"/>
    </PropertyDesc>
    <PropertyDesc token="windowsize" name="Window Size" type="int" num="2">
        <help>Maximum size to open vjcontrol windows</help>
        <item label="Width" defaultvalue="0"/>
        <item label="Height" defaultvalue="0"/>
    </PropertyDesc>
    <PropertyDesc token="user_level" name="My skill level is..." type="int" num="1">
        <help>Choose 'Beginner' to hide less-used features.</help>
        <item label="User Level" defaultvalue="0"/>
        <enumeration name="Beginner" value="0"/>
        <enumeration name="Expert" value="10"/>
    </PropertyDesc>
    <PropertyDesc token="user_type" name="I want to be able to..." type="bool" num="5">
        <help>Choose your tasks to decide which panels VjControl displays.</help>
        <item label="Do everything (Power User)" defaultvalue="true"/>
        <item label="Edit Configuration File" defaultvalue="false"/>
        <item label="Control VR Juggler applications" defaultvalue="false"/>
        <item label="Analyze Juggler performance data" defaultvalue="false"/>
        <item label="Define new types of config info" defaultvalue="false"/>
    </PropertyDesc>
  </ChunkDesc>
  <ChunkDesc token="vjIncludeFile" name="Include File">
    <help>File that should be loaded along with this one</help>
  </ChunkDesc>
  <ChunkDesc token="vjc_ChunkPanelComponent" name="VjControl ConfigChunk Editor Component">
    <PropertyDesc token="ClassName" name="Class Name" type="string" num="1">
        <help>Complete package/class name</help>
    </PropertyDesc>
    <PropertyDesc token="Dependencies" name="Dependencies" type="chunk" num="variable">
    </PropertyDesc>
    <PropertyDesc token="ParentComp" name="Parent Component" type="chunk" num="1">
        <help>Component to which to add this</help>
    </PropertyDesc>
    <PropertyDesc token="UsedFor" name="Associated Chunks" type="string" num="variable">
        <help>ConfigChunks which should be editted with this Editor Frame</help>
    </PropertyDesc>
    <PropertyDesc token="user_level" name="Complexity of this interface:" type="int" num="1">
        <item label="User Level" defaultvalue="0"/>
        <enumeration name="Beginner" value="0"/>
        <enumeration name="Expert" value="10"/>
    </PropertyDesc>
    <PropertyDesc token="user_type" name="User tasks this interface is for:" type="bool" num="5">
        <item label="Always Display" defaultvalue="true"/>
        <item label="Config Editing" defaultvalue="false"/>
        <item label="Dynamic Reconfiguration" defaultvalue="false"/>
        <item label="Perf Analysis" defaultvalue="false"/>
        <item label="Advanced Config" defaultvalue="false"/>
    </PropertyDesc>
  </ChunkDesc>
  <ChunkDesc token="vjc_PlugPanelContainer" name="VjControl ControlUI Panel Container">
    <PropertyDesc token="ClassName" name="Class Name" type="string" num="1">
        <help>Complete package/class name</help>
    </PropertyDesc>
    <PropertyDesc token="Dependencies" name="Dependencies" type="chunk" num="variable">
    </PropertyDesc>
    <PropertyDesc token="ParentComp" name="Parent Component" type="chunk" num="1">
        <help>Component to which to add this</help>
    </PropertyDesc>
    <PropertyDesc token="default_panel" name="Default Panel" type="string" num="1">
    </PropertyDesc>
    <PropertyDesc token="user_level" name="Complexity of this interface:" type="int" num="1">
        <item label="User Level" defaultvalue="0"/>
        <enumeration name="Beginner" value="0"/>
        <enumeration name="Expert" value="10"/>
    </PropertyDesc>
    <PropertyDesc token="user_type" name="User tasks this interface is for:" type="bool" num="5">
        <item label="Always Display" defaultvalue="true"/>
        <item label="Config Editing" defualtvalue="false"/>
        <item label="Dynamic Reconfiguration" defualtvalue="false"/>
        <item label="Perf Analysis" defualtvalue="false"/>
        <item label="Advanced Config" defualtvalue="false"/>
    </PropertyDesc>
  </ChunkDesc>
  <ChunkDesc token="vjc_Component" name="VjControl Component Class Registration">
    <help>Registers a VjControl Extension Component class (usually found in contents.config of extension .jars)</help>
  </ChunkDesc>
  <ChunkDesc token="vjc_PlugPanel" name="VjControl PlugPanel Instance">
    <help>Configuration information for a VjControl PlugPanel.</help>
    <PropertyDesc token="ClassName" name="Class Name" type="string" num="1">
    </PropertyDesc>
    <PropertyDesc token="Dependencies" name="Dependencies" type="chunk" num="variable">
    </PropertyDesc>
    <PropertyDesc token="ParentComp" name="Parent Component" type="chunk" num="1">
        <help>Component to which to add this</help>
    </PropertyDesc>
    <PropertyDesc token="Category" name="Categories" type="string" num="variable">
    </PropertyDesc>
    <PropertyDesc token="Hints" name="UI Hints" type="string" num="variable">
    </PropertyDesc>
    <PropertyDesc token="user_level" name="Complexity of this interface:" type="int" num="1">
        <item label="User Level" defaultvalue="0"/>
        <enumeration name="Beginner" value="0"/>
        <enumeration name="Expert" value="10"/>
    </PropertyDesc>
    <PropertyDesc token="user_type" name="User tasks this interface is for:" type="bool" num="5">
        <item label="Always Display" defaultvalue="true"/>
        <item label="Config Editing" defaultvalue="false"/>
        <item label="Dynamic Reconfiguration" defaultvalue="false"/>
        <item label="Perf Analysis" defaultvalue="false"/>
        <item label="Advanced Config" defaultvalue="false"/>
    </PropertyDesc>
  </ChunkDesc>
  <ChunkDesc token="vjc_GenericInstance" name="VjControl Component Instance">
    <PropertyDesc token="ClassName" name="Class Name" type="string" num="1">
    </PropertyDesc>
    <PropertyDesc token="Dependencies" name="Dependencies" type="chunk" num="variable">
    </PropertyDesc>
    <PropertyDesc token="ParentComp" name="Parent Component" type="chunk" num="1">
        <help>Component to which to add this</help>
    </PropertyDesc>
    <PropertyDesc token="user_level" name="Complexity of this interface:" type="int" num="1">
        <item label="User Level" defaultvalue="0"/>
        <enumeration name="Beginner" value="0"/>
        <enumeration name="Expert" value="10"/>
    </PropertyDesc>
    <PropertyDesc token="user_type" name="User tasks this interface is for:" type="bool" num="5">
        <item label="Always Display" defaultvalue="true"/>
        <item label="Config Editing" defaultvalue="false"/>
        <item label="Dynamic Reconfiguration" defaultvalue="false"/>
        <item label="Perf Analysis" defaultvalue="false"/>
        <item label="Advanced Config" defaultvalue="false"/>
    </PropertyDesc>
  </ChunkDesc>
  <ChunkDesc token="PerfData" name="Performance Data Display">
    <help>Information for displaying data from perfbuffers with a matching name.</help>
    <PropertyDesc token="labels" name="Labels" type="string" num="variable">
        <help>Labels for Perf Buffer phases</help>
    </PropertyDesc>
  </ChunkDesc>
</ChunkDescDB>
