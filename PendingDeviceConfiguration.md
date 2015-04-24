# Q: Why does my input device configuration element end up in the pending list? #

When I run an application developed against VR Juggler 2.0 Alpha 1 or newer, the configuration element for my input device ends up in the pending list, and the application receives no input.  The output looks like this:

```
[0001596/000] DBG: ConfigManager::pendingNeedsChecked: Pending list is now
                   STALE: 1 items still in pending
                   NOTE: These items have been specified in configuration,
                         but have not been loaded.
                         This may be a problem in the configuration OR
                         it may be waiting for more configuration information.
[0001596/000] DBG: ---- Pending list: 1 items ----
                   ADD -->Isense1 type: Intersense
                   ----------------------------------
```

What is going wrong?

# A: The device driver plug-in is not being loaded correctly. #

Since October 2002, the Input Manager in [Gadgeteer](http://www.vrjuggler.org/gadgeteer/) has loaded device drivers at ''run time'' as plug-ins (components).  The driver plug-ins to be loaded must be configured in order for the device-specific configuration elements to be handled.  As part of this process, there are three things that are most likely to go wrong:

  1. Failure to configure the Input Manager
  1. Failure to load the driver plug-in
  1. Multiple Input Manager configuration elements with the same name

These are discussed in more detail below.

## Failure to Configure the Input Manager ##

If the Input Manager is not configured to load a device driver plug-in, then there will be no code in the system to handle the configuration element for the input device.  The Input Manager must be configured to load all the plug-ins that you need.  For VR Juggler 2.0 Alpha 1 through Alpha 3, the following XML configures the Input Manager to load the Ascension MotionStar driver and Fakespace Pinch Glove driver:

```
<?xml version="1.0" encoding="UTF-8"?>
<?org-vrjuggler-jccl-settings config.db.version="2.1"?>
<ConfigChunkDB>
   <InputManager name="Input Manager">
      <driver>${VJ_BASE_DIR}/lib/gadgeteer/drivers/MotionStar_drv.so</driver>
      <driver>${VJ_BASE_DIR}/lib/gadgeteer/drivers/PinchGlove_drv.so</driver>
   </InputManager>
</ConfigChunkDB>
```

**NOTE** Aspects of the above will vary depending on the target operating system.  For example, the extension `.dll` would be used on Windows instead of `.so`.

For versions of VR Juggler after 2.0 Alpha 3, the following configures the Input Manager in the same way:

```
<?xml version="1.0" encoding="UTF-8"?>
<?org-vrjuggler-jccl-settings configuration.version="3.0"?>
<configuration xmlns="http://www.vrjuggler.org/jccl/xsd/3.0/configuration"
               name="Dynamic Driver Loading Test"
               xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
               xsi:schemaLocation="http://www.vrjuggler.org/jccl/xsd/3.0/configuration http://www.vrjuggler.org/jccl/xsd/3.0/configuration.xsd">
   <elements>
      <input_manager name="Input Manager" version="2">
         <driver_path>${VJ_BASE_DIR}/lib/gadgeteer/drivers</driver_path>
         <driver_path>${VJ_BASE_DIR}/lib32/gadgeteer/drivers</driver_path>
         <driver>MotionStar_drv</driver>
         <driver>PinchGlove_drv</driver>
      </input_manager>
   </elements>
</configuration>
```

**Do not explicitly list the debug (`_d`) version of the driver for a `driver` property value!**  The debug/release issues are handled automatically by the Input Manager and do not require any help from the user.

The above does not have to vary for each platform, though it would have to change to handle a 64-bit build of VR Juggler on IRIX.

If you want to get maximum portability of a configuration file, do not include any values for the `driver_path` property.  The default driver search path will cause the Input Manager to look in the ABI-specific Gadgeteer driver directory for the build you are using.  In other words, the default search path will be one of the following directories based on the Juggler build:

  * `${VJ_BASE_DIR}/lib/gadgeteer/drivers` for all platforms except IRIX.
  * `${VJ_BASE_DIR}/lib32/gadgeteer/drivers` for N32 IRIX builds.
  * `${VJ_BASE_DIR}/lib64/gadgeteer/drivers` for N64 IRIX builds.

Furthermore, explicitly listing one of the above three directories as a `driver_path` property value in an `input_manager` configuration element is redundant.  The default paths will always be searched, but they will be searched '''after''' values entered for the `driver_path` property.

## Failure to Load Driver Plug-In ##

If the Input Manager is configured correctly but the output regarding the pending list is seen, then scrolling up will probably show one of the following output messages:

```
[0020472/000] DBG:Loading library: IS900_drv.so
[0020472/000] DBG:WARNING: Could not load 'IS900_drv.so' -- IS900_drv.so: cannot open shared object file: No such file or directory
```

or

```
[0001596/000] DBG:Loading device library: IS900_drv.so
[0001596/000] gadgetINP:Loaded DSO success.
[ FAILED lookup ]
[0001596/000] gadgetINP:WARNING: Failed to look up factory function in driver DSO 'IS900_drv.so'
```

Instead, the following should be seen:

```
[0020516/000] DBG:Loading library: IS900_drv.so	  [ OK ]
```

**NOTE:** The output above will vary depending on what operating system is used and what device driver is loaded.

Until the driver plug-in is loaded and registered, there will be no handler for the configuration element that the driver accepts.  Thus, the config element ends up in the pending list.

If no attempt is made to load a driver plug-in at all, then the Input Manager is not being configured correctly.  See above for information on how to configure the Input Manager.

## Multiple Input Manager Configuration Elements with the Same Name ##

In some cases, we have seen problems where multiple `InputManager` (or `input_manager`) config elements with the same name result in incomplete configurations.  To be safe, you should give each Input Manager configuration element a unique name.  Whether this is a bug or "just the way it works" is unclear at this time.

  * The name of a configuration element is specified in the `name` attribute of the XML tag.  It provides an identifier for that specific config element, and in some cases, having a unique name is absolutely require.  The name can be changed using VRJConfig.

Back to [FAQ](LiveFaq.md)