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
