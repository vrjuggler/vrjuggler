#python

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2003 by Iowa State University
#
# Original Authors:
#   Allen Bierbaum, Christopher Just,
#   Patrick Hartling, Kevin Meinert,
#   Carolina Cruz-Neira, Albert Baker
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

ErrorImportingTkinter = False
try:
   import Tkinter
   import tkMessageBox
   import tkFileDialog
   import threading
except ImportError, ex:
   ErrorImportingTkinter = True
   print ex

def guessBoostToolset(reattempt = False):
   (cl_stdin, cl_stdout, cl_stderr) = os.popen3('cl')
   cl_version_line = cl_stderr.readline()

   cl_ver_match = re.compile(r'Compiler Version ((\d+)\.(\d+)\.(\d+)) for')
   ver_string_match = cl_ver_match.search(cl_version_line)

   if ver_string_match is not None:
      cl_major = int(ver_string_match.group(2))
      cl_minor = int(ver_string_match.group(3))

      if cl_major == 13 and cl_minor < 10:
         print "It appears that we will be using Visual Studio .NET 2002"
         boost_tool_guess = 'vc7'
      else:
         print "It appears that we will be using Visual Studio .NET 2003"
         boost_tool_guess = 'vc71'
   else:
      boost_tool_guess = ''

   in_status  = cl_stdin.close()
   out_status = cl_stdout.close()
   err_status = cl_stderr.close()

   # If there was an error closing any of the streams returned by
   # os.popen3(), then the command was not opened correctly.  That means
   # that CL.EXE is not in the user's path.
   if in_status is not None or out_status is not None or err_status is not None:
      # If this is not a reattempt to guess the Visual Studio version, then
      # be nice and extend the user's path to include their Visual Studio
      # installation.
      if not reattempt:
         # Common installation directories for Visual Studio 7.x.
         vs_dirs = [r'C:\Program Files\Microsoft Visual Studio .NET',
                    r'C:\Program Files\Microsoft Visual Studio .NET 2003']

         for d in vs_dirs:
            if os.path.exists(d):
               print "NOTE: Using Visual Studio installation in"
               print "      " + d
               vs_path = [os.path.join(d, r'Common7\IDE'),
                          os.path.join(d, r'VC7\BIN'),
                          os.path.join(d, r'Common7\Tools'),
                          os.path.join(d, r'Common7\Tools\bin\prerelease'),
                          os.path.join(d, r'Common7\Tools\bin')]
               path_add = ';'.join(vs_path)
               os.environ['PATH'] = path_add + os.pathsep + os.getenv('PATH', '')

               # Try again to guess the Visual Studio version.
               return guessBoostToolset(True)

         # If execution reaches this point, our attempts to guess the
         # location of a Visual Studio 7.x installation failed.
         noVisualStudioError()

      # If this is a reattempt to guess the Visual Studio version, then
      # something is wrong with the user's Visual Studio installation.
      else:
         noVisualStudioError()

   return boost_tool_guess

def noVisualStudioError():
   print "ERROR: Visual Studio commands are not in your path!"
   print "Run vsvars32.bat in this shell or update the %PATH% environment"
   print "variable on your system."
   sys.exit(1)

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

def setDefaultVars():
   boost_tool_fallback = guessBoostToolset()

   options = {
      # Default values for required settings.
      'BOOST_ROOT'     : os.getenv('BOOST_ROOT', ''),
      'BOOST_VERSION'  : os.getenv('BOOST_VERSION', '1_31'),
      'BOOST_INCLUDES' : os.getenv('BOOST_INCLUDES', ''),
      'BOOST_TOOL'     : os.getenv('BOOST_TOOL', boost_tool_fallback),
      'NSPR_ROOT'      : os.getenv('NSPR_ROOT', ''),
      'CPPDOM_ROOT'    : os.getenv('CPPDOM_ROOT', ''),
      'GMTL_ROOT'      : os.getenv('GMTL_ROOT', ''),

      # Default values for optional settings.
      'JAVA_HOME'       : os.getenv('JAVA_HOME', r'C:\java'),
      'JAVA3D_HOME'     : os.getenv('JAVA3D_HOME', os.getenv('JAVA_HOME', '')),
      'OMNIORB_ROOT'    : os.getenv('OMNIORB_ROOT', ''),
      'PFROOT'          : os.getenv('PFROOT',
                                    r'C:\Program Files\Silicon Graphics\OpenGL Performer'),
      'VRPN_ROOT'       : os.getenv('VRPN_ROOT', ''),
      'AUDIERE_ROOT'    : os.getenv('AUDIERE_ROOT', ''),
      'OPENAL_ROOT'     : os.getenv('OPENAL_ROOT', ''),
      'TRACKD_API_ROOT' : os.getenv('TRACKD_API_ROOT', ''),
      'prefix'          : r'C:\vrjuggler',
      'deps-prefix'     : r'C:\vrjuggler-deps'
   }

   # If there are cached options, read them in.
   cache_file = os.path.join(juggler_dir, 'options.cache')
   if os.path.exists(cache_file):
      execfile(cache_file)
   return options

def setVars():
   cache_file = os.path.join(juggler_dir, 'options.cache')
   options = setDefaultVars()

   print "+++ Required Settings"
   boost_ver = processInput(options, 'prefix', 'installation prefix')
   boost_ver = processInput(options, 'BOOST_VERSION', 'Boost C++ version')
   boost_dir = processInput(options, 'BOOST_ROOT',
                            'Boost C++ installation directory')

   if options['BOOST_INCLUDES'] == '':
      options['BOOST_INCLUDES'] = boost_dir + r'\include\boost-' + boost_ver

   processInput(options, 'BOOST_INCLUDES',
                'directory containing the Boost C++ header tree')

   processInput(options, 'BOOST_TOOL',
                'the Boost.Build toolset used to compile Boost C++')

   processInput(options, 'NSPR_ROOT', 'NSPR installation directory')
   processInput(options, 'CPPDOM_ROOT', 'CppDOM installation directory')
   processInput(options, 'GMTL_ROOT', 'GMTL installation directory')

   print "+++ Optional Settings"
   processInput(options, 'deps-prefix', 'dependency installation prefix')
   processInput(options, 'JAVA_HOME', 'Java installation directory', False)

   # If the %JAVA_HOME% setting is a valid directory, add its bin subdirectory
   # to the path.
   if os.environ['JAVA_HOME'] != '' and os.path.exists(os.environ['JAVA_HOME']):
      jdk_path = os.path.join(os.environ['JAVA_HOME'], 'bin')
      os.environ['PATH'] = jdk_path + os.pathsep + os.environ['PATH']
      os.environ['JACORB_PATH'] = os.path.join(juggler_dir, r'external\JacORB')

   processInput(options, 'JAVA3D_HOME', 'Java3D installation directory', False)
   processInput(options, 'OMNIORB_ROOT', 'omniORB installation directory',
                False)

   if os.environ['OMNIORB_ROOT'] != '' and os.path.exists(os.environ['OMNIORB_ROOT']):
      omni_bin = os.path.join(os.environ['OMNIORB_ROOT'], 'bin')

      if os.path.exists(os.path.join(omni_bin, 'omniidl.exe')):
         os.environ['OMNIORB_BIN'] = omni_bin
      else:
         os.environ['OMNIORB_BIN'] = os.path.join(omni_bin, 'x86_win32')

      # Extend the path to include omniORB's bin directory.
      os.environ['PATH'] = os.environ['OMNIORB_BIN'] + os.pathsep + os.environ['PATH']

      omni_lib = os.path.join(os.environ['OMNIORB_ROOT'], 'lib')

      if os.getenv('PYTHONPATH', '') != '':
         os.environ['PYTHONPATH'] = os.path.join(omni_lib, 'python') + os.pathsep + os.environ['PYTHONPATH']
      else:
         os.environ['PYTHONPATH'] = os.path.join(omni_lib, 'python')

      if os.path.exists(os.path.join(omni_lib, 'omnithread.lib')):
         os.environ['OMNIORB_LIB'] = omni_lib
      else:
         os.environ['OMNIORB_LIB'] = os.path.join(omni_lib, 'x86_win32')

      omni_glob = os.path.join(os.environ['OMNIORB_LIB'], 'omniORB*_rt.lib')
      libs = glob.glob(omni_glob)
      omni_ver_re = re.compile(r'omniORB(\d\d\d)_rt.lib')

      for l in libs:
         match = omni_ver_re.search(l)
         if match is not None:
            os.environ['OMNIORB_VERSION'] = match.group(1)
            break

   processInput(options, 'PFROOT', 'OpenGL Performer installation directory',
                False)
   processInput(options, 'VRPN_ROOT', 'VRPN installation directory', False)
   processInput(options, 'AUDIERE_ROOT', 'Audiere installation directory',
                False)
   processInput(options, 'OPENAL_ROOT', 'OpenAL SDK installation directory',
                False)
   processInput(options, 'TRACKD_API_ROOT', 'TrackdAPI installation directory',
                False)

   cache_file = open(cache_file, 'w')
   for k, v in options.iteritems():
      output = "options['%s'] = r'%s'\n" % (k, v)
      cache_file.write(output)
   cache_file.close()

   return options

