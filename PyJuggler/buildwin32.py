#python

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

import glob
import os
import re
import shutil
import sys
import time
import traceback

python_ver_re = re.compile(r'(\d+)\.(\d+)')
python_pkg_dir = ''

def setVars():
   global python_pkg_dir

   def processInput(optionDict, envVar, inputDesc, required = False):
      default_value = optionDict[envVar]
      print "    Enter %s [%s]: " % (inputDesc, default_value),
      input_str = sys.stdin.readline().strip(" \n")

      if input_str == '':
         if required and (default_value is None or default_value == ''):
            print "ERROR: %s value required" % inputDesc
            os.exit(1)
         else:
            value_str = default_value
      else:
         value_str = input_str

      optionDict[envVar] = value_str
      os.environ[envVar] = value_str

      return value_str

   options = {
      # Default values for required settings.
      'VJ_BASE_DIR'    : os.getenv('VJ_BASE_DIR', ''),
      'VJ_DEPS_DIR'    : os.getenv('VJ_DEPS_DIR', ''),
      'BOOST_VERSION'  : os.getenv('BOOST_VERSION', '1_31'),
      'BOOST_TOOL'     : os.getenv('BOOST_TOOL', 'vc71'),
      'PYTHON_ROOT'    : os.getenv('PYTHON_ROOT', ''),
      'prefix'         : r'C:\PyJuggler',

      # Default values for optional settings.
      'OSGHOME'        : os.getenv('OSGHOME', '')
   }

   # If there are cached options, read them in.
   cache_file = os.path.join(pyj_dir, 'options.cache')
   if os.path.exists(cache_file):
      execfile(cache_file)

   print "+++ Required Settings"
   processInput(options, 'prefix', 'installation prefix')
   processInput(options, 'VJ_BASE_DIR', 'VR Juggler installation directory')
   processInput(options, 'VJ_DEPS_DIR',
                'VR Juggler dependency installation directory')
   processInput(options, 'BOOST_VERSION', 'Boost C++ version')
   processInput(options, 'BOOST_TOOL',
                'the Boost.Build toolset used to compile Boost C++')
   processInput(options, 'PYTHON_ROOT', 'Python installation directory')

   print "\n+++ Optional Settings"
   processInput(options, 'OSGHOME', 'Open Scene Graph installation directory',
                False)

   cache_file = open(cache_file, 'w')
   for k, v in options.iteritems():
      output = "options['%s'] = r'%s'\n" % (k, v)
      cache_file.write(output)
   cache_file.close()

   return options

def doInstall(prefix):
   makeTree(prefix)
   installDist(prefix)
   installMods(prefix)
   installExamples(prefix)

def mkinstalldirs(dir):
   if not os.path.exists(dir):
      (head, tail) = os.path.split(dir)
      mkinstalldirs(head)
      os.mkdir(dir)

def makeTree(prefix):
#   mkinstalldirs(os.path.join(prefix, 'include'))
   mkinstalldirs(os.path.join(prefix, 'lib'))
   mkinstalldirs(os.path.join(prefix, python_pkg_dir))
   mkinstalldirs(os.path.join(prefix, 'share', 'pyjuggler'))

def installDir(startDir, destDir, allowedExts = None, disallowedExts = None,
               disallowedFiles = None):
   cwd = os.getcwd()
   mkinstalldirs(destDir)

   os.chdir(startDir)
   contents = os.listdir(startDir)

   if disallowedExts is None:
      disallowedExts = []

   if disallowedFiles is None:
      disallowedFiles = []

   # Add some extensions that should always be disallowed.  This relieves the
   # caller from having to add these repeatedly.
   disallowedExts.append('.ilk')
   disallowedExts.append('.ncb')
   disallowedExts.append('.pdb')
   disallowedExts.append('.suo')

   skip_dirs = ['CVS', 'autom4te.cache']
   for f in contents:
      if os.path.isdir(f):
         if f in skip_dirs:
            continue

         start_dir = os.path.join(startDir, f)
         dest_dir  = os.path.join(destDir, f)
         installDir(start_dir, dest_dir, allowedExts, disallowedExts)
      else:
         (root, f_ext) = os.path.splitext(f)
         if allowedExts is None:
            if f_ext not in disallowedExts:
               shutil.copy2(f, destDir)
         elif f_ext in allowedExts:
            (head, tail) = os.path.split(f)
            if f not in disallowedFiles:
               shutil.copy2(f, destDir)

   os.chdir(cwd)

def installDist(prefix):
   print "Installing distribution files ..."
   destdir = os.path.join(prefix, python_pkg_dir, 'PyJuggler')
   srcdir  = os.path.join(pyj_dir, 'dist')

   files = glob.glob(os.path.join(srcdir, '*.py'))

   mkinstalldirs(destdir)
   for f in files:
      shutil.copy2(f, destdir)

def installMods(prefix):
   print "Installing Python extension modules ..."

   destdir = os.path.join(prefix, python_pkg_dir, 'PyJuggler')
   srcdir  = os.path.join(pyj_dir, 'vc7', 'lib')

   files = glob.glob(os.path.join(srcdir, '*.pyd'))

   mkinstalldirs(destdir)
   for f in files:
      shutil.copy2(f, destdir)

def installExamples(prefix):
   print "Installing example code ..."

   destdir = os.path.join(prefix, 'share', 'pyjuggler', 'examples')
   srcdir  = os.path.join(pyj_dir, 'examples')
   installDir(srcdir, destdir, None, ['.in'])

def main():
   options = setVars()

   devenv_cmd = None
   for p in str.split(os.getenv('PATH', ''), os.pathsep):
      if os.path.exists(os.path.join(p, 'devenv.exe')):
         devenv_cmd = os.path.join(p, 'devenv.exe')
         break

   if devenv_cmd is None:
      # The environment variable %VSINSTALLDIR% is set by vsvars32.bat.
      print "WARNING: Falling back on the use of %VSINSTALLDIR%"
      devenv_cmd = r'%s' % os.path.join(os.getenv('VSINSTALLDIR', ''),
                                        'devenv.exe')

   solution_file = r'%s' % os.path.join(pyj_dir, 'vc7', 'PyJuggler.sln')

   try:
      status = os.spawnl(os.P_WAIT, devenv_cmd, 'devenv', solution_file)

      if status == 0:
         print "Proceed with PyJuggler installation [y]: ",
         proceed = sys.stdin.readline().strip(" \n")
         if proceed == '' or proceed.lower().startswith('y'):
            doInstall(options['prefix'])
   except OSError, osEx:
      print "Could not execute %s: %s" % (devenv_cmd, osEx)
      sys.exit(3)

   sys.exit(0)

pyj_dir = os.path.dirname(os.path.abspath(sys.argv[0]))
if __name__ == '__main__':
   try:
      main()
   except SystemExit, exitEx:
      if exitEx.code == 0:
         status = 'successful completion'
      elif exitEx.code == 1:
         status = 'no Visual Studio installation found'
      elif exitEx.code == 2:
         status = 'could not read data file required for compiling'
      elif exitEx.code == 3:
         status = 'could not start Visual Studio'
      elif exitEx.code == 4:
         status = 'invalid input'
      else:
         status = 'error encountered'

      print "Exiting with status %d (%s)" % (exitEx.code, status)
      print "Press <ENTER> to quit ..."
      sys.stdin.readline()

      # Exit for real without throwing another SystemExit exception.
      os._exit(exitEx.code)
   except:
      info = sys.exc_info()
      traceback.print_exception(info[0], info[1], info[2])
      print "An exception was caught.  Press <ENTER> to quit ..."
      sys.stdin.readline()
