<?xml version="1.0" encoding="UTF-8"?>
<ChunkDescDB>
  <ChunkDesc token="pf_basic_nav_app" name="Basic nav app">
    <help>A basic performer navigation application</help>
    <PropertyDesc token="Name" name="Name" type="String" num="1">
        <help>Unique name of an instance of this chunk type</help>
    </PropertyDesc>
    <PropertyDesc token="start_location" name="Start Location" type="Float" num="3">
    </PropertyDesc>
    <PropertyDesc token="file_path" name="File Paths" type="String" num="1">
        <help>Paths to search for files</help>
    </PropertyDesc>
    <PropertyDesc token="Sound" name="Sounds" type="EmbeddedChunk" num="variable">
        <help>specify a sound here.  usually used </help>
        <enumeration name="Sound"/>
    </PropertyDesc>
    <PropertyDesc token="Model" name="Models" type="EmbeddedChunk" num="variable">
        <enumeration name="Model"/>
    </PropertyDesc>
    <PropertyDesc token="bounding_size" name="Bounding Sphere size" type="Float" num="1">
    </PropertyDesc>
    <PropertyDesc token="icon_view_offset" name="Icon model offset" type="Float" num="3">
        <help>Amount to tranlate the application scene graph for displaying the icon (in pf coords, model coords)</help>
        <label name="X"/>
        <label name="y"/>
        <label name="z"/>
    </PropertyDesc>
    <PropertyDesc token="enable_nav" name="enable_nav" type="Bool" num="1">
        <help>Allow navigation or not... (false for animation)</help>
    </PropertyDesc>
    <PropertyDesc token="animation_play" name="animation_play" type="Bool" num="1">
        <help>Play animation (true) or stop (false)</help>
    </PropertyDesc>
    <PropertyDesc token="animation_loops" name="animation_loops" type="Float" num="1">
        <help>Number of times to loop animation (-1 for infinite)</help>
    </PropertyDesc>
    <PropertyDesc token="animation_filename" name="animation_filename" type="String" num="1">
        <help>name of animation file to load</help>
    </PropertyDesc>
  </ChunkDesc>
  <ChunkDesc token="Model" name="Model">
    <PropertyDesc token="Name" name="Name" type="String" num="1">
        <help>Unique name of an instance of this chunk type</help>
    </PropertyDesc>
    <PropertyDesc token="filename" name="Filename" type="String" num="1">
    </PropertyDesc>
    <PropertyDesc token="Scale" name="Scale" type="Float" num="1">
        <help>Amount to scale the model</help>
    </PropertyDesc>
    <PropertyDesc token="x" name="x" type="Float" num="1">
        <help>starting position of the model</help>
    </PropertyDesc>
    <PropertyDesc token="y" name="y" type="Float" num="1">
        <help>starting position of the model</help>
    </PropertyDesc>
    <PropertyDesc token="z" name="z" type="Float" num="1">
        <help>starting position of the model</help>
    </PropertyDesc>
    <PropertyDesc token="rotx" name="rot x" type="Float" num="1">
        <help>Model starting rotation</help>
    </PropertyDesc>
    <PropertyDesc token="roty" name="rot y" type="Float" num="1">
    </PropertyDesc>
    <PropertyDesc token="rotz" name="rot z" type="Float" num="1">
    </PropertyDesc>
    <PropertyDesc token="collidable" name="collidable" type="Bool" num="1">
        <help>Use collision detection on this model or not.</help>
    </PropertyDesc>
  </ChunkDesc>
  <ChunkDesc token="Sound" name="Sound">
    <PropertyDesc token="Name" name="Name" type="String" num="1">
        <help>Unique name of an instance of this chunk type</help>
    </PropertyDesc>
    <PropertyDesc token="soundAlias" name="Sound Alias" type="String" num="1">
    </PropertyDesc>
    <PropertyDesc token="positional" name="Positional" type="Bool" num="1">
        <help>Is it position based sound (vol changes with movement)? Or is it just ambient.</help>
    </PropertyDesc>
    <PropertyDesc token="x" name="x" type="Float" num="1">
        <help>initial position of the sound in juggler coordinates</help>
    </PropertyDesc>
    <PropertyDesc token="y" name="y" type="Float" num="1">
        <help>initial position of the sound in juggler coordinates</help>
    </PropertyDesc>
    <PropertyDesc token="z" name="z" type="Float" num="1">
        <help>initial position of sound...</help>
    </PropertyDesc>
  </ChunkDesc>
</ChunkDescDB>