def generateVersionHeaders():
   class JugglerModule:
      def __init__(self, srcDir, header, projDir, templateFile = None):
         self.source_dir     = os.path.join(juggler_dir, srcDir)
         self.header         = os.path.join(juggler_dir, 'vc7', projDir,
                                            header)
         self.version_params = os.path.join(self.source_dir, 'Makefile.inc.in')
         self.version_file   = os.path.join(self.source_dir, 'VERSION')

         if templateFile is not None:
            self.header_template = templateFile
         else:
            self.header_template = os.path.join(self.source_dir, header + '.in')

      def generateParamHeader(self):
         if os.path.exists(self.header):
            mtime = os.path.getmtime
            # This test to determine if the module's param header needs to be
            # regenerated is equivalent to that used by the UNIX build system.
            if mtime(self.version_file) > mtime(self.header) or \
               mtime(self.header_template) > mtime(self.header):
               self.__genHeader()
         else:
            self.__genHeader()

      version_re    = re.compile(r'((\d+)\.(\d+)\.(\d+)-(\d+))\s')
      branch_re     = re.compile(r'BRANCH\s*=\s*(\w+)')
      canon_name_re = re.compile(r'CANON_NAME\s*=\s*(\S.+)')
      vernum_re     = re.compile(r'@VER_NUMBER@')
      verstr_re     = re.compile(r'@VER_STRING@')
      zero_strip_re = re.compile(r'^0*([^0]\d+)')

      def __genHeader(self):
         ver_file = open(self.version_file)
         cur_ver  = ver_file.readline()
         ver_file.close()
         ver_match = self.version_re.match(cur_ver)
         version = ver_match.group(1)
         major   = int(ver_match.group(2))
         minor   = int(ver_match.group(3))
         patch   = int(ver_match.group(4))

         # NOTE: This will not always be identical to the UNIX version because
         # Python does not have %e as a time formatting directive.
         date       = time.strftime('%b %d, %Y %H:%M:%S')
         canon_name = ''
         branch     = ''

         param_file = open(self.version_params, 'r')
         params     = param_file.readlines()
         param_file.close()

         # This is basically a poor man's grep.  Can this be done better?
         for line in params:
            match = self.branch_re.match(line)
            if match is not None:
               branch_name = match.group(1)
               continue

            match = self.canon_name_re.match(line)
            if match is not None:
               canon_name = match.group(1)
               continue

         version_number = '%03d%03d%03d' % (major, minor, patch)
         version_string = "\"v%s '%s' (NSPR) %s %s\"" % \
                             (version, canon_name, branch, date)

         # Strip leading zeroes from version_number.  Is there an easier way
         # to do this?
         version_number = self.zero_strip_re.match(version_number).group(1)

         try:
            input_file  = open(self.header_template, 'r')
            input_lines = input_file.readlines()
            input_file.close()

            for i in xrange(len(input_lines)):
               line = input_lines[i]
               if self.vernum_re.search(line):
                  input_lines[i] = self.vernum_re.sub(version_number, line)
               elif self.verstr_re.search(line):
                  input_lines[i] = self.verstr_re.sub(version_string, line)

            print "Generating updated", self.header
            param_header = open(self.header, 'w')
            param_header.writelines(input_lines)
            param_header.close()
         except IOError, ex:
            print "ERROR: Could not read from %s" % self.header_template
            print ex
            print "Cannot continue; exiting with error status."
            sys.exit(2)

   mods = []
   mods.append(JugglerModule(r'modules\vapor', r'vpr\vprParam.h', 'VPR'))
   mods.append(JugglerModule(r'modules\tweek', r'tweek\tweekParam.h',
                             'Tweek_CXX'))
   mods.append(JugglerModule(r'modules\jackal', r'jccl\jcclParam.h', 'JCCL',
                             os.path.join(juggler_dir,
                                          r'modules\jackal\common\jccl\jcclParam.h.in')))
   mods.append(JugglerModule(r'modules\sonix', r'snx\snxParam.h', 'Sonix'))
   mods.append(JugglerModule(r'modules\gadgeteer', r'gadget\gadgetParam.h',
                             'Gadgeteer'))
   mods.append(JugglerModule(r'modules\vrjuggler', r'vrj\vrjParam.h',
                             'VRJuggler'))

   for m in mods:
      m.generateParamHeader()

