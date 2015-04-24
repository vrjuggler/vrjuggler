**IMPORTANT**: This page has not yet been updated completely for the switch to Subversion.

## Branch Overview ##

### Spelling Conventions ###

  * The use of underscores is preferred over hyphens as a word separator.

### Branch Creation ###

Commands for creating branch and branch points:

```
svn copy ...
```

## Module: juggler ##

### Primary Branches ###

  * `trunk`: The mainline branch being used to develop VR Juggler 2.3 (the code that will become VR Juggler 3.0).
  * `juggler/branches/1.0_docs`: This branch contains DocBook versions of all VR Juggler 1.0 documentation (no code).  It is maintained here to allow easy merging from the mainline branch when appropriate.

### Release Branches ###

(These exist to allow a given release to stabilize with bug fixes.  Changes are normally merged from `trunk` as appropriate.)

  * `juggler/branches/2.0`: The branch used for the VR Juggler 2.0.x release series.  It was created from the trunk on June 6, 2005.  The first release from this branch was VR Juggler 2.0 Beta 3.
  * `juggler/branches/2.2`:

### In-Development Branches ###

(These have not yet been merged back to the branch from which they were created.)

### Defunct Branches ###

  * `juggler/branches/2.0_alpha_2`: A branch created from the VR Juggler 2.0 Alpha 2 release tag.  This branch was created July 3, 2003, from `trunk` to allow a stable code base to be used for SIGGRAPH 2003 while also allowing the `jccl_config_refactor_branch` code to be merged back to `trunk`.  This branch was used to prepare the VR Juggler 2.0 Alpha 3 bug fix release.
  * `juggler/branches/2.0_alpha_4`: A branch created for making the VR Juggler 2.0 Alpha 4 release.  It was created from `trunk` using the branch point `VRJ_2_0_ALPHA_4_BRANCH_bp` on April 28, 2004.  This branch will allow a stable code base to be used for IPT 2004.

### Future Branches ###

## Module: PyJuggler ##

### Primary Branch ###
  * `HEAD`: The mainline branch being used to develop PyJuggler 1.1 (the code that will become PyJuggler 1.2).

### Release Branches ###
(These exist to allow a given release to stabilize with bug fixes.  Changes are normally merged from `HEAD` as appropriate.)

  * `RELENG_1_0`: The branch used for the PyJuggler 1.0.x release series, which is kept up to date with the VR Juggler 2.0.x release series.
  * `RELENG_0_7`: The source used for PyJuggler 0.7.x, which corresponds with VR Juggler 2.0 Alpha 2 and Alpha 3.  Changes from `HEAD` are merged to improve stability and multi-platform usability only.