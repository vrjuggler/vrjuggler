/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <stdlib.h>
#include <jni.h>


/*
 * This is based on example code that can be found here:
 *
 *    http://www.rgagnon.com/javadetails/java-0460.html
 */
JNIEXPORT jstring JNICALL
Java_org_vrjuggler_tweek_EnvironmentServiceImpl_tweekJniGetenv(JNIEnv* env,
                                                               jclass c,
                                                               jstring jname)
{
   jstring result = NULL;

   if ( NULL != jname )
   {
      const char* name = (*env)->GetStringUTFChars(env, jname, NULL);
      const char* value = getenv(name);
      (*env)->ReleaseStringUTFChars(env, jname, name);

      if ( NULL != value )
      {
         result = (*env)->NewStringUTF(env, value);
      }
   }

   return result;
}
