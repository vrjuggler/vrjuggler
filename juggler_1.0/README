# What is this: VR Juggler
#
# Organization Credits: Iowa State University
#                       Virtual Reality Applications Center (VRAC)
#
# Contributors (Alphabetically): 
#               Al Baker, Allen Bierbaum, Carolina Cruz-Neira, 
#               Jesse Coats, Patrick Hartling, Chris Just, Kevin Meinert
#
# This File's Intent: Describe what to do with this source tree - 
#                     How to compile, build and test VR Juggler
#
# 

------------------
Table of contents:
------------------
1.) Choose a directory to put Juggler.
2.) Goto that directory and checkout juggler using CVS
3.) Enter the newly checked out Juggler source directory
4.) Set the environment variable VJ_BASE_DIR
5.) Compile juggler (using the autoconf system, and gnu-make):
    a.) Auto-Generate Headers
    b.) Auto-Generate the configuration script "configure"
    c.) Probe the system for capabilities, generate Makefiles, etc...
        this "configures" the source tree for the current system
    d.) "Make" the distribution using gnu-make. 
6.) Advanced topics
    a.) Send compiler output to a different directory than the source tree.

--------------------------------------------------------------------------

1.) Choose a directory to put Juggler.
setenv source_code_dir /home/users/joeuser/src

2.) Goto that directory and checkout juggler using CVS
    NOTE: make sure CVS is set up, and pointing to the Juggler repository.
cd "$source_code_dir"
cvs checkout juggler

3.) Enter the newly checked out Juggler source directory
cd "$source_code_dir"/juggler

4.) Set the environment variable VJ_BASE_DIR
    NOTE: This is required by Juggler's build system (makefiles, etc..) 
    NOTE: 1.) The directory "instlinks" is semi-important. 
              What does the word "instlinks" mean? 
	      It is a working distribution of the Juggler library.
	      After "gmake" is done, you'll be able to use this directory 
	      to compile your code against.
          2.) It also determines where juggler files will be created:
	      Juggler files will be created in (some-directory)/instlinks/..
	      (in other words, files are created one dir back from instlinks)
    NOTE: Make sure this line is in your .tcshrc or .cshrc file.
setenv VJ_BASE_DIR "$source_code_dir"/juggler/instlinks


5.) Compile juggler (using the autoconf system, and gnu-make):
    a.) Auto-Generate Headers
autoheader
    
    b.) Auto-Generate the configuration script "configure"
autoconf
    
    c.) Probe the system for capabilities, generate Makefiles, etc...
        this "configures" the source tree for the current system
"$source_code_dir"/juggler/configure

    d.) "Make" the distribution using gnu-make.  
        This compiles, links, and creates the final distribution tree 
        (in the form of sym-links).
gmake


6.) Advanced topics
    a.) Send compiler output to a different directory than the source tree.
        Run this in the directory you want compiler output to go to.
	NOTE: You can change the word "myBuildDirectory" to a name of 
	      your choice.
setenv a_different_build_dir myBuildDirectory
mkdir "$source_code_dir"/juggler/"$a_different_build_dir"
cd "$source_code_dir"/juggler/"$a_different_build_dir"
"$source_code_dir"/juggler/configure
