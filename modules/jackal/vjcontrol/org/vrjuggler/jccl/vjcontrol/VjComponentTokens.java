package org.vrjuggler.jccl.vjcontrol;


/**
 * This class exists as a container for tokens that are needed by the
 * VjComponent system.  It is needed only for that code.  The tokens defined
 * here match elements in the configuration chunks used by the VjComponent
 * system.
 */
public class VjComponentTokens
{
   /** Do not allow objects of this type to be created. */
   private VjComponentTokens ()
   {
   }

   public static final String CATEGORY      = "Category";
   public static final String CHUNK_FILES   = "chunkfiles";
   public static final String CLASS_NAME    = "ClassName";
   public static final String DEFAULT_PANEL = "default_panel";
   public static final String DESC_FILES    = "descfiles";
   public static final String DEPENDENCIES  = "Dependencies";
   public static final String FONT_NAME     = "fontname";
   public static final String FONT_SIZE     = "fontsize";
   public static final String HOST          = "host";
   public static final String LOOKNFEEL     = "looknfeel";
   public static final String PARENT_COMP   = "ParentComp";
   public static final String PORT          = "port";
   public static final String USED_FOR      = "UsedFor";
   public static final String USER_LEVEL    = "user_level";
   public static final String USER_TYPE     = "user_type";
   public static final String WINDOW_SIZE   = "windowsize";
}