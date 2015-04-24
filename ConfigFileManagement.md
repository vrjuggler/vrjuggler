# General Issues #

To date, there have been no recommendations for managing VR Juggler configurations. Users have undoubtedly adopted conventions of their own, but it would be good if vendors of VR Juggler applications could count on certain things being true about how customers' configurations are set up. On this page, we aim to specify conventions (standards?) for VR Juggler configuration management.

## Parallel VR Juggler Installations ##

If we allow [parallel VR Juggler installations](PackageManagement.md), then we need to allow parallel VR Juggler configurations. This is necessary because configurations may not be compatible between releases. This would happen if new definition versions are added in a newer Juggler release or if new configurable components are added.

As with conventions set up for installing parallel releases of VR Juggler, a convention would have to be chosen for how to have parallel VR Juggler configurations. A reasonable approach on Linux would be to put Juggler configuration files in `/etc/vrjuggler`. The versioning could come into play in one of two ways:

  1. Versioning on the `vrjuggler` directory: `/etc/vrjuggler-2.0`
  1. A version-specific sub-directory: `/etc/vrjuggler/2.0`

Either approach ought to be equally easy to handle in the Juggler software.

Q: Where would configuration files go on Windows?

<font color='blue'>Could we use the registry on windows to point to the site wide VR Juggler configuration directory (the equivalent of /etc/vrjuggler above)? -AB </font>

Q: How version specific should these directory names be?

For example if we have a vrj 2.0.5 release will that use config files in /etc/vrjuggler/2.0.5?  In the stable releases we could probably avoid this but in the development versions we will probably need the patch number as part of the version number.

Q: Should we provide a fallback to older versioned directories?

If we are searching for default.jconf with a vrjuggler 2.3 development release should it fallback to searching in `/etc/vrjuggler/2.2` ?

<font color='blue'>
Imagine the following use case.  You want to send a self-contained application to a site that already has vrjuggler.  You are using a version of VR Juggler that has not been installed at the site before.  For now let's imagine a development release of vrjuggler 2.3.  You want to be able to just unpackage the application (with the associated libraries) and run the application using the local default configuration.  For this to work either a) we have to provide a way to fallback through older versions or b) the end-user needs to copy their existing configuration files over to the new versioned directory.<br>
<br>
This manually copying of configuration files will be required any time a new version of vrjuggler is installed and used.  If at all possible I would like to simplify this and allow things to work if the old configuration files are still compatible.  -AB<br>
</font>

## Extended Configuration File Search Path ##

Currently, VR Juggler uses a search path to find `.jconf` files when no path is provided to said files. The search order is the following:

  1. The local directory (i.e., the current working directory)
  1. `$VJ_BASE_DIR/share/vrjuggler/data/configFiles`

We could extend this to include more directories to allow for finding site-specific configurations more easily. A possible new default search path could be the following:

  1. The current working directory
  1. `$HOME/.vrjuggler`
    * On Windows and Mac OS X, this should probably use the platform-specific convention for application data file storage. On Windows, the directory would be `Application Data\VR Juggler Configuration` (or similar) in the user's profile directory. On Mac OS X, the directory would be `$HOME/Library/VRJugglerConfiguration` (or similar). The `Configuration` part of the directory name could probably be dropped since the proposed `.vrjuggler` directory does not have this substring.
    * It might be wise to include version-specific information for this directory on all platforms. As with the system-wide directory name, the version number could go into the base directory name, or there could be a version-specific subdirectory. The same convention should be used for both cases.
    * Being able to override this with an environment variable (`VJ_USER_DIR`?) could be beneficial. With this capability, then the version information should almost certainly be handled using a subdirectory (e.g., `$VJ_USER_DIR/2.0`) so that users do not have to worry about capturing versioning in the environment variable setting.
  1. The versioned site-wide Juggler configuration directory
  1. `$VJ_BASE_DIR/share/vrjuggler/data/configFiles`

This could almost be achieved right now using `VJ_CFG_PATH` except that the new directories are inserted into the middle of the search path.

## Default Configuration ##

For easy launching of a VR Juggler application, it would be beneficial if users did not have to know the specifics of a given site's configuration. A default VR Juggler configuration file would make this possible. For example, this file could be `default.jconf`, and it could be looked up using the default search path and `VJ_CFG_PATH` just as would any other `.jconf` file.

Currently, many (most?) VR Juggler applications are written to require one or more configuration files to be given on the command line. The logic for a default configuration would probably go into the VR Juggler kernel, but existing applications would have to be modified to allow launching without a configuration file.

<font color='blue'>How will this work at sites that have multiple VR systems?  Is there anyway it could work or will users just always need to pass the base configuration file for the given VR system?  -AB</font>

## Cluster Configuration Management ##

The VR Juggler clustering infrastructure has been designed to allow the same configuration to be used on all the nodes of the cluster. This was done for ease of maintenance, but it is only easy to maintain this single configuration if there is in fact ''one'' installation. If the configuration is duplicated on all the cluster nodes, then changes have to be propagated. In general, exporting `/etc` or a subdirectory thereof using NFS (or other networked file system) is probably undesirable. However, mounting the configuration directory in `/etc` (or making a symlink from `/etc` to the mounted configuration) is not unreasonable. Of course, symbolic links are not an option on Windows.


<font color='blue'>If we provided a way for the user to change where the system looks for the site-wide Juggler configuration this issue could be worked around.  For example if we provided and  environment variable (like <code>VJ_BASE_CONFIG_DIRECTORY</code> ) then the scripts for running cluster applications could redirect this environment variable to point at a configuration repository on a shared filesystem. -AB </font>