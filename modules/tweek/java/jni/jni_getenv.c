/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <stdlib.h>
#include <string.h>
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

#if defined(_MSC_VER) && _MSC_VER >= 1400
      char* value;
      size_t value_size = 256 * sizeof(char);
      size_t req_size;
      errno_t status;

      value = (char*) malloc(value_size);
      memset(value, '\0', value_size);
      status = getenv_s(&req_size, value, value_size, name);

      // If the memory allocated for value was insufficient, we need
      // to call getenv_s() again with a new buffer that has the
      // necessary size.
      if ( req_size > value_size )
      {
         value_size = req_size;
         value = (char*) realloc(value, value_size);
         status = getenv_s(&req_size, value, value_size, name);
      }

      if ( 0 == status )
      {
         result = (*env)->NewStringUTF(env, value);
      }

      free(value);
#else
      const char* value = getenv(name);

      if ( NULL != value )
      {
         result = (*env)->NewStringUTF(env, value);
      }
#endif

      (*env)->ReleaseStringUTFChars(env, jname, name);
   }

   return result;
}
