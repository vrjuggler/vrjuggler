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


1.) Choose a directory to check out Juggler to.
setenv source_code_dir /home/users/joeuser/src

2.) Goto that directory and checkout juggler using CVS
cd "$source_code_dir"
cvs checkout juggler

3.) Enter the newly checked out Juggler source directory
cd "$source_code_dir"/juggler

4.) Compile juggler:
a.) Auto-Generate Headers
autoheader
b.) Auto-Generate the executable file "configure"
autoconf
c.) Probe the system for capabilities, generate Makefiles, etc...
    this "configures" the source tree for the current system
configure
d.) "Make" the distribution using gnu-make.  
    This compiles, links, and creates the final distribution tree 
    (in the form of sym-links).
gmake

5.) Add this line to your .tcshrc or .cshrc file.
    NOTE: make sure you put the full path if $source_code_dir is not 
    defined in .tcshrc
setenv VJ_BASE_DIR "$source_code_dir"/juggler/instlinks

