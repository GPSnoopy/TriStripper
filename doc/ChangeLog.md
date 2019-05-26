# Tri Stripper Change Log

## 26/05/2019

GitHub import. Move to CMake. Removed a few obsolete classes. Misc cleanup.

## 09/06/2005

Released Tri Stripper 1.10 Beta 5.
This release fixes a supposedly was-fixed-in-beta-4 bug.

- [tri_stripper] Fixed a silent out-of-range error in heap_array::pop(), that would arise when the last element in the heap was popped.
- [test program] Added a more compatible and less buggy stdint.h.
- [test program] Now compiles and work on Linux AMD64 (fixed a small problem with 32 bits versus 64 bits indices).

## 16/02/2005

Released Tri Stripper 1.10 Beta 4.
This release fixes two bugs have been lurking there since 1.0. Thanks to Grisha Spivak for reporting them.

- [tri_stripper] Fixed a silent out-of-range error in heap_array::erase(), that would happen when the element to be erased is located at the end of the heap. (Fixed in 1.10 Beta 3)
- [tri_stripper] Fixed a silent out-of-range error in heap_array::pop(), that would arise when the last element in the heap was popped.

## 09/01/2005

Released Tri Stripper 1.10 Beta 2.

- [tri_stripper] Added SetPushCacheHits() to match the cache model used by newer GPUs. To quote Jason Hughes who emailed me about this limitation: "The FIFO on an NV25 and more recent check the FIFO for an index's presence before pushing/popping onto the queue". By default, the stripifier still uses the old behavior.
- [tri_stripper] Added a protection against min/max macros. This will generate a readable error message, but it's up to you to resolve the issue. Thanks to Patrik Müller for reporting this.

## 14/11/2004

Added a Tri Stripper 1.01 "configure/make/install" package for UNIX-like systems.

Released Tri Stripper 1.10 Beta 1.
During the summer I've made a major rewrite of most of Tri Stripper. Unfortunately, I haven't found the time to finish the whole thing for a final release, and since then it's been rotting on the Subversion repository, left untouched. But Tri Stripper itself is done, and what has still to be finished is the test program (I wanted to add command line arguments) and the package itself (making a nice configure/make/install one). I decided to export the repository as it stands and release it as 1.10 Beta 1.

Here is a non-exhaustive list of the major changes:

- [tri_stripper] A refreshed interface. Beware that it's incompatible with the old one even though changes are small.
- [tri_stripper] Semantic change for the constructor: now when created, a tri_stripper object isn't linked with the indices anymore, instead it directly builds the internal structures in the constructor. Meaning that you can dispose as you wish of the indices you gave to the tri_stripper object after it's been constructed, it won't affect the outcome of Strip() calls.
- [tri_stripper] Fixed a couple of possible bugs (better be safe than sorry).
- [tri_stripper] Speed improvement of the algorithm ranging from 10% to 50% depending on the platform. This is mainly due to the fact that the code of the internal structures was rewritten, leading to smaller memory usage, less memory allocations and better memory cache coherency. Also removed some useless operations.
- [tri_stripper] Added a new configuration parameter called "Backward Search". When disabled, the stripifier algorithm behaves exactly as it did in 1.01. When enabled, it treats the candidate triangle as also possibly being the last triangle of a strip and not only the possible start triangle. Backward search makes the algorithm slower (although almost negligible) but provides better results. Beware that backward search is mainly intended to be used when the given CacheSize = 0, some early tests have shown it is *not* suited when stripifying for a target 3D system that has a post T&L vertex cache.
- [test program] Should be portable enough to run on big-endian platforms now, but I haven't got the chance to test it.
- [all] Replaced all the #pragma once with regular C header guards.

## 11/07/2004

In the test program: removed the "press enter to continue", as well as some exit code, which were causing problems with GLUT.
Website: removed all older versions from the website, except 1.0 Beta 5 source files.


## 10/11/2003

Released Tri Stripper 1.01 Final. Improved template code of graph_array.h (implicit typename fixed).

## 23/04/2003 *

Released Tri Stripper 1.00 Final. It doesn't bring any new features nor does fix any bugs. But it changes a few thing in the architecture of the code to make it cleaner and more portable. Also it was tested with Visual C++ .NET 2003 and GCC 3.2; both compile the same code so there is no need for those "patches" anymore. But this move comes at the cost of letting down older compilers. If you have troubles compiling this new version, you can always try the old 1.00 Beta 5:

- 1.00 BETA 5
- MAIN ARCHIVE - Windows (VC++ .NET 7.0)
- Linux x86-32 Patch (makefile for GCC 3.2)
- Windows (VC++ 6.0) Patch

## 10/12/2002

Released Tri Stripper 1.00 BETA 5. It fixes a bug that could cause it to go into an infinite loop in some cases. For example when sending some degenerated triangles in the right order, or when using a CacheSize = 2 with MinStripSize = 2. (Thanks to Remy for his helpful bug report)

It also contains a newer version of "graph_array.h". The changes and the bug fixes aren't located in the functions used by tri_stripper, so that changes absolutely nothing for Tri Stripper but it's just in case someone would want to use this header in its own project.

Also included "stdafx.h" in the separated source "tri_stripper_100B5.zip", this should make the integration of Tri Stripper easier in some projects (read: "projects that don't already contain "stdafx.h" with the right headers" (read: "projects that aren't made under Visual Studio")). ;-)

## 18/11/2002

Released Tri Stripper 1.00 BETA 4. This new version fixes a bug that could cause it to crash (thanks to Nicolas for finding it).

This new version also modifies a bit its interface in order to remove the dependency on OpenGL (thanks to Patrik for finding this useless dependency). This means that a new type was added, "primitive_type", which introduce "PT_Triangles" and "PT_Triangle_Strip". It also means "gl_primitives" and "gl_primitives_vector" were respectively renamed to "primitives" and "primitives_vector".

Because of this modification in the interface, some applications using Tri Stripper may not compile with this new version. To avoid further problems "PT_Triangles" and "PT_Triangle_Strip" values are respectively equal to GL_TRIANGLES and GL_TRIANGLE_STRIP so that applications using the older versions only have to rename the used types to be up-to-date again; but new applications should avoid relying on their values.

If you don't want to break your application because of this modification of interface but still want the previously stated bug fixed you can download Tri Stripper 1.00 BETA 3; but future versions will use this new interface.

## 16/11/2002

Released Tri Stripper 1.00 BETA 2. No bug fix, just an improved portability with compilers that aren't 100% compliant to the C++ standard.

Moved to a patch system for the "Example and test program": the main package for the test program is the Windows (VC++ .NET 7.0) version, and other packages for other platforms are simply patches that have to be applied to that main package. Note that these patches only replace files relative to the test program 3D engine and not relative to the Tri Stripper itself (The TriStripper source files are the same among all the platforms).

Updated the makefile for GCC 3.2 for better optimizations, now the GCC executable is as fast as the Win32 one.

I'd like to thanks some people (they'll recognize themselves) for helping me with the VC6 patch. It wouldn't have seen the daylight without them.
