/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


// Minimal VPR includes
#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Util/FileUtils.h>

// Minimal JCCL includes
#include <jccl/Config/ParseUtil.h>
#include <jccl/Config/ElementFactory.h>
#include <jccl/RTRC/ConfigManager.h>

// Interfaces to devices
#include <gadget/InputManager.h>
#include <gadget/Type/PositionInterface.h>

#include <gmtl/Matrix.h>
#include <gmtl/Output.h>

#include <string>

using namespace gmtl;

// Set some default configuration. Based on the initialiser for vrj::Kernel.
// Last taken from 2.0alpha4

void configuration()
{
   // Load in the configuration definitions
   std::string def_path;
   if ( ! vpr::System::getenv("JCCL_DEFINITION_PATH", def_path).success() )
   {
      def_path = "${VJ_BASE_DIR}/share/vrjuggler/data/definitions";
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "JCCL_DEFINITION_PATH environment variable not set.\n"
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "Defaulting to " << def_path << std::endl << vprDEBUG_FLUSH;
   }
   jccl::ElementFactory::instance()->loadDefs(def_path);
   
   std::string cfg_path;

   // Set the configuration file directory
   if ( ! vpr::System::getenv("JCCL_CFG_PATH", cfg_path).success() )
   {
      if ( vpr::System::getenv("VJ_CFG_PATH", cfg_path).success() )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "JCCL_CFG_PATH environment variable not set.\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Using VJ_CFG_PATH instead.\n" << vprDEBUG_FLUSH;
      }
      // Neither $JCCL_CFG_PATH nor $VJ_CFG_PATH is set, so use what basically
      // amounts to a hard-coded default.
      else
      {
         cfg_path = "${VJ_BASE_DIR}/share/vrjuggler/data/configFiles";
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Neither JCCL_CFG_PATH nor VJ_CFG_PATH is set.\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Defaulting to " << cfg_path << std::endl << vprDEBUG_FLUSH;
         cfg_path = vpr::replaceEnvVars(cfg_path);
      }
      jccl::ParseUtil::setCfgSearchPath(cfg_path);
   }
}

int main(int argc, char* argv[])
{

   // Set some default configuration variables
   configuration();

   // Load in the configuration file
   for( int i = 1; i < argc; ++i )
   {
      jccl::Configuration cfg;
      std::cout << "Loading config file: " << argv[i] << std::endl;
      cfg.load(argv[i]);
      jccl::ConfigManager::instance()->addConfigurationAdditions(&cfg);
   }
  
   // Create the input manager
   gadget::InputManager* input_manager = gadget::InputManager::instance();

   // Associate it with the config files
   jccl::ConfigManager::instance()->addConfigElementHandler(input_manager);

   // Do the configuration
   while(jccl::ConfigManager::instance()->attemptReconfiguration())
   {
      ;
   }

   // Create the input devices
   gadget::PositionInterface wand;    
   gadget::PositionInterface head;    

   wand.init("VJWand");
   head.init("VJHead");

   Matrix44f wand_matrix;
   Matrix44f head_matrix;

   while(1)
   {
      vpr::System::sleep(1);
      std::cout << "Updating All Data .. " << std::endl;
      input_manager->updateAllData();

      wand_matrix = wand->getData();
      std::cout << "Wand pos: \n" << wand_matrix << std::endl;

      head_matrix = head->getData();
      std::cout << "Head pos: \n" << head_matrix << std::endl;

   }

   return 0;
}
