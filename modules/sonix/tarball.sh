#!/bin/sh
if [ $# -lt 1 ]; then
	echo "syntax:   $0 somefilepath"
	echo "optional: $0 somefilepath make.exe"
        exit
fi

echo "Making the tarball target with TARBALLPATH=\"$1\""
make-tarball.sh jfx src $1