def generateAntBuildFiles():
   class AntTarget:
      def __init__(self, srcdir, moduleName, outputFile = 'build.xml'):
         self.srcdir      = os.path.join(juggler_dir, srcdir)
         self.module_name = os.path.join(juggler_dir, r'vc7', moduleName)
         self.output_file = os.path.join(self.module_name, outputFile)

      # This form of regular expressions appears to be necessary because
      # the sub() method does not handle backslashes in the replacement string
      # the way I would like.
      srcdir_re         = re.compile(r'^(.*)@srcdir@(.*)$')
      topdir_re         = re.compile(r'^(.*)@topdir@(.*)$')
      juggler_root_re   = re.compile(r'^(.*)@JUGGLERROOT_ABS@(.*)$')
      jdom_jar_re       = re.compile(r'^(.*)@JDOM_JAR@(.*)$')
      tweek_jars_re     = re.compile(r'^(.*)@TWEEK_JARS@(.*)$')
      tweek_ext_jars_re = re.compile(r'^(.*)@TWEEK_EXT_JARS@(.*)$')
      jccl_jars_re      = re.compile(r'^(.*)@JCCL_JARS@(.*)$')
      java_orb_jar_re   = re.compile(r'^(.*)@JAVA_ORB_JAR@(.*)$')
      java3d_jars_re    = re.compile(r'^(.*)@JAVA3D_JAR@(.*)$')

      jdom_jars = [
         os.path.join(juggler_dir, r'external\jdom\jaxen-core.jar'),
         os.path.join(juggler_dir, r'external\jdom\xalan.jar'),
         os.path.join(juggler_dir, r'external\jdom\jaxen-jdom.jar'),
         os.path.join(juggler_dir, r'external\jdom\xerces.jar'),
         os.path.join(juggler_dir, r'external\jdom\jdom.jar'),
         os.path.join(juggler_dir, r'external\jdom\xml-apis.jar'),
         os.path.join(juggler_dir, r'external\jdom\saxpath.jar')
      ]

      tweek_jars = [
         os.path.join(juggler_dir, r'vc7\Tweek_Java', 'Tweek.jar'),
         os.path.join(juggler_dir, r'vc7\Tweek_Java', 'TweekBeans.jar'),
         os.path.join(juggler_dir, r'vc7\Tweek_Java', 'TweekEvents.jar'),
         os.path.join(juggler_dir, r'vc7\Tweek_Java', 'TweekNet.jar'),
         os.path.join(juggler_dir, r'vc7\Tweek_Java', 'TweekBeanDelivery.jar'),
         os.path.join(juggler_dir, r'vc7\Tweek_Java', 'TweekServices.jar'),
         os.path.join(juggler_dir, r'vc7\Tweek_Java', 'Viewers.jar'),
         os.path.join(juggler_dir, r'vc7\Tweek_Java', 'kunststoff-mod.jar')
      ]

      tweek_ext_jars = [
         os.path.join(juggler_dir, r'vc7\Tweek_Java', 'ui.jar'),
         os.path.join(juggler_dir, r'vc7\Tweek_Java', 'wizard.jar'),
         os.path.join(juggler_dir, r'vc7\Tweek_Java', 'WizardBuilder.jar')
      ]

      jccl_jars = [
         os.path.join(juggler_dir, r'vc7\JCCL_Java', 'jccl_config.jar'),
         os.path.join(juggler_dir, r'vc7\JCCL_Java', 'jccl_editors.jar')
      ]

      jccl_rtrc_jars = [
         os.path.join(juggler_dir, r'vc7\JCCL_Java\RTRC_Plugin_Java',
                      'jccl_rtrc.jar')
      ]

      java3d_jars = [
         os.path.join(os.environ['JAVA3D_HOME'], r'jre\lib\ext\j3daudio.jar'),
         os.path.join(os.environ['JAVA3D_HOME'], r'jre\lib\ext\j3dcore.jar'),
         os.path.join(os.environ['JAVA3D_HOME'], r'jre\lib\ext\j3dutils.jar'),
         os.path.join(os.environ['JAVA3D_HOME'], r'jre\lib\ext\vecmath.jar')
      ]

      def generateBuildFile(self):
         input_file = open(os.path.join(self.srcdir, 'build.xml.in'), 'r')
         input = input_file.readlines()
         input_file.close()

         for i in xrange(len(input)):
            line = input[i]

            if self.srcdir_re.search(line):
               match = self.srcdir_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], self.srcdir,
                                        match.groups()[1])
            elif self.topdir_re.search(line):
               match = self.topdir_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], self.module_name,
                                        match.groups()[1])
            elif self.juggler_root_re.search(line):
               match = self.juggler_root_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], juggler_dir,
                                        match.groups()[1])
            elif self.java_orb_jar_re.search(line):
               match = self.java_orb_jar_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], "",
                                        match.groups()[1])
            elif self.jdom_jar_re.search(line):
               jars = os.pathsep.join(self.jdom_jars)
               match = self.jdom_jar_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], jars,
                                        match.groups()[1])
            elif self.tweek_jars_re.search(line):
               jars = os.pathsep.join(self.tweek_jars + self.jdom_jars)
               match = self.tweek_jars_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], jars,
                                        match.groups()[1])
            elif self.tweek_ext_jars_re.search(line):
               jars = os.pathsep.join(self.tweek_ext_jars)
               match = self.tweek_ext_jars_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], jars,
                                        match.groups()[1])
            elif self.jccl_jars_re.search(line):
               jars = os.pathsep.join(self.jccl_jars + self.jccl_rtrc_jars)
               match = self.jccl_jars_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], jars,
                                        match.groups()[1])
            elif self.java3d_jars_re.search(line):
               jars = os.pathsep.join(self.java3d_jars)
               match = self.java3d_jars_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], jars,
                                        match.groups()[1])

         build_file = open(self.output_file, 'w')
         build_file.writelines(input)
         build_file.close()

   mods = []
   mods.append(AntTarget(r'modules\tweek\java', 'Tweek_Java'))
   mods.append(AntTarget(r'modules\tweek\extensions\java', 'Tweek_Java',
                         'build-ext.xml'))
   mods.append(AntTarget(r'modules\jackal\config', 'JCCL_Java',
                         'build-config.xml'))
   mods.append(AntTarget(r'modules\jackal\editors', 'JCCL_Java',
                         'build-editors.xml'))
   mods.append(AntTarget(r'modules\jackal\plugins\corba_rtrc',
                         r'JCCL_Java\RTRC_Plugin_Java', 'build.xml'))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig', 'VRJConfig',
                         'build.xml'))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\intersense',
                         'VRJConfig', 'build-intersense.xml'))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\proxyeditor',
                         'VRJConfig', 'build-proxyeditor.xml'))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\surfacedisplayeditor',
                         'VRJConfig', 'build-surfacedisplayeditor.xml'))

   for m in mods:
      m.generateBuildFile()

def doInstall(prefix):
   makeTree(prefix)
   installExternal(prefix)
   installVPR(prefix)
   installTweek(prefix)
   installTweekJava(prefix)
   installJCCL(prefix)
   installJCCLJava(prefix)
   installJCCLPlugins(prefix)
   installJCCLPluginsJava(prefix)
   installSonix(prefix)
   installSonixPlugins(prefix)
   installGadgeteer(prefix)
   installGadgeteerDrivers(prefix)
   installGadgeteerPlugins(prefix)
   installVRJuggler(prefix)
   installVRJConfig(prefix)
   installMsvcRT(prefix)

def mkinstalldirs(dir):
#   print "Checking for", dir
   if not os.path.exists(dir):
      (head, tail) = os.path.split(dir)
      mkinstalldirs(head)
      os.mkdir(dir)

def makeTree(prefix):
   mkinstalldirs(os.path.join(prefix, 'bin'))
   mkinstalldirs(os.path.join(prefix, 'include'))
   mkinstalldirs(os.path.join(prefix, 'lib'))
   mkinstalldirs(os.path.join(prefix, 'share'))

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

def installLibs(srcRoot, destdir,
                buildTypes = ['ReleaseDLL', 'DebugDLL', 'Release', 'Debug'],
                extensions = ['.dll', '.lib']):
   for t in buildTypes:
      srcdir = os.path.join(srcRoot, t)
      if os.path.exists(srcdir):
         installDir(srcdir, destdir, extensions)

def installExternal(prefix):
   # Install Doozer (even though it probably won't be used).
   print "Installing Doozer ..."
   destdir = os.path.join(prefix, 'share', 'Doozer')
   srcdir  = os.path.join(juggler_dir, 'external', 'Doozer')
   installDir(srcdir, destdir, ['.mk'])

def installVPR(prefix):
   print "Installing VPR headers and libraries ..."

   destdir = os.path.join(prefix, 'include', 'vpr')
   srcdir  = os.path.join(juggler_dir, 'modules', 'vapor', 'vpr')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(juggler_dir, 'vc7', 'VPR', 'vpr')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(juggler_dir, 'vc7', 'VPR')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'share', 'vpr', 'test')
   srcdir  = os.path.join(juggler_dir, 'modules', 'vapor', 'test')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'vpr')
   srcroot = os.path.join(juggler_dir, 'modules', 'vapor')

   extra_files = ['ChangeLog', 'COPYING.txt', 'README.txt', 'RELEASE_NOTES.txt']
   for f in extra_files:
      shutil.copy2(os.path.join(srcroot, f), destdir)

def installTweek(prefix):
   print "Installing Tweek C++ headers, libraries, and data files ..."

   destdir = os.path.join(prefix, 'include', 'tweek')
   srcdir  = os.path.join(juggler_dir, 'modules', 'tweek', 'tweek')
   installDir(srcdir, destdir, ['.h', '.idl'])

   srcdir  = os.path.join(juggler_dir, 'vc7', 'Tweek_CXX', 'tweek')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(juggler_dir, 'vc7', 'Tweek_CXX')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'share', 'tweek', 'test')
   srcdir  = os.path.join(juggler_dir, 'modules', 'tweek', 'test')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'tweek', 'data')
   srcdir  = os.path.join(juggler_dir, 'modules', 'tweek', 'data')
   installDir(srcdir, destdir)

   destdir = os.path.join(prefix, 'share', 'tweek')
   srcroot = os.path.join(juggler_dir, 'modules', 'tweek')

   extra_files = ['ChangeLog', 'COPYING.txt', 'RELEASE_NOTES.txt']
   for f in extra_files:
      shutil.copy2(os.path.join(srcroot, f), destdir)

