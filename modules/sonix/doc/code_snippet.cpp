   
   // change the API to OpenAL
   // This doesn't affect any loaded sounds 
   // i.e. they will still be there when you manipulate them next
   sonix::instance()->changeAPI( "OpenAL );
   
   // configure a sound 
   snx::SoundInfo si;
   si.filename = "bigbang.wav";
   si.datasource = snx::SoundInfo::FILESYSTEM;   
   sonix::instance()->configure( "bigbang", si );
   
   
   // trigger (play) the sound
   sonix::instance()->trigger( "bigbang" );
   
   // pause the sound
   sonix::instance()->pause( "bigbang" );

   // stop the sound
   sonix::instance()->stop( "bigbang" );

   // set the position of the sound
   // relative to 0,0,0
   sonix::instance()->setPosition( "bigbang", -60, 0, 0 );
