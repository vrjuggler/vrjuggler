#!/bin/sh
# This is a very quick/dirty script for making the docs tarball easily...
# Maybe base this script on the current juggler build in the future...

if [ $# != 1 ]; then
        echo " "
        echo "Juggler Documentation Tarballer"
        echo "Send questions, comments to kevn@vrjuggler.org"
        echo "Usage: $0 [documentation version]"
        echo "Example: $0 0.1.95"
        echo " "
        exit
fi


VERSION="$1";
DESTINATION="pub/distributions/docs";
TARBALL="vrjuggler-"$VERSION".docs.tar.gz";
TEMPBALL="vrjuggler-"$VERSION".docs.tar";

echo " "
echo "Making version $VERSION Juggler documentation"
echo " "
cd ..;
echo "------------------------------------------------------------------------------------"
echo "Cleaning up any old temp files (if exist)"
echo "------------------------------------------------------------------------------------"
rm $TARBALL;
rm $TEMPBALL;
echo " "
echo "------------------------------------------------------------------------------------"
echo "Removing old Ver$VERSION tar files from $DESTINATION (dont' want to re-tar them, things might get huge)"
echo "------------------------------------------------------------------------------------"
rm "$DESTINATION"/"$TARBALL"
echo " "
echo "---------------------------------------"
echo "Archiving the documentation source tree"
echo "---------------------------------------"
tar -cvf $TEMPBALL *
echo " "
echo "--------------------------------------------"
echo "Compressing the documentation source tree..."
echo "--------------------------------------------"
gzip $TEMPBALL
echo " "
echo "-----------------------------------------------------------------"
echo "Putting the new tarball into the $DESTINATION directory"
echo "-----------------------------------------------------------------"
mv $TARBALL $DESTINATION
echo " "
