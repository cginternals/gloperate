![gloperate Logo](gloperate-logo.png "gloperate")

C++ library for defining and controlling modern GPU rendering/processing operations

## Project Health (master)

| Service | System | Compiler | Options | Status |
| ------- | ------ | -------- | ------- | ------ |
| Jenkins | Ubuntu 14.04 | GCC 4.8 | default | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=gloperate-linux-gcc4.8)](http://jenkins.hpi3d.de/job/gloperate-linux-gcc4.8)|
| Jenkins | Ubuntu 14.04 | GCC 4.9 | default | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=gloperate-linux-gcc4.9)](http://jenkins.hpi3d.de/job/gloperate-linux-gcc4.9)|
| Jenkins | Ubuntu 14.04 | Clang 3.5 | default | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=gloperate-linux-clang3.5)](http://jenkins.hpi3d.de/job/gloperate-linux-clang3.5) |
| Jenkins | OS X 10.10 | Clang 3.5 | default | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=gloperate-osx-clang3.5)](http://jenkins.hpi3d.de/job/gloperate-osx-clang3.5) |
| Jenkins | Windows 8.1 | MSVC 2013 Update 5 | default | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=gloperate-windows-msvc2013)](http://jenkins.hpi3d.de/job/gloperate-windows-msvc2013) |
| Jenkins | Windows 8.1 | MSVC 2015 | default | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=gloperate-windows-msvc2015)](http://jenkins.hpi3d.de/job/gloperate-windows-msvc2015) |

### Dependencies

The following dev-libraries and programs need to be provided for correct CMake configuration:
* C++11 compatible compiler (e.g. gcc >=4.7, MSVC >=2013)
* CMake (>=2.8.9, better 2.8.12): http://www.cmake.org/
* OpenGL C++ Bindings (glbinding >=1.0.0): https://github.com/hpicgs/glbinding
* OpenGL Mathematics (GLM >=0.9.4): http://glm.g-truc.net/, https://github.com/g-truc/glm
* OpenGL Objects Wrapper (globjects >=0.3.2): https://github.com/hpicgs/globjects

### Optional Dependencies

* Window and Context creation (GLFW 3.0): http://www.glfw.org/
* Qt (>=5.2): http://qt-project.org/

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
