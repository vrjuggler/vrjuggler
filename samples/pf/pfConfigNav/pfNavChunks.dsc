chunk pf_basic_nav_app "Basic nav app" "A basic performer navigation application"
  Name String 1 "Name" "Unique name of an instance of this chunk type"
  start_location Float 3 "Start Location" ""
  file_path String 1 "File Paths" "Paths to search for files"
  Sounds EmbeddedChunk -1 "Sounds" vj_enumeration { "pfNavSound" } "specify a sound here.  usually used "
  models EmbeddedChunk -1 "Models" vj_enumeration { "pf_model_info" } ""
  end
chunk Model "Model" ""
  Name String 1 "Name" "Unique name of an instance of this chunk type"
  filename String 1 "Filename" ""
  Scale Float 1 "Scale" "Amount to scale the model"
  x Float 1 "x" "starting position of the model"
  y Float 1 "y" "starting position of the model"
  z Float 1 "z" "starting position of the model"
  rotx Float 1 "rot x" ""
  roty Float 1 "rot y" ""
  rotz Float 1 "rot z" ""
  end
chunk Sound "Sound" ""
  Name String 1 "Name" "Unique name of an instance of this chunk type"
  soundAlias Int 1 "Sound Alias" ""
  positional Bool 1 "Positional" "Is it position based sound (vol changes with movement)? Or is it just ambient."
  x Float 1 "x" "initial position of the sound in juggler coordinates"
  y Float 1 "y" "initial position of the sound in juggler coordinates"
  z Float 1 "z" "initial position of sound..."
  end
End