def installTweekJava(prefix):
   srcdir = os.path.join(juggler_dir, 'vc7', 'Tweek_Java')

   if os.path.exists(os.path.join(srcdir, 'Tweek.jar')):
      print "Installing Tweek Java libraries and data files ..."

      jars = [
         'Tweek.jar',
         'TweekBeanDelivery.jar',
         'TweekBeans.jar',
         'TweekEvents.jar',
         'TweekNet.jar',
         'TweekServices.jar',
         'kunststoff-mod.jar',
         'ui.jar',
         'wizard.jar'
      ]

      beans     = ['Viewers']
      ext_beans = ['WizardBuilder']

      destdir = os.path.join(prefix, 'bin')
      mkinstalldirs(destdir)

      # Install the base JAR files that make up the Tweek Java API.
      for j in jars:
         shutil.copy2(os.path.join(srcdir, j), destdir)

      destdir = os.path.join(prefix, 'bin', 'beans')
      mkinstalldirs(destdir)

      bean_srcdir = srcdir
      xml_srcdir  = os.path.join(juggler_dir, 'modules', 'tweek', 'java')

      # Install the standard Tweek Beans.
      for b in beans:
         jar = b + '.jar'
         xml = b + '.xml'
         shutil.copy2(os.path.join(bean_srcdir, jar), destdir)
         shutil.copy2(os.path.join(xml_srcdir, xml), destdir)

      xml_srcdir  = os.path.join(juggler_dir, 'modules', 'tweek', 'extensions',
                                 'java')

      # Install the extension Tweek Beans.
      for b in ext_beans:
         jar = b + '.jar'
         xml = b + '.xml'
         shutil.copy2(os.path.join(bean_srcdir, jar), destdir)
         shutil.copy2(os.path.join(xml_srcdir, xml), destdir)

      # Install tweek.bat.
      srcdir = os.path.join(juggler_dir, 'modules', 'tweek', 'java')
      destdir = os.path.join(prefix, 'bin')
      shutil.copy2(os.path.join(srcdir, 'tweek.bat'), destdir)

      # Install JacORB IDL compiler.
      srcdir = os.path.join(juggler_dir, 'external', 'JacORB')
      installDir(srcdir, destdir, ['.jar'])
      shutil.copy2(os.path.join(srcdir, 'idl.bat'), destdir)

      # Install JDOM.
      srcdir = os.path.join(juggler_dir, 'external', 'jdom')
      installDir(srcdir, destdir, ['.jar'])

      # Install various look and feel implementations.
      laf_jars = [
         r'jgoodies-looks\looks.jar',
         r'liquid\liquidlnf.jar',
         r'metouia\metouia.jar'
      ]

      srcroot = os.path.join(juggler_dir, 'external', 'swing-laf')
      for j in laf_jars:
         shutil.copy2(os.path.join(srcroot, j), destdir)
   else:
      print "Tweek Java API not built.  Skipping."

def installJCCL(prefix):
   print "Installing JCCL C++ headers, libraries, and tools ..."

   destdir = os.path.join(prefix, 'include', 'jccl')
   srcdir  = os.path.join(juggler_dir, 'modules', 'jackal', 'common', 'jccl')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(juggler_dir, 'modules', 'jackal', 'config', 'jccl')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(juggler_dir, 'modules', 'jackal', 'rtrc', 'jccl')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(juggler_dir, 'vc7', 'JCCL', 'jccl')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(juggler_dir, 'vc7', 'JCCL')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'share', 'jccl', 'test')
   srcdir  = os.path.join(juggler_dir, 'modules', 'jackal', 'test')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'jccl', 'tools')
   srcdir  = os.path.join(juggler_dir, 'modules', 'jackal', 'tools')
   installDir(srcdir, destdir)

   destdir = os.path.join(prefix, 'share', 'jccl', 'data')
   srcdir  = os.path.join(juggler_dir, 'modules', 'jackal', 'data')
   installDir(srcdir, destdir)

   destdir = os.path.join(prefix, 'share', 'jccl')
   srcroot = os.path.join(juggler_dir, 'modules', 'jackal')

   extra_files = ['ChangeLog', 'COPYING.txt', 'RELEASE_NOTES.txt']
   for f in extra_files:
      shutil.copy2(os.path.join(srcroot, f), destdir)

def installJCCLPlugins(prefix):
   print "Installing JCCL C++ plug-ins ..."

   destdir = os.path.join(prefix, 'lib', 'jccl', 'plugins')
   srcroot = os.path.join(juggler_dir, 'vc7', 'JCCL', 'RTRC_Plugin_CXX')
   installLibs(srcroot, destdir, extensions = ['.dll'])

def installJCCLJava(prefix):
   srcdir = os.path.join(juggler_dir, 'vc7', 'JCCL_Java')

   if os.path.exists(os.path.join(srcdir, 'jccl_config.jar')):
      print "Installing JCCL Java libraries and data files ..."

      destdir = os.path.join(prefix, 'bin', 'beans')

      for f in ['jccl_config.jar', 'jccl_editors.jar']:
         shutil.copy2(os.path.join(srcdir, f), destdir)

      srcdir = os.path.join(juggler_dir, 'modules', 'jackal', 'config')
      shutil.copy2(os.path.join(srcdir, 'jccl_config.xml'), destdir)
   else:
      print "JCCL Java API not built.  Skipping."

def installJCCLPluginsJava(prefix):
   srcdir = os.path.join(juggler_dir, 'vc7', 'JCCL_Java', 'RTRC_Plugin_Java')

   if os.path.exists(os.path.join(srcdir, 'jccl_rtrc.jar')):
      print "Installing JCCL Java plug-ins ..."

      destdir = os.path.join(prefix, 'bin', 'beans')
      shutil.copy2(os.path.join(srcdir, 'jccl_rtrc.jar'), destdir)

      srcdir = os.path.join(juggler_dir, 'modules', 'jackal', 'plugins',
                            'corba_rtrc')
      shutil.copy2(os.path.join(srcdir, 'jccl_rtrc.xml'), destdir)
   else:
      print "JCCL Java plug-ins not built.  Skipping."

def installSonix(prefix):
   print "Installing Sonix headers, libraries, and samples ..."

   destdir = os.path.join(prefix, 'include', 'snx')
   srcdir  = os.path.join(juggler_dir, 'modules', 'sonix', 'snx')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(juggler_dir, 'vc7', 'Sonix', 'snx')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(juggler_dir, 'vc7', 'Sonix')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'share', 'sonix', 'samples')
   srcdir  = os.path.join(juggler_dir, 'modules', 'sonix', 'samples')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'sonix', 'data')
   srcdir  = os.path.join(juggler_dir, 'modules', 'sonix', 'data')
   installDir(srcdir, destdir)

   destdir = os.path.join(prefix, 'share', 'sonix')
   srcroot = os.path.join(juggler_dir, 'modules', 'sonix')

   extra_files = ['ChangeLog', 'COPYING.txt', 'README.txt']
   for f in extra_files:
      shutil.copy2(os.path.join(srcroot, f), destdir)

def installSonixPlugins(prefix):
   print "Installing Sonix plug-ins ..."

   destdir_dbg = os.path.join(prefix, 'lib', 'snx', 'plugins', 'dbg')
   destdir_opt = os.path.join(prefix, 'lib', 'snx', 'plugins', 'opt')

   srcroot = os.path.join(juggler_dir, 'vc7', 'Sonix', 'OpenAL')
   installLibs(srcroot, destdir_dbg, ['DebugDLL'], ['.dll'])
   installLibs(srcroot, destdir_opt, ['ReleaseDLL'], ['.dll'])

   srcroot = os.path.join(juggler_dir, 'vc7', 'Sonix', 'Audiere')
   installLibs(srcroot, destdir_dbg, ['DebugDLL'], ['.dll'])
   installLibs(srcroot, destdir_opt, ['ReleaseDLL'], ['.dll'])

def installGadgeteer(prefix):
   print "Installing Gadgeteer headers, libraries, and samples ..."

   destdir = os.path.join(prefix, 'include', 'gadget')
   srcdir  = os.path.join(juggler_dir, 'modules', 'gadgeteer', 'gadget')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(juggler_dir, 'vc7', 'Gadgeteer', 'gadget')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'include', 'cluster')
   srcdir  = os.path.join(juggler_dir, 'modules', 'gadgeteer', 'cluster')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(juggler_dir, 'vc7', 'Gadgeteer')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'share', 'gadgeteer', 'data')
   srcdir  = os.path.join(juggler_dir, 'modules', 'gadgeteer', 'data')
   installDir(srcdir, destdir)

   destdir = os.path.join(prefix, 'share', 'gadgeteer', 'samples')
   srcdir  = os.path.join(juggler_dir, 'modules', 'gadgeteer', 'samples')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'gadgeteer', 'test')
   srcdir  = os.path.join(juggler_dir, 'modules', 'gadgeteer', 'test')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'gadgeteer', 'tools')
   srcdir  = os.path.join(juggler_dir, 'modules', 'gadgeteer', 'tools')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'gadgeteer')
   srcroot = os.path.join(juggler_dir, 'modules', 'gadgeteer')

   extra_files = ['ChangeLog', 'COPYING.txt', 'RELEASE_NOTES.txt']
   for f in extra_files:
      shutil.copy2(os.path.join(srcroot, f), destdir)

