# Making a VR Juggler Release #

## Pieces ##

"VR Juggler" is basically supposed to be everything, which means that _a lot_ of code goes into a release distribution.  The following lists everything that goes into a distribution of VR Juggler (for a 2.0 pre-release).

  * VPR
  * Tweek
    * Java support which requires CORBA
    * C++ CORBA support which requires omniORB 4.0.3 or newer (for full compiler support)
  * JCCL
    * Java support
  * Sonix
    * All possible plug-ins (OpenAL and Audiere as a bare minimum)
  * Gadgeteer
    * All drivers except TrackdAPI (we don't have a copy of it)
    * For VRPN, use 06.06 or newer as it is the first version that compiles with Visual C++ 7.1
  * VR Juggler and VRJConfig

## Steps ##

### Source Distribution ###

  1. Check out a clean copy of the `juggler` module.  ''Do not'' run cvs-gather.
  1. Increment the version in `juggler/modules/vrjuggler/VERSION`
    * Do this so that the release is against a specific VR Juggler version.
  1. Update `juggler/modules/vrjuggler/RELEASE-NOTES.txt` with any relevant information regarding API changes, _major_ bug fixes, and cool new features.
  1. Tag the tree with a release tag (referred to below as `&lt;TAG_NAME&gt;`).
  1. Remove the source tree you were just working with and run the following:

```
svn export http://vrjuggler.googlecode.com/svn/juggler/tags/<TAG_NAME> vrjuggler-x.y.z-src
cd vrjuggler-x.y.z-src
./autogen.sh
```

Once the above is done, tar up the fully populated source tree.  Compress it with `bzip2 -9` to make it as small as possible. This is the source distribution.

### Binary Distributions ###

  * Use maximum compression when creating the final archive to be uploaded to SourceForge.
  * Use `make release` unless otherwise noted.
  * The target `install-deps` installs all the detected dependencies.  By default, it installs into the same tree where VR Juggler is installed by the `release` target, but this can be changed by setting the varaible `deps_prefix` when running `make`.  For example:

```
make prefix=/home/users/me/vrjuggler-2.0 release
make deps_prefix=/home/users/me/vrjuggler-2.0-deps install-deps
```

#### Before Beginning ####

  * Install the freshly made source distribution in `/home/vr/Juggler/2.0`.
    * Make all builds against this distribution, though do them in `/tmp` or `/var/tmp` to make the process a little faster.  `configure` will find the source code in /home/vr/Juggler/2.0, so don't worry about that.
  * When running `configure.pl`, pass `--prefx=/TEMP_DIRECTORY/vrjuggler-VERSION.PLATFORM` so that the installation happens on the local disk.
    * Once the distribution has been completed, it can be moved into `/home/vr/Juggler/2.0`.

#### IRIX ####

  * Don't use the IRIX tar to untar...Use a linux box to untar because IRIX tar cannot make all of the symlinks right
  * Build N32/mips3 (default) SPROC
  * Build N32/mips3 (default) POSIX
  * Build 64/mips4 (`--with-abi=64_M4`) SPROC
  * Build 64/mips4 (`--with-abi=64_M4`) POSIX

#### Linux ####

  * Build POSIX

#### Windows ####

  * Build NSPR debug and release
    * The Python script `buildwin32.py` will ask for a path for the VR Juggler installation and for the dependency installation.  These can be the same if desired.
    * Select everything under the Batch Build dialog and build.
    * Compress the binary distribution tree using WinZip with maximum compression.

#### Mac OS X ####

  * Build Cocoa version as a universal binary with the prefix set to `/usr/local`

```
make prefix=$HOME/vrjuggler/usr/local release
make deps_prefix=$HOME/vrjuggler/usr/local/vrjuggler-deps install-deps
cd $HOME/vrjuggler/usr/local
mkdir ../../Applications
mv Tweek.app VRJConfig.app ../../Applications
```

  * Create a package for isntalling and add the `preflight` and `postflight` scripts
  * Create a disk image for distribution purposes

#### Documentation ####

The DocBook-related tools needed for rendering the documentation can be found as attachments to WritingDocumentationUsingDocBook.

  * Build a documentation release:

```
make -f Makefile.docs
make -f Makefile.docs release
cd ~
mv vrj-docs.dist <versioned documentation distribution name>
```

### Things to check before releasing ###

  * All Boost files were installed, including the few `.h` files that they have in their source tree.

### Uploading to SourceForge ###

  * Read their [documentation](http://sourceforge.net/docman/display_doc.php?docid=6445&group_id=1) about making file releases.
  * Read their [documentation](http://sourceforge.net/docman/display_doc.php?docid=12834&group_id=1) about posting a news item (in this case, a release announcement).
    * Usually, it is fine to base an announcement on the last one that was made for that release series.

### Finishing Up ###

  * Send an announcement email to vrjuggler-announce@googlegroups.com.
  * Use the announcement to the VR Juggler lists to post a news item on our SourceForge project page.