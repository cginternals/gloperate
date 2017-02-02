![gloperate Logo](gloperate-logo.png "gloperate")

C++ library for defining and controlling modern GPU rendering/processing operations

## Project Health (master)

| Service | System | Compiler | Status |
| ------- | ------ | -------- | -----: |
|  [Travis-CI](https://travis-ci.org/cginternals/globjects) | Ubuntu 14.04 | GCC 4.8, Clang 3.5 | upcoming |
|  [Travis-CI](https://travis-ci.org/cginternals/globjects) | OS X | Clang ? | upcoming |
| [Coverity](https://scan.coverity.com/projects/6831?tab=overview) | Ubuntu 14.04 | GCC 5.4 | [![Coverity Status](https://scan.coverity.com/projects/6831/badge.svg)](https://scan.coverity.com/projects/6831) |
| Jenkins <br><br><br><br> | Ubuntu 14.04 <br><br><br><br> | GCC 4.8 <br> GCC 4.9 <br> GCC 5.4 <br> Clang 3.9 <br> | upcoming <br> upcoming <br> upcoming <br> upcoming <br> |
| Jenkins <br><br> | Windows 10 <br><br> | MSVC 2013 Update 5 <br>  MSVC 2015 Update 1 <br> | upcoming <br>upcoming <br> |

Please note that our OS X build node is currently broken (physically). However, *gloperate* is maintained for OS X as well and there are many people using it on OS X on a regular basis.

### Dependencies

The following dev-libraries and programs need to be provided for correct CMake configuration:
* C++11 compatible compiler (e.g. gcc >=4.7, MSVC >=2013)
* CMake (>=2.8.9, better 2.8.12): http://www.cmake.org/
* OpenGL C++ Bindings (glbinding >=1.0.0): https://github.com/cginternals/glbinding
* OpenGL Mathematics (GLM >=0.9.4): http://glm.g-truc.net/, https://github.com/g-truc/glm
* OpenGL Objects Wrapper (globjects >=0.3.2): https://github.com/cginternals/globjects

### Optional Dependencies

* Window and Context creation (GLFW 3.1): http://www.glfw.org/
* Qt (>=5.4): http://qt-project.org/

### Development Notes

If you are contributing to this project, please keep the following notes in mind:
* Add your name and email to the AUTHORS file.
* Follow coding conventions according to google's [C++ Style Guide](http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml).
* Commits should provide a meaningful  message, which uses the imperative, present tense: "change", not "changed" nor "changes", AND start with a capital letter.
* Commits should always refer to an issue: use ```#xxx```, ```fix #xxx```, or ```close #xxx```.
* Pull Requests are reviewed by at least one other developer on another platform.
* Use lazy initialization as often as possible for time consuming tasks.
* Member Variables always start with ```m_```, ```g_```, and ```s_``` (e.g.,```m_member```, ```g_global```, and ```s_static```)..
* Interface or abstract class names always start with Abstract (e.g., ```AbstractArray```).
* Enforce strict include sequence: cpp-related header, std, glm, glbinding, globjects.
