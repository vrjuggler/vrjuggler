# Guidelines for Version Bumping #

This page provides (mutable) guidelines for when and how to increment project version numbers.  In particular, this is geared towards the Juggler Project modules (VPR, Tweek, VR Juggler, PyJuggler, etc.), but the content is based on many years of maintaining software and observing other projects.  Nothing here is set in stone thanks to the magic of Wiki webs, so comments, corrections, and discussions are encouraged.

## Motivation ##

The motivation for having this page is to help everyone writing and using our software.  Proper versioning helps in tracking changes and in knowing what code is appropriate to use for a given task.  Well-defined versioning makes it possible to perform checks at the time of configuration, compilation, and execution to ensure that requirements are met.

## Basics ##

### Version Format ###

The format we use for version strings is relatively simple.  It is designed to encapsulate architectural changes, API changes, patches/bug fixes, and build numbers.  The format is as follows:

```
<major>.<minor>.<patch>-<build>
```

The major version number indicates the architecture revision.  The minor version number indicates the API version of the current architcture.  The patch (or micro) version number indicates the patch level of the current API.  These three make up the actual release version, and they are concatenated to create an integer version number using the following form:

```
MMMMmmmppp
```

Thus, there are four digits for the major version number, three for the minor version number, and three for the patch level.  (Leading zeroes are stripped to prevent interpretation as an octal value.)

Finally, the build number indicates which build of the actual version.  This is used when the source base is being built automatically, normally once per day.  Often times, the build number will be 0.

### Conventions ###

We follow the numbering convention set by the Linux kernel numbering scheme.  That is, an even-numbered minor version indicates a stable release branch, and an odd-numbered minor version indicates an unstable, in-development branch.  The exeception to this rule is when the major number is 0, which indicates that no stable release has been made yet.  Hence, _all_ 0.x releases are considered in-development.  This follows with the traditions set forth by hundreds of software projects, and it only makes sense for us to follow suit.

## Incrementing Version Numbers ##

### Major Version Increment ###

Deciding when to increment the major version is a tricky process.  Reaching a 1.0 release is part of that, of course.  Beyond that, determining when the underlying architcture has changed enough to warrant a major version increment may involve a lot of debate.  In general, no one person should make this decision--unless there is only one person working on the code.

### Minor Version Increment ###

Prior to a 1.0 release, the minor version number will probably be incremented frequently as the API is developed.  After 1.0, however, the general thinking is that the API should be stabilized.  Of course, with a revision control system in place, it's easy to keep the stable release on a branch of its own while development continues on the odd-numbered minor version line.

However, during development, the API could change.	This is to be expected since the purpose of the odd-numbered API version is to progress toward another even-numbered API version (the next in the series to be more specific).  Such changes should be considered carefully, however.  Based on the principles of software development, backwards compatibility, and versioning, it is reasonble to expect that between 2.0 and 2.2 (for example), the API will not change in a way that will break existing code.  Instead, new API features would be _added_ to extend the 2.0 interface.  If it seems as though there is a need for more than one minor version increment between two stable releases, there is probably something wrong.

### Patch Level Increment ###

Patches and bug fixes have differing significance depending on whether the source base is stabilized or in development.  For a stable source base, patches would probably be applied infrequently, usually based on user feedback and/or bug reports.  Whether the patch level should be incremented for every patch applied is difficult to decide.  When working from a stable source base, however, most projects tend to follow a methodology wherein the patch level is only incremented when a significant bug (or a significant number of bugs) is fixed, thus requiring a new release to get better code out to the users.

On the other hand, for an in-development source base, changes are going to occur frequently.  That's the point of the in-development tree.  A decision needs to be made about how often to increment the patch level.  For a 0.y.z version, it makes sense to increment the patch level when the API has not changed but a critical bug has been fixed.  For an x.y.z version (where y is an odd number), the patch level may be incremented for an important API change _or_ for an important bug fix.  Since it is already known that the API is being modified for the in-development code, the API change is more of a patch than anything else--especially if the aforementioned guideline for API additions is followed.

## General Rules ##

With all of the above in mind, here are the rules we should be trying to follow:

  1. Increment the major version number infrequrently (on the order of years).
  1. Increment the minor version number accordingly based on long-term goals and API revisions.  For 0.x releases, it is okay for the minor version number to change often--even multiple times in one day.  For [1-9]+.x releases, the minor version number should change based on the release schedule.
  1. Increment the patch level appropriately based on criticality of a bug fix or based on the maintenance release scheudle.
  1. Increment the build number only as part of an automated test build process.  Whenever one of the above version numbers is changed, always reset the build number to 0.