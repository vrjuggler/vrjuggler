### Q: What is a VR Juggler developer release? ###

I keep hearing about developer releases of VR Juggler.  What is a developer release?  What is VR Juggler 1.1?

### A: A developer release is a snapshot of in-development code that will eventually become a major, stable release. ###

#### Developer Releases ####

Many open source projects make use of unstable developer releases to give users a preview of what is to come in the next stable release.  A developer release is usually a snapshot of the code that has been deemed stable enough for "advanced" users to try.  The point is to get the code out to the people so that they can prepare for any major changes and so that they can report bugs to the developers.  The hope is that the final stable release will be much improved through the feedback provided by those users who try out the developer release.

A developer release is not for everyone, however.  There are many things to keep in mind, including, but not limited to, the following:

  * **The code is unstable.** Despite our best efforts, we are not always able to test every possible situation.  We make every attempt to ensure that developer releases are resonably stable and usable, but they are _not meant to be stable release_ and as such they do not recieve extensive testing before release.  Crashes may occur, and when they do, users should report them so that the developers can fix the bug(s).  In any case, always remember the following: _a developer release is not for production use_.
  * **Pieces may be missing.**  During development, some features may be disabled due to bugs or large syntactic and/or semantic changes.  These disabled features will be re-enabled by the time the stable release is made.
  * **The APIs are not finished.**  Because developer releases are snapshots of the current development, that APIs may not be in a final form.  Developers should not rely upon the APIs remaining consistent from one developer release to the next.  In most cases the APIs will remain the same, but there is no gaurantee of this being true.

#### VR Juggler 1.1 ####

VR Juggler 1.1 is the code that will become VR Juggler 2.0.  In our version numbering scheme, we use the following scheme: `<major>.<minor>.<patch>`.  The meanings of these fields are as follows:

  * The major number indicates the architecture.
  * The minor number indicates the API and the code stability.  Even numbers (0, 2, 4, etc.) indicate a stable release; odd numbers indicate an unstable release.
  * The patch number for stable releases indicates a minor increment since the previous release.  The changes include bug fixes and enhancements that do not change the API _at all_.

Hence, VR Juggler 1.1 is an unstable release line, and VR Juggler 2.0 will be a stable release line.

The use of the minor number to indicate stable vs. unstable releases is derived from the conventions used by the Linux kernel developers.  Many GNU projects also follow this convention.

Back to: LiveFaq

See also: VrJugglerDeveloperReleases