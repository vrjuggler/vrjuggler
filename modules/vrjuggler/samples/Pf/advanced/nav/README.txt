navigator...

Cave-style navigation based on allen's Naver abstraction to terry's code..

Still needs much cleaning, but this (the CaveNavigator class) does
actually navigate like the old CAVE apps now (like terry's code did).

CaveNavigator is the class to use - it uses vjMatrix which *if* you're
using performer, you'll need to convert and then copy to a pfDCS.

CaveNavigator also works under OpenGL because of this.  Only the
planeCollider will work under OpenGL though, since it is the only collider
not tied to performer (duh)...

CaveNavigator has a bunch of statistical stuff, like speed and etc...

I've also started a new collider that uses a ray to test for intersection.
The ray goes from [whereYouAre] to [whereYouWantToBe].  Strangely we
didn't have one of those yet (terry?), we only had volume collider (which
*kind* of did that, but was restrictive), and RideCollider (which only   
points down - real restrictive/specialized)...



TODO: I'd like to make the hierarchy a little more shallow by
incorporating terry's code into the Collider classes.  I found the
hierarchy a little deep and hard to debug at times.