def installGadgeteerDrivers(prefix):
   print "Installing Gadgeteer device drivers ..."

   destdir = os.path.join(prefix, 'lib', 'gadgeteer', 'drivers')
   srcroot = os.path.join(juggler_dir, 'vc7', 'Gadgeteer')

   drivers = ['DataGlove', 'Fastrak', 'Flock', 'IBox', 'IntersenseAPI',
              'IS900', 'MotionStar', 'MSFTSpeechRecognition', 'PinchGlove', 'SerialEncoder',
              'TrackdAPI', 'VRPN']

   for d in drivers:
      srcdir = os.path.join(srcroot, d)
      installLibs(srcdir, destdir, extensions = ['.dll'])

def installGadgeteerPlugins(prefix):
   print "Installing Gadgeteer cluster plug-ins ..."

   destdir = os.path.join(prefix, 'include', 'plugins',
                          'ApplicationDataManager')
   srcdir  = os.path.join(juggler_dir, 'modules', 'gadgeteer', 'plugins',
                          'ApplicationDataManager')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib', 'gadgeteer', 'plugins')
   srcroot = os.path.join(juggler_dir, 'vc7', 'Gadgeteer')

   plugins = ['ApplicationDataManager', 'RemoteInputManager',
              'StartBarrierPlugin', 'SwapLockTCPPlugin']

   for p in plugins:
      srcdir = os.path.join(srcroot, p)
      installLibs(srcdir, destdir, extensions = ['.dll'])

def installVRJuggler(prefix):
   print "Installing VR Juggler headers, libraries, and samples ..."

   destdir = os.path.join(prefix, 'include', 'vrj')
   srcdir  = os.path.join(juggler_dir, 'modules', 'vrjuggler', 'vrj')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(juggler_dir, 'vc7', 'VRJuggler', 'vrj')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(juggler_dir, 'vc7', 'VRJuggler')
   installLibs(srcroot, destdir)

   srcroot = os.path.join(juggler_dir, 'vc7', 'VRJuggler',
                          'OpenGL_Draw_Manager')
   installLibs(srcroot, destdir)

   srcroot = os.path.join(juggler_dir, 'vc7', 'VRJuggler',
                          'Performer_Draw_Manager')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'share', 'vrjuggler', 'data')
   srcdir  = os.path.join(juggler_dir, 'modules', 'vrjuggler', 'data')
   installDir(srcdir, destdir)

   destdir = os.path.join(prefix, 'share', 'vrjuggler', 'samples')
   srcdir  = os.path.join(juggler_dir, 'modules', 'vrjuggler', 'samples')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'vrjuggler', 'test')
   srcdir  = os.path.join(juggler_dir, 'modules', 'vrjuggler', 'test')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'vrjuggler', 'tools')
   srcdir  = os.path.join(juggler_dir, 'modules', 'vrjuggler', 'tools')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'vrjuggler')
   srcroot = os.path.join(juggler_dir, 'modules', 'vrjuggler')

   extra_files = ['ChangeLog', 'COPYING.txt', 'RELEASE_NOTES.txt']
   for f in extra_files:
      shutil.copy2(os.path.join(srcroot, f), destdir)

def installVRJConfig(prefix):
   srcdir = os.path.join(juggler_dir, 'vc7', 'VRJConfig')

   if os.path.exists(os.path.join(srcdir, 'VRJConfig.jar')):
      print "Installing VRJConfig ..."

      lib_jars = [
         'Devices.jar',
         'ClusterWizard.jar'
      ]
      bean_jars = [
         'VRJConfig.jar',
         'IntersenseEditor.jar',
         'ProxyEditor.jar',
         'SurfaceDisplayEditor.jar'
      ]

      destdir = os.path.join(prefix, 'bin')
      for j in lib_jars:
         jar_file = os.path.join(srcdir, j)
         if os.path.exists(jar_file):
            shutil.copy2(jar_file, destdir)

      destdir = os.path.join(prefix, 'bin', 'beans')
      for j in bean_jars:
         jar_file = os.path.join(srcdir, j)
         if os.path.exists(jar_file):
            shutil.copy2(jar_file, destdir)

      srcdir = os.path.join(juggler_dir, 'modules', 'vrjuggler', 'vrjconfig')
      shutil.copy2(os.path.join(srcdir, 'VRJConfig.xml'), destdir)

      # Install vrjconfig.bat.
      destdir = os.path.join(prefix, 'bin')
      shutil.copy2(os.path.join(srcdir, 'vrjconfig.bat'), destdir)

      # Install dependencies.
      dep_jars = [
         r'jgraph\jgraphpad.jar',
         r'TableLayout\TableLayout.jar'
      ]

      srcroot = os.path.join(juggler_dir, 'external')
      for j in dep_jars:
         shutil.copy2(os.path.join(srcroot, j), destdir)
   else:
      print "VRJConfig not build.  Skipping."

def installMsvcRT(prefix):
   print "Installing MSVC runtime DLLs"

   try:
      srcroot = os.environ['SystemRoot']
      destdir = os.path.join(prefix, 'lib')

      # Get *every* MSVC runtime DLL.  This list could be shortened at some
      # point if anyone cares to try.
      dlls = glob.glob(os.path.join(srcroot, 'System32', 'msvc*.dll'))

      for d in dlls:
         shutil.copy2(d, destdir)
   except KeyError, ex:
      print "WARNING: Could not install MSVC runtime DLLs"
      print ex

def doDependencyInstall(prefix):
   makeTree(prefix)
   installNSPR(prefix)
   installCppDOM(prefix)
   installBoost(prefix)
   installGMTL(prefix)
   installAudiere(prefix)
   installOpenAL(prefix)
   installOmniORB(prefix)

def simpleInstall(name, root, prefix, optional = False):
   if optional and root == '':
      return

   print "Installing", name

   # Install all header files.
   srcdir = os.path.join(root, 'include')

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'include')
      installDir(srcdir, destdir, ['.h', '.hpp', '.ipp'])

   # Install all libraries.
   srcdir = os.path.join(root, 'lib')

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'lib')
      installDir(srcdir, destdir)

   # Install all executables.
   srcdir = os.path.join(root, 'bin')

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'bin')
      installDir(srcdir, destdir)

def installNSPR(prefix):
   simpleInstall('NSPR headers and libraries', os.environ['NSPR_ROOT'],
                 prefix)

def installCppDOM(prefix):
   simpleInstall('CppDOM headers and libraries', os.environ['CPPDOM_ROOT'],
                 prefix)

def installBoost(prefix):
   print "Installing Boost headers and libraries"

   srcroot = os.environ['BOOST_ROOT']

   srcdir = os.environ['BOOST_INCLUDES']
   destdir = os.path.join(prefix, 'include')
   installDir(srcdir, destdir)

   srcdir = os.path.join(srcroot, 'lib')
   destdir = os.path.join(prefix, 'lib')
   mkinstalldirs(destdir)

   lib_list = glob.glob(os.path.join(srcdir, '*filesystem*')) + \
              glob.glob(os.path.join(srcdir, '*python*'))

   for f in lib_list:
      shutil.copy2(f, destdir)

def installGMTL(prefix):
   simpleInstall('GMTL headers', os.environ['GMTL_ROOT'], prefix)

def installAudiere(prefix):
   simpleInstall('Audiere headers, libraries, and executables',
                 os.getenv('AUDIERE_ROOT', ''), prefix, True)

def installOpenAL(prefix):
   print "Installing OpenAL DLL"
   srcdir  = os.environ['OPENAL_ROOT']
   if srcdir != "":
       destdir = os.path.join(prefix, 'bin')
       shutil.copy2(os.path.join(srcdir, 'dll', 'OpenAL32.dll'), destdir)

def installOmniORB(prefix):
   root = os.getenv('OMNIORB_ROOT', '')
   if root == '':
      return

   print 'Installing omniORB headers, libraries, and executables'

   # Install all header files.
   srcdir = os.path.join(root, 'include')

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'include')
      installDir(srcdir, destdir, ['.h', '.hh'])

   # Install all libraries.
   # NOTE: When we install the omniORB .lib files, we get rid of the x86_win32
   # subdirectory.
   srcdir = os.environ['OMNIORB_LIB']

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'lib')
      installDir(srcdir, destdir)

   srcdir = os.path.join(root, 'lib', 'python')

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'lib', 'python')
      installDir(srcdir, destdir)

   # Install all executables and DLLs.
   # NOTE: When we install the omniORB .dll files, we get rid of the x86_win32
   # subdirectory.
   srcdir = os.environ['OMNIORB_BIN']

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'bin')
      installDir(srcdir, destdir)

   srcdir = os.path.join(root, 'bin', 'scripts')

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'bin', 'scripts')
      installDir(srcdir, destdir)
      
