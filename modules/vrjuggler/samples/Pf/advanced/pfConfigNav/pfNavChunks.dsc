<?xml version="1.0" encoding="UTF-8"?>
<?org-vrjuggler-jccl-settings desc.db.version="2.1"?>
<ChunkDescDB>
  <ChunkDesc token="pf_basic_nav_app" name="Basic nav app">
    <category>Unassigned</category>
    <help>A basic performer navigation application</help>
    <PropertyDesc token="start_location" name="Start Location" type="float" variable="0">
      <help />
      <item label="Start Location (x)" defaultvalue="0.0" />
      <item label="Start Location (y)" defaultvalue="0.0" />
      <item label="Start Location (z)" defaultvalue="0.0" />
    </PropertyDesc>
    <PropertyDesc token="file_path" name="File Paths" type="string" variable="0">
      <help>Paths to search for files</help>
      <item label="File Paths" defaultvalue="" />
    </PropertyDesc>
    <PropertyDesc token="Sound" name="Sounds" type="embeddedchunk" variable="1">
      <help>specify a sound here.  usually used </help>
      <item label="Sounds" />
      <enumeration name="Sound" />
    </PropertyDesc>
    <PropertyDesc token="Model" name="Models" type="embeddedchunk" variable="1">
      <help />
      <item label="Models" />
      <enumeration name="Model" />
    </PropertyDesc>
    <PropertyDesc token="bounding_size" name="Bounding sphere size" type="float" variable="0">
      <help />
      <item label="Bounding sphere size" defaultvalue="0.0" />
    </PropertyDesc>
    <PropertyDesc token="icon_view_offset" name="Icon model offset" type="float" variable="0">
      <help>Amount to tranlate the application scene graph for displaying the icon (in pf coords, model coords)</help>
      <item label="X Translation" defaultvalue="0.0" />
      <item label="Y Translation" defaultvalue="0.0" />
      <item label="Z Translation" defaultvalue="0.0" />
    </PropertyDesc>
    <PropertyDesc token="enable_nav" name="enable_nav" type="bool" variable="0">
      <help>Allow navigation or not... (false for animation)</help>
      <item label="Enable navigation?" defaultvalue="1" />
    </PropertyDesc>
    <PropertyDesc token="animation_play" name="animation_play" type="bool" variable="0">
      <help>Play animation (true) or stop (false)</help>
      <item label="Play animation?" defaultvalue="0" />
    </PropertyDesc>
    <PropertyDesc token="animation_loops" name="animation_loops" type="int" variable="0">
      <help>Number of times to loop animation (-1 for infinite)</help>
      <item label="Number of loops" defaultvalue="1" />
    </PropertyDesc>
    <PropertyDesc token="animation_filename" name="animation_filename" type="string" variable="0">
      <help>name of animation file to load</help>
      <item label="Animation filename" defaultvalue="" />
    </PropertyDesc>
  </ChunkDesc>
  <ChunkDesc token="Model" name="Model">
    <category>Unassigned</category>
    <help />
    <PropertyDesc token="filename" name="Filename" type="string" variable="0">
      <help />
      <item label="Filename" defaultvalue="" />
    </PropertyDesc>
    <PropertyDesc token="Scale" name="Scale" type="float" variable="0">
      <help>Amount to scale the model</help>
      <item label="Scale" defaultvalue="0.0" />
    </PropertyDesc>
    <PropertyDesc token="x" name="x" type="float" variable="0">
      <help>starting position of the model</help>
      <item label="X Position" defaultvalue="0.0" />
    </PropertyDesc>
    <PropertyDesc token="y" name="y" type="float" variable="0">
      <help>starting position of the model</help>
      <item label="Y Position" defaultvalue="0.0" />
    </PropertyDesc>
    <PropertyDesc token="z" name="z" type="float" variable="0">
      <help>starting position of the model</help>
      <item label="Z Position" defaultvalue="0.0" />
    </PropertyDesc>
    <PropertyDesc token="rotx" name="rot x" type="float" variable="0">
      <help>Model starting rotation</help>
      <item label="Rotation on X-Axis" defaultvalue="0.0" />
    </PropertyDesc>
    <PropertyDesc token="roty" name="rot y" type="float" variable="0">
      <help />
      <item label="Rotation on Y-Axis" defaultvalue="0.0" />
    </PropertyDesc>
    <PropertyDesc token="rotz" name="rot z" type="float" variable="0">
      <help />
      <item label="Rotation on Z-Axis" defaultvalue="0.0" />
    </PropertyDesc>
    <PropertyDesc token="collidable" name="collidable" type="bool" variable="0">
      <help>Use collision detection on this model or not.</help>
      <item label="Is collidable?" defaultvalue="1" />
    </PropertyDesc>
  </ChunkDesc>
  <ChunkDesc token="Sound" name="Sound">
    <category>Unassigned</category>
    <help />
    <PropertyDesc token="soundAlias" name="Sound Alias" type="string" variable="0">
      <help />
      <item label="Sound Alias" defaultvalue="" />
    </PropertyDesc>
    <PropertyDesc token="positional" name="Positional" type="bool" variable="0">
      <help>Is it position based sound (vol changes with movement)? Or is it just ambient?</help>
      <item label="Is positional?" defaultvalue="0" />
    </PropertyDesc>
    <PropertyDesc token="x" name="x" type="float" variable="0">
      <help>initial position of the sound in juggler coordinates</help>
      <item label="X Position" defaultvalue="0.0" />
    </PropertyDesc>
    <PropertyDesc token="y" name="y" type="float" variable="0">
      <help>initial position of the sound in juggler coordinates</help>
      <item label="Y Position" defaultvalue="0.0" />
    </PropertyDesc>
    <PropertyDesc token="z" name="z" type="float" variable="0">
      <help>initial position of sound...</help>
      <item label="Z Position" defaultvalue="0.0" />
    </PropertyDesc>
  </ChunkDesc>
</ChunkDescDB>
