
# ***************** <SNX heading BEGIN do not edit this line> *****************
#
# Audio Juggler
#
# Original Authors:
#   Kevin Meinert, Carolina Cruz-Neira
#
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# ****************** <SNX heading END do not edit this line> ******************
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
# *************** <auto-copyright.pl END do not edit this line> ***************

#!/bin/sh


###################################
# Edit this section with your info!
###################################

projectname=$1
format=$2

###################################
echo " "
echo "Kevin's tarballer 1.0"
echo "Creates a tarball with name \"projectname.version.format.tar.gz\""
echo "Send questions, comments to KevinMeinert@bigfoot.com"
echo "  Usage: "
echo "  * Default: \"$0 projectname format\""
echo "  * Install in custom location: \"$0 projectname format path\""
echo "  Example: \"$0 fractal src ~/public_html/pub\""
echo " "
echo " "
if [ $# -lt 1 ]; then
	echo "Not enough arguments"
	echo ""
	exit
fi
currentdir=`pwd`
version=`cat VERSION | sed "s/\[\([^]^[]*\)\].*/\1/g"`
tarballname="$projectname-$version.$format.tar"
newdirectoryname="$projectname-$version.$format"

echo "--- software version: $version"
echo "--- tarball name: $tarballname.gz"
echo "--- directory name: $newdirectoryname"
echo "--- "

if [ -d "$newdirectoryname" ]; then
	echo "cleaning out the old directory \"$newdirectoryname\""
	rm -r $newdirectoryname
fi

echo "creating temporary directory to hold tarball contents"
tempdirname=.tarballertempdir1234
mkdir ../$tempdirname
cp -r * ../$tempdirname
mv ../$tempdirname ./$newdirectoryname

echo "Making $tarballname"
tar -cvf $tarballname $newdirectoryname

echo "removing temporary directory"
rm -r $newdirectoryname

echo "Compressing $tarballname"
gzip -f $tarballname

echo " "
echo "Installing your tarball to:"
if [ $# -ge 3 ]; then
        location=$3
fi
if [ $# -lt 3 ]; then
        location=..
fi
mv $tarballname.gz $location
echo "$location/$tarballname.gz"
echo " "