def Str2TkinterStrVar(Str):
   TempStrVar = Tkinter.StringVar()
   TempStrVar.set(Str)
   return TempStrVar

class Win32SetupFrontEnd:
   def __init__(self, master):
      self.mRoot = master
      self.mRoot.title("VrJuggler Win32 Build -- Python")

      Options = setDefaultVars()
      self.mOptions = {}
      #make a StringVar Dictionary
      for k in Options:
         self.mOptions[k] = Str2TkinterStrVar(Options[k])
   
      self.makeOptionsInterface()

   def makeOptionsInterface(self):
      #Set up the Frames
      PadAmmount=10

      #Settings
      self.Juggler = "#0EAE06"
      self.JugglerYellow = "#EECE26"
      self.JugglerPurple = "#8E76AA"
      self.HeaderFont = (16)
      
      self.mRoot.HeaderFrame = Tkinter.Frame(self.mRoot, borderwidth=1, relief="sunken", bg=self.JugglerYellow)
      self.mRoot.HeaderFrame.grid(row=0, column=0, sticky=Tkinter.N+Tkinter.E+Tkinter.S+Tkinter.W, ipadx=PadAmmount, ipady=PadAmmount)
      
      self.mRoot.SettingsFrame = Tkinter.Frame(self.mRoot, borderwidth=1, relief="sunken", bg=self.JugglerYellow)
      self.mRoot.SettingsFrame.grid(row=0, column=1, sticky=Tkinter.N+Tkinter.E+Tkinter.S+Tkinter.W, ipadx=PadAmmount, ipady=PadAmmount)

      self.mRoot.CommandFrame = Tkinter.Frame(self.mRoot, borderwidth=1, relief="sunken", bg=self.JugglerYellow)
      self.mRoot.CommandFrame.grid(row=1, column=0, sticky=Tkinter.N+Tkinter.E+Tkinter.S+Tkinter.W, ipadx=PadAmmount, ipady=PadAmmount)

      self.mRoot.OutputFrame = Tkinter.Frame(self.mRoot, borderwidth=1, relief="sunken", bg=self.JugglerYellow)
      self.mRoot.OutputFrame.grid(row=1, column=1, sticky=Tkinter.N+Tkinter.E+Tkinter.S+Tkinter.W, ipadx=PadAmmount, ipady=PadAmmount)

      self.mRoot.StatusFrame = Tkinter.Frame(self.mRoot, borderwidth=1, relief="sunken", bg=self.JugglerYellow)
      self.mRoot.StatusFrame.grid(row=2, column=0, columnspan=2, sticky=Tkinter.W+Tkinter.E)
      
      #HeaderFrame Innards
      self.mRoot.HeaderFrame.vjImage = Tkinter.PhotoImage(file="modules/vrjuggler/vrjconfig/org/vrjuggler/vrjconfig/images/juggler.logo.100x100.gif", format="gif")
      self.mRoot.HeaderFrame.Image = Tkinter.Label(self.mRoot.HeaderFrame, image=self.mRoot.HeaderFrame.vjImage, bg=self.JugglerYellow)
      self.mRoot.HeaderFrame.Image.grid(row=0, column=0, sticky=Tkinter.N)
      self.mRoot.HeaderFrame.ImageLabel = Tkinter.Label(self.mRoot.HeaderFrame, text="VR Juggler Win32 Build Configuration", bg=self.JugglerYellow, font=self.HeaderFont)
      self.mRoot.HeaderFrame.ImageLabel.grid(row=1, column=0, sticky=Tkinter.N)
      self.mRoot.HeaderFrame.rowconfigure(1, weight = 1)
      
      #SettingsFrame Innards
      self.mRoot.SettingsFrame.RequiredSettingsFrame = Tkinter.Frame(self.mRoot.SettingsFrame, borderwidth=1, relief="sunken", bg=self.JugglerYellow)
      self.mRoot.SettingsFrame.RequiredSettingsFrame.grid(row=1, column=0, sticky=Tkinter.N+Tkinter.E+Tkinter.S+Tkinter.W)
      self.mRoot.SettingsFrame.RequiredSettingsFrame.SettingsRows = {}
      self.mRoot.SettingsFrame.OptionalSettingsFrame = Tkinter.Frame(self.mRoot.SettingsFrame, borderwidth=1, relief="sunken", bg=self.JugglerYellow)
      self.mRoot.SettingsFrame.OptionalSettingsFrame.grid(row=2, column=0, sticky=Tkinter.N+Tkinter.E+Tkinter.S+Tkinter.W)
      self.mRoot.SettingsFrame.OptionalSettingsFrame.SettingsRows = {}
      
      #RequiredSettingsFrame
      NextRow = 0
      self.mRoot.SettingsFrame.RequiredSettingsFrame.Label = Tkinter.Label(self.mRoot.SettingsFrame.RequiredSettingsFrame, font=self.HeaderFont, text="Required Settings", bg=self.JugglerPurple)
      self.mRoot.SettingsFrame.RequiredSettingsFrame.Label.grid(row=NextRow, column=0, columnspan=3, sticky=Tkinter.EW)
      self.mRoot.SettingsFrame.RequiredSettingsFrame.columnconfigure(0, weight = 1)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.RequiredSettingsFrame,"Instalation Prefix:", 'prefix', NextRow)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.RequiredSettingsFrame,"Boost C++ version:", 'BOOST_VERSION',NextRow,True, False)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.RequiredSettingsFrame,"Boost C++ instalation directory:", 'BOOST_ROOT',NextRow)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.RequiredSettingsFrame,"Directory Containing Boost C++ Header Tree:", 'BOOST_INCLUDES',NextRow)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.RequiredSettingsFrame,"Boost.Build toolset used to Construct Boost C++:", 'BOOST_TOOL',NextRow,True, False)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.RequiredSettingsFrame,"NSPR instalation directory:", 'NSPR_ROOT',NextRow)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.RequiredSettingsFrame,"CppDom instalation directory:", 'CPPDOM_ROOT',NextRow)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.RequiredSettingsFrame,"GMTL instalation directory:", 'GMTL_ROOT',NextRow)
      NextRow = NextRow+1

      #OptionalSettingsFrame
      NextRow = 0
      self.mRoot.SettingsFrame.OptionalSettingsFrame.Label = Tkinter.Label(self.mRoot.SettingsFrame.OptionalSettingsFrame, font=self.HeaderFont, text="Optional Settings", bg=self.JugglerPurple)
      self.mRoot.SettingsFrame.OptionalSettingsFrame.Label.grid(row=NextRow, column=0, columnspan=3, sticky=Tkinter.EW)
      self.mRoot.SettingsFrame.OptionalSettingsFrame.columnconfigure(0, weight = 1)
      NextRow = NextRow+1

      self.makeEntryRow(self.mRoot.SettingsFrame.OptionalSettingsFrame,"Dependency instalation prefix:", 'deps-prefix',NextRow, False)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.OptionalSettingsFrame,"Java instalation directory:", 'JAVA_HOME',NextRow, False)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.OptionalSettingsFrame,"Java3D instalation directory:", 'JAVA3D_HOME',NextRow, False)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.OptionalSettingsFrame,"OmniORB instalation directory:", 'OMNIORB_ROOT',NextRow, False)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.OptionalSettingsFrame,"OpenGL Performer instalation directory:", 'PFROOT',NextRow, False)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.OptionalSettingsFrame,"VRPN instalation directory:", 'VRPN_ROOT',NextRow, False)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.OptionalSettingsFrame,"Audiere instalation directory:", 'AUDIERE_ROOT',NextRow, False)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.OptionalSettingsFrame,"OpenAL SDK instalation directory:", 'OPENAL_ROOT',NextRow, False)
      NextRow = NextRow+1
      self.makeEntryRow(self.mRoot.SettingsFrame.OptionalSettingsFrame,"TrackdAPI instalation directory:", 'TRACKD_API_ROOT',NextRow, False)
      NextRow = NextRow+1
      #self.makeEntryRow(self.mRoot.mFrame,"Microsoft Speech SDK instalation directory:", '',NextRow, False)
      #NextRow = NextRow+1

      #CommandFrame Innards
      NextRow = 0
      self.mRoot.CommandFrame.InstallJugglerCheck = Tkinter.Checkbutton(self.mRoot.CommandFrame,text="Install Juggler", bg=self.JugglerYellow, activebackground=self.JugglerYellow, onvalue="Yes", offvalue="No")
      self.mRoot.CommandFrame.InstallJugglerCheck.Variable = Tkinter.StringVar()
      self.mRoot.CommandFrame.InstallJugglerCheck.Variable.set("Yes")
      self.mRoot.CommandFrame.InstallJugglerCheck["variable"] = self.mRoot.CommandFrame.InstallJugglerCheck.Variable
      self.mRoot.CommandFrame.InstallJugglerCheck.grid(row=NextRow, column=0, sticky=Tkinter.EW, pady=4)
      NextRow = NextRow+1
      self.mRoot.CommandFrame.InstallJugglerDepsCheck = Tkinter.Checkbutton(self.mRoot.CommandFrame,text="Install Juggler Dependancies", bg=self.JugglerYellow, activebackground=self.JugglerYellow, onvalue="Yes", offvalue="No")#,command=self.installDeps, state="disabled")
      self.mRoot.CommandFrame.InstallJugglerDepsCheck.Variable = Tkinter.StringVar()
      self.mRoot.CommandFrame.InstallJugglerDepsCheck.Variable.set("No")
      self.mRoot.CommandFrame.InstallJugglerDepsCheck["variable"] = self.mRoot.CommandFrame.InstallJugglerDepsCheck.Variable
      self.mRoot.CommandFrame.InstallJugglerDepsCheck.grid(row=NextRow, column=0, sticky=Tkinter.EW, pady=4)
      NextRow = NextRow+1
     
      self.mRoot.CommandFrame.BuildInstallButton = Tkinter.Button(self.mRoot.CommandFrame,text="Build and Install",command=self.doBuild, state="disabled")
      self.mRoot.CommandFrame.BuildInstallButton.grid(row=NextRow, column=0, sticky=Tkinter.EW, pady=4)
      NextRow = NextRow+1

      #OutputFrame Innards
      self.mRoot.OutputFrame.MessageText = Tkinter.Text(self.mRoot.OutputFrame, height=20, width=100, state='disabled')
      self.mRoot.OutputFrame.MessageText.grid(row=0, column=0)
      self.mRoot.OutputFrame.MessageText.tag_config("a", foreground="blue")

      self.mRoot.OutputFrame.MessageText.ScrollBar = Tkinter.Scrollbar(self.mRoot.OutputFrame)
      self.mRoot.OutputFrame.MessageText.ScrollBar.grid(row=0, column=1, sticky=Tkinter.W+Tkinter.N+Tkinter.S)

      self.mRoot.OutputFrame.MessageText.config(yscrollcommand=self.mRoot.OutputFrame.MessageText.ScrollBar.set)
      self.mRoot.OutputFrame.MessageText.ScrollBar.config(command=self.mRoot.OutputFrame.MessageText.yview)

      #StatusFrame Innards
      self.mRoot.StatusFrame.Label = Tkinter.Label(self.mRoot.StatusFrame, text="Status: ")
      self.mRoot.StatusFrame.Label.grid(row=0, column=0, rowspan=2)#, sticky="W")
      self.mRoot.StatusFrame.StatusLabel = Tkinter.Label(self.mRoot.StatusFrame, text="Test", anchor=Tkinter.W)
      self.mRoot.StatusFrame.StatusLabel.grid(row=0, column=1, sticky=Tkinter.EW)
      self.mRoot.StatusFrame.columnconfigure(1, weight = 1)

      self.update()

   def update(self):
         self.updateRequiredOptions()
         self.updateOptionsValidation()
         self.updateCommandFrame()

   def entryValidation(self, Entry, Update=False):
      if os.path.isdir( Entry.get() ):
         Entry.config(fg="blue")
      else:
         Entry.config(fg="red")
      if Update:
         self.updateRequiredOptions()
         self.updateCommandFrame()
      
   def updateOptionValidation(self, Setting):
      if Setting["isDirectory"]:
         self.entryValidation(Setting["Entry"])
            
   def updateOptionsValidation(self):
      for k in self.mRoot.SettingsFrame.RequiredSettingsFrame.SettingsRows:
         self.updateOptionValidation(self.mRoot.SettingsFrame.RequiredSettingsFrame.SettingsRows[k])
      for k in self.mRoot.SettingsFrame.OptionalSettingsFrame.SettingsRows:
         self.updateOptionValidation(self.mRoot.SettingsFrame.OptionalSettingsFrame.SettingsRows[k])
         
   def updateRequiredOptions(self):
      self.mReqSettingsSet = True
      for k in self.mRoot.SettingsFrame.RequiredSettingsFrame.SettingsRows:
         if self.mOptions[k].get() == "":
            self.mReqSettingsSet = False

   def updateCommandFrame(self):
      if self.mReqSettingsSet:
         #self.mRoot.CommandFrame.BuildButton.config(state="normal")
         self.mRoot.CommandFrame.BuildInstallButton.config(state="normal")
      else:
         #self.mRoot.CommandFrame.BuildButton.config(state="disabled")
         self.mRoot.CommandFrame.BuildInstallButton.config(state="disabled")
            
      
   def validateOptions(self):
      RetValue = True

      #Make sure that all options that are directories reference valid directories
      InvDirList = []
      for k in self.mOptions:
         if (self.mOptionWidgetsDict[k][4]) and ( not os.path.isdir(self.mOptions[k].get()) and (self.mOptions[k].get() != "")):
            RetValue = False
            InvDirList.append( self.mOptionWidgetsDict[k][0]['text'] )
      if not RetValue:
         tkMessageBox.showwarning(
            "Invalid Directory",
            "The following settings reference non-existant directories and must be set before proceeding.\n%s" % (InvDirList) )
         return RetValue

      return RetValue

   def doBuild(self):
      #Set the Environment Vars
      for k in self.mOptions.iterkeys():
         os.environ[k] = self.mOptions[k].get()

      if True:#self.validateOptions():
         if os.environ['JAVA_HOME'] != '' and os.path.exists(os.environ['JAVA_HOME']):
            jdk_path = os.path.join(os.environ['JAVA_HOME'], 'bin')
            os.environ['PATH'] = jdk_path + os.pathsep + os.environ['PATH']
            os.environ['JACORB_PATH'] = os.path.join(juggler_dir, r'external\JacORB')

         if os.environ['OMNIORB_ROOT'] != '' and os.path.exists(os.environ['OMNIORB_ROOT']):
            omni_bin = os.path.join(os.environ['OMNIORB_ROOT'], 'bin')

            if os.path.exists(os.path.join(omni_bin, 'omniidl.exe')):
               os.environ['OMNIORB_BIN'] = omni_bin
            else:
               os.environ['OMNIORB_BIN'] = os.path.join(omni_bin, 'x86_win32')

            # Extend the path to include omniORB's bin directory.
            os.environ['PATH'] = os.environ['OMNIORB_BIN'] + os.pathsep + os.environ['PATH']

            omni_lib = os.path.join(os.environ['OMNIORB_ROOT'], 'lib')

            if os.getenv('PYTHONPATH', '') != '':
               os.environ['PYTHONPATH'] = os.path.join(omni_lib, 'python') + os.pathsep + os.environ['PYTHONPATH']
            else:
               os.environ['PYTHONPATH'] = os.path.join(omni_lib, 'python')

            if os.path.exists(os.path.join(omni_lib, 'omnithread.lib')):
               os.environ['OMNIORB_LIB'] = omni_lib
            else:
               os.environ['OMNIORB_LIB'] = os.path.join(omni_lib, 'x86_win32')

            omni_glob = os.path.join(os.environ['OMNIORB_LIB'], 'omniORB*_rt.lib')
            libs = glob.glob(omni_glob)
            omni_ver_re = re.compile(r'omniORB(\d\d\d)_rt.lib')

            for l in libs:
               match = omni_ver_re.search(l)
               if match is not None:
                  os.environ['OMNIORB_VERSION'] = match.group(1)
                  break

         cache_file = os.path.join(juggler_dir, 'options.cache')
         cache_file = open(cache_file, 'w')
         for k, v in self.mOptions.iteritems():
            output = "options['%s'] = r'%s'\n" % (k, v.get())
            cache_file.write(output)
         cache_file.close()
         
         self.BuildThread = threading.Thread(None, self.runVisualStudio, "BuildThread")
         self.FinishedBuildThread = threading.Thread(None, self.waitForBuildFinish, "FinishedBuildThread")
         self.BuildThread.start()
         self.FinishedBuildThread.start()

   def installJuggler(self):
      doInstall(self.mOptions['prefix'].get())

   def installDeps(self):
      doDependencyInstall(self.mOptions['deps-prefix'].get())

   def getFile(self, optionIndex, initialdir, toEntry):
      def clearAndGet(self, optionIndex, initialdir):
         ResultDir = tkFileDialog.askdirectory(title=optionIndex, initialdir=initialdir)
         if ResultDir != '':
            self.mOptions[optionIndex].set(ResultDir)

      return lambda: clearAndGet(self, optionIndex, initialdir)
   
   def makeEntryRow(self, master, label, optionIndex, row, Required=True, isDirectory=True):
      #Label
      tempLabelRef = Tkinter.Label(master, text=label)
      tempLabelRef.grid(row=row, column=0, sticky=Tkinter.N+Tkinter.E+Tkinter.S+Tkinter.W, pady=2)

      #Entry
      tempEntryRef = Tkinter.Entry(master, width=75, textvariable=self.mOptions[optionIndex])
      tempEntryRef.grid(row=row, column=1, sticky=Tkinter.N+Tkinter.E+Tkinter.S+Tkinter.W, pady=2)
      if isDirectory:
         if Required:
            self.mOptions[optionIndex].trace_variable('w', lambda n, i, m: self.entryValidation(tempEntryRef, True))
         else:
            self.mOptions[optionIndex].trace_variable('w', lambda n, i, m: self.entryValidation(tempEntryRef, False))


      #Button, if there is one
      if (not isDirectory):
         tempButtonRef = None
      else:
         tempButtonRef = Tkinter.Button(master, text='Browse', command=self.getFile(optionIndex, self.mOptions[optionIndex].get(), tempEntryRef))
         tempButtonRef.grid(row=row, column=2, sticky=Tkinter.N+Tkinter.E+Tkinter.S+Tkinter.W, pady=2)
      master.SettingsRows[optionIndex] = {
         "Label" : tempLabelRef,
         "Entry" : tempEntryRef,
         "Button" : tempButtonRef,
         "isRequired" : Required,
         "isDirectory" : isDirectory
         }
      
   def buildFinished(self):
      print self.mRoot.CommandFrame.InstallJugglerCheck.Variable.get()
      print self.mRoot.CommandFrame.InstallJugglerDepsCheck['state']
      if self.mRoot.CommandFrame.InstallJugglerCheck.Variable.get() == "Yes":
         self.mRoot.OutputFrame.MessageText['state'] = 'normal'
         self.mRoot.OutputFrame.MessageText.insert(Tkinter.END, "Installing Juggler...\n", "a")
         self.mRoot.OutputFrame.MessageText['state'] = 'disabled'
         doInstall(self.mOptions['prefix'].get())
      if self.mRoot.CommandFrame.InstallJugglerDepsCheck.Variable.get() == "Yes":
         self.mRoot.OutputFrame.MessageText['state'] = 'normal'
         self.mRoot.OutputFrame.MessageText.insert(Tkinter.END, "Installing Juggler Dependancies...\n", "a")
         self.mRoot.OutputFrame.MessageText['state'] = 'disabled'
         doDependencyInstall(self.mOptions['deps-prefix'].get())
      self.mRoot.OutputFrame.MessageText['state'] = 'normal'
      self.mRoot.OutputFrame.MessageText.insert(Tkinter.END, "Build and Installation Finished.\n", "a")
      self.mRoot.OutputFrame.MessageText['state'] = 'disabled'

   def waitForBuildFinish( self ):
      while self.BuildThread.isAlive():
         i=0
      self.buildFinished()

   def runVisualStudio( self ):
      #print "generateVersionHeaders()"
      self.mRoot.OutputFrame.MessageText['state'] = 'normal'
      self.mRoot.OutputFrame.MessageText.insert(Tkinter.END, "Generating Version Headers.\n", "a")
      self.mRoot.OutputFrame.MessageText['state'] = 'disabled'
      generateVersionHeaders()
      self.mRoot.OutputFrame.MessageText['state'] = 'normal'
      self.mRoot.OutputFrame.MessageText.insert(Tkinter.END, "Generating Ant Build Files.\n", "a")
      self.mRoot.OutputFrame.MessageText['state'] = 'disabled'
      generateAntBuildFiles()
      devenv_cmd = None
      for p in str.split(os.getenv('PATH', ''), os.pathsep):
   #      print "Searching in", p
         if os.path.exists(os.path.join(p, 'devenv.exe')):
            devenv_cmd = os.path.join(p, 'devenv.exe')
            break

      if devenv_cmd is None:
         # The environment variable %VSINSTALLDIR% is set by vsvars32.bat.
         print "WARNING: Falling back on the use of %VSINSTALLDIR%"
         devenv_cmd = r'%s' % os.path.join(os.getenv('VSINSTALLDIR', ''),
                                           'devenv.exe')

      (devenv_cmd_no_exe, ext) = os.path.splitext(devenv_cmd)
      devenv_cmd_no_exe = '"%s"' % (devenv_cmd_no_exe)

      #solution_file = r'%s' % os.path.join(juggler_dir, 'vc7', 'Juggler.sln')
      solution_file = r'%s' % ('C:\Solution1\Solution1.sln')
      build_args = r'/build Debug'

      cmd = devenv_cmd_no_exe + ' ' + solution_file + ' ' + build_args

      print cmd
      try:
         (stdout, stdin) = os.popen2(cmd)

         while True:
            line = stdin.readline()
            if not line:
               break
            self.mRoot.OutputFrame.MessageText['state'] = 'normal'
            self.mRoot.OutputFrame.MessageText.insert(Tkinter.END, line)
            self.mRoot.OutputFrame.MessageText.yview("moveto", 1.0)
            self.mRoot.OutputFrame.MessageText['state'] = 'disabled'

         return 0
      except OSError, osEx:
         print "Could not execute %s: %s" % (cmd, osEx)
         sys.exit(3)

def main():
   if ErrorImportingTkinter:
      print "Tkinter dependencies not found switching to command-line mode."
      options = setVars()
      generateVersionHeaders()
      generateAntBuildFiles()

      devenv_cmd = None
      for p in str.split(os.getenv('PATH', ''), os.pathsep):
   #      print "Searching in", p
         if os.path.exists(os.path.join(p, 'devenv.exe')):
            devenv_cmd = os.path.join(p, 'devenv.exe')
            break

      if devenv_cmd is None:
         # The environment variable %VSINSTALLDIR% is set by vsvars32.bat.
         print "WARNING: Falling back on the use of %VSINSTALLDIR%"
         devenv_cmd = r'%s' % os.path.join(os.getenv('VSINSTALLDIR', ''),
                                           'devenv.exe')

      solution_file = r'%s' % os.path.join(juggler_dir, 'vc7', 'Juggler.sln')

      try:
         status = os.spawnl(os.P_WAIT, devenv_cmd, 'devenv', solution_file)

         if status == 0:
            print "Proceed with VR Juggler installation [y]: ",
            proceed = sys.stdin.readline().strip(" \n")
            if proceed == '' or proceed.lower().startswith('y'):
               doInstall(options['prefix'])

               print "Proceed with VR Juggler dependency installation [y]: ",
               proceed = sys.stdin.readline().strip(" \n")
               if proceed == '' or proceed.lower().startswith('y'):
                  doDependencyInstall(options['deps-prefix'])
      except OSError, osEx:
         print "Could not execute %s: %s" % (devenv_cmd, osEx)
         sys.exit(3)

      sys.exit(0)
   else:
      root = Tkinter.Tk()

      myApp = Win32SetupFrontEnd(root)

      root.mainloop()


juggler_dir = os.path.dirname(os.path.abspath(sys.argv[0]))
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
