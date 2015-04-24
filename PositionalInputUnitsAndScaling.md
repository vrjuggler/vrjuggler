### Background ###
There is currently an issue with gadgeteer and VR Juggler where applications can not easily move between systems that are using different units for their base configuration units (meters vs. feet).

We need a way to allow applications to move transparently between systems without regards to what units that system has been configured with.

### Proposed solution ###

**Configuration**

| Positional devices | Translation, rotation, and scale to get to common units for the given VR system |
|:-------------------|:--------------------------------------------------------------------------------|
| Gadgeteer | Scale value to convert for local system base units to common Juggler base units (meters) |
| VRJ Apps | Member function returns a scale value to convert from VRJ base units (meters) to whatever units application may be in |

This configuration method allows for:

  * Local systems to be configured using whatever base measurement units they like.  People can configure their systems with feet, meters, or even some imaginary base unit that only exists in their system.
  * Applications can use any measurement units that they like.

Restrictions:

  * All devices must be configured to return positional input in the same units and coordinate frame.  (this isn't much of a restriction since this was already required anyway)

Data flow:

The data would flow something like this...

```
Pos data > xform(device xform) > scale(gadget scalar * app scalar) => final pos
(flock bird)
```

### Notes ###

  * Helpers would be provided in the configuration chunks and in the application callback for setting and returning unit conversion factors.  For example in a configuration chunk, a user would be able to select from a pull-down list of available options like: meters, centimeters, inches, feet.  In an application, the user could return something like:  `return gadget::Unit::feet;`.  This would be an alias for the actual conversion factor.

The base class would have:

```
float vrj::App::getAppUnitScalar()
{
   return gadget::Units::Feet;
}
```

Then user applications could override this with any unit they like:

```
float UserApp::getAppUnitScalar()
{
   return gadget::Units::Meter;
}
```

I am not sure on the method name yet (any suggestions??), but this would be the basic idea.

### Comments ###

  * Why use meters as the base (internal) VR Juggler units? (why not feet, inches, centimeters, etc)

I chose meters because it is the international standard unit of measurement.  It is easy for most people to use and seemed liked a better choice than the other units.