<ChunkDescDB>
  <ChunkDesc token="vjcontrol" name="VjControl Configuration">
    <help>Configuration information for the VjControl GUI itself</help>
    <PropertyDesc token="Name" name="Name" type="String" num="1">
        <help>Unique name of an instance of this chunk type</help>
    </PropertyDesc>
    <PropertyDesc token="looknfeel" name="Look and Feel" type="String" num="1">
        <help>The visual 'Look and Feel' to use for the GUI</help>
        <enumeration name="Java"/>
        <enumeration name="Motif"/>
        <enumeration name="Windows"/>
    </PropertyDesc>
    <PropertyDesc token="host" name="Default Host" type="String" num="1">
        <help>A remote host that VjControl should try to connect to</help>
    </PropertyDesc>
    <PropertyDesc token="port" name="Default Port" type="Int" num="1">
        <help>Network Port that VjControl should try to connect to</help>
    </PropertyDesc>
    <PropertyDesc token="descfiles" name="Preload ChunkDesc Files" type="String" num="variable">
        <help>The names of Chunk Description files that VjControl should load at startup</help>
    </PropertyDesc>
    <PropertyDesc token="chunkfiles" name="Preload ConfigChunk Files" type="String" num="variable">
        <help>The names of ConfigChunk files that VjControl should load at startup</help>
    </PropertyDesc>
    <PropertyDesc token="defaultchunkfiles" name="Default ConfigChunk Files" type="String" num="variable">
        <help>Files that contain default values for new ConfigChunks</help>
    </PropertyDesc>
    <PropertyDesc token="fontname" name="Font Name" type="String" num="1">
        <help>Name of the font to use for the GUI</help>
        <enumeration name="Courier"/>
        <enumeration name="Dialog"/>
        <enumeration name="DialogInput"/>
        <enumeration name="Helvetica"/>
        <enumeration name="TimesRoman"/>
    </PropertyDesc>
    <PropertyDesc token="fontsize" name="Font Size" type="Int" num="1">
        <help>Font size to use for GUI</help>
    </PropertyDesc>
    <PropertyDesc token="windowsize" name="Max Window Size" type="Int" num="2">
        <help>Maximum size to open vjcontrol windows</help>
        <label name="Width"/>
        <label name="Height"/>
    </PropertyDesc>
    <PropertyDesc token="user_level" name="My skill level is..." type="Int" num="1">
        <help>Choose 'Beginner' to hide less-used features.</help>
        <enumeration name="Beginner" value="0"/>
        <enumeration name="Expert" value="10"/>
    </PropertyDesc>
    <PropertyDesc token="user_type" name="I want to be able to..." type="Bool" num="5">
        <help>Choose your tasks to decide which panels VjControl displays.</help>
        <label name="Do everything (Power User)"/>
        <label name="Edit Configuration File"/>
        <label name="Control VR Juggler applications"/>
        <label name="Analyze Juggler performance data"/>
        <label name="Define new types of config info"/>
    </PropertyDesc>
    <Defaults>
      <vjcontrol name="">
        <looknfeel>"Java" </looknfeel>
        <host>"localhost" </host>
        <port>"4450" </port>
        <descfiles></descfiles>
        <chunkfiles></chunkfiles>
        <defaultchunkfiles></defaultchunkfiles>
        <fontname>"" </fontname>
        <fontsize>"0" </fontsize>
        <windowsize>"0" "0" </windowsize>
        <user_level>"0" </user_level>
        <user_type>"True" "False" "False" "False" "False" </user_type>
      </vjcontrol>
    </Defaults>
  </ChunkDesc>
  <ChunkDesc token="vjIncludeFile" name="Include File">
    <help>File that should be loaded along with this one</help>
    <PropertyDesc token="Name" name="Name" type="String" num="1">
        <help>Unique name of an instance of this chunk type</help>
    </PropertyDesc>
  </ChunkDesc>
  <ChunkDesc token="vjc_ChunkPanelComponent" name="VjControl ConfigChunk Editor Component">
    <PropertyDesc token="Name" name="Name" type="String" num="1">
        <help>Unique name of an instance of this chunk type</help>
    </PropertyDesc>
    <PropertyDesc token="ClassName" name="Class Name" type="String" num="1">
        <help>Complete package/class name</help>
    </PropertyDesc>
    <PropertyDesc token="Dependencies" name="Dependencies" type="Chunk" num="variable">
    </PropertyDesc>
    <PropertyDesc token="ParentComp" name="Parent Component" type="Chunk" num="1">
        <help>Component to which to add this</help>
    </PropertyDesc>
    <PropertyDesc token="UsedFor" name="Associated Chunks" type="String" num="variable">
        <help>ConfigChunks which should be editted with this Editor Frame</help>
    </PropertyDesc>
    <PropertyDesc token="user_level" name="Complexity of this interface:" type="Int" num="1">
        <enumeration name="Beginner" value="0"/>
        <enumeration name="Expert" value="10"/>
    </PropertyDesc>
    <PropertyDesc token="user_type" name="User tasks this interface is for:" type="Bool" num="5">
        <label name="Always Display"/>
        <label name="Config Editing"/>
        <label name="Dynamic Reconfiguration"/>
        <label name="Perf Analysis"/>
        <label name="Advanced Config"/>
    </PropertyDesc>
    <Defaults>
      <vjc_ChunkPanelComponent name="">
        <ClassName>"" </ClassName>
        <Dependencies></Dependencies>
        <ParentComp>"" </ParentComp>
        <UsedFor></UsedFor>
        <user_level>"0" </user_level>
        <user_type>"True" "False" "False" "False" "False" </user_type>
      </vjc_ChunkPanelComponent>
    </Defaults>
  </ChunkDesc>
  <ChunkDesc token="vjc_PlugPanelContainer" name="VjControl ControlUI Panel Container">
    <PropertyDesc token="Name" name="Name" type="String" num="1">
        <help>Unique name of an instance of this chunk type</help>
    </PropertyDesc>
    <PropertyDesc token="ClassName" name="Class Name" type="String" num="1">
        <help>Complete package/class name</help>
    </PropertyDesc>
    <PropertyDesc token="Dependencies" name="Dependencies" type="Chunk" num="variable">
    </PropertyDesc>
    <PropertyDesc token="ParentComp" name="Parent Component" type="Chunk" num="1">
        <help>Component to which to add this</help>
    </PropertyDesc>
    <PropertyDesc token="default_panel" name="Default Panel" type="String" num="1">
    </PropertyDesc>
    <PropertyDesc token="user_level" name="Complexity of this interface:" type="Int" num="1">
        <enumeration name="Beginner" value="0"/>
        <enumeration name="Expert" value="10"/>
    </PropertyDesc>
    <PropertyDesc token="user_type" name="User tasks this interface is for:" type="Bool" num="5">
        <label name="Always Display"/>
        <label name="Config Editing"/>
        <label name="Dynamic Reconfiguration"/>
        <label name="Perf Analysis"/>
        <label name="Advanced Config"/>
    </PropertyDesc>
    <Defaults>
      <vjc_PlugPanelContainer name="">
        <ClassName>"" </ClassName>
        <Dependencies></Dependencies>
        <ParentComp>"" </ParentComp>
        <default_panel>"" </default_panel>
        <user_level>"0" </user_level>
        <user_type>"True" "False" "False" "False" "False" </user_type>
      </vjc_PlugPanelContainer>
    </Defaults>
  </ChunkDesc>
  <ChunkDesc token="vjc_Component" name="VjControl Component Class Registration">
    <help>Registers a VjControl Extension Component class (usually found in contents.config of extension .jars)</help>
    <PropertyDesc token="Name" name="Name" type="String" num="1">
        <help>Unique name of an instance of this chunk type</help>
    </PropertyDesc>
  </ChunkDesc>
  <ChunkDesc token="vjc_PlugPanel" name="VjControl PlugPanel Instance">
    <help>Configuration information for a VjControl PlugPanel.</help>
    <PropertyDesc token="Name" name="Name" type="String" num="1">
        <help>Unique name of an instance of this chunk type</help>
    </PropertyDesc>
    <PropertyDesc token="ClassName" name="Class Name" type="String" num="1">
    </PropertyDesc>
    <PropertyDesc token="Dependencies" name="Dependencies" type="Chunk" num="variable">
    </PropertyDesc>
    <PropertyDesc token="ParentComp" name="Parent Component" type="Chunk" num="1">
        <help>Component to which to add this</help>
    </PropertyDesc>
    <PropertyDesc token="Category" name="Categories" type="String" num="variable">
    </PropertyDesc>
    <PropertyDesc token="Hints" name="UI Hints" type="String" num="variable">
    </PropertyDesc>
    <PropertyDesc token="user_level" name="Complexity of this interface:" type="Int" num="1">
        <enumeration name="Beginner" value="0"/>
        <enumeration name="Expert" value="10"/>
    </PropertyDesc>
    <PropertyDesc token="user_type" name="User tasks this interface is for:" type="Bool" num="5">
        <label name="Always Display"/>
        <label name="Config Editing"/>
        <label name="Dynamic Reconfiguration"/>
        <label name="Perf Analysis"/>
        <label name="Advanced Config"/>
    </PropertyDesc>
    <Defaults>
      <vjc_PlugPanel name="">
        <ClassName>"" </ClassName>
        <Dependencies></Dependencies>
        <ParentComp>"" </ParentComp>
        <Category></Category>
        <Hints></Hints>
        <user_level>"0" </user_level>
        <user_type>"True" "False" "False" "False" "False" </user_type>
      </vjc_PlugPanel>
    </Defaults>
  </ChunkDesc>
  <ChunkDesc token="vjc_GenericInstance" name="VjControl Component Instance">
    <PropertyDesc token="Name" name="Name" type="String" num="1">
        <help>Unique name of an instance of this chunk type</help>
    </PropertyDesc>
    <PropertyDesc token="ClassName" name="Class Name" type="String" num="1">
    </PropertyDesc>
    <PropertyDesc token="Dependencies" name="Dependencies" type="Chunk" num="variable">
    </PropertyDesc>
    <PropertyDesc token="ParentComp" name="Parent Component" type="Chunk" num="1">
        <help>Component to which to add this</help>
    </PropertyDesc>
    <PropertyDesc token="user_level" name="Complexity of this interface:" type="Int" num="1">
        <enumeration name="Beginner" value="0"/>
        <enumeration name="Expert" value="10"/>
    </PropertyDesc>
    <PropertyDesc token="user_type" name="User tasks this interface is for:" type="Bool" num="5">
        <label name="Always Display"/>
        <label name="Config Editing"/>
        <label name="Dynamic Reconfiguration"/>
        <label name="Perf Analysis"/>
        <label name="Advanced Config"/>
    </PropertyDesc>
    <Defaults>
      <vjc_GenericInstance name="">
        <ClassName>"" </ClassName>
        <Dependencies></Dependencies>
        <ParentComp>"" </ParentComp>
        <user_level>"0" </user_level>
        <user_type>"True" "False" "False" "False" "False" </user_type>
      </vjc_GenericInstance>
    </Defaults>
  </ChunkDesc>
  <ChunkDesc token="PerfData" name="Performance Data Display">
    <help>Information for displaying data from perfbuffers with a matching name.</help>
    <PropertyDesc token="Name" name="Name" type="String" num="1">
        <help>Unique name of an instance of this chunk type</help>
    </PropertyDesc>
    <PropertyDesc token="labels" name="Labels" type="String" num="variable">
        <help>Labels for Perf Buffer phases</help>
    </PropertyDesc>
  </ChunkDesc>
</ChunkDescDB>
