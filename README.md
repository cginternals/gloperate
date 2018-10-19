
<br><a href="https://github.com/cginternals/gloperate/"><img src="https://raw.githubusercontent.com/cginternals/gloperate/master/gloperate-logo.svg?sanitize=true" width="50%"></a>

*gloperate* is a cross-platform C++ library for defining and controlling modern GPU rendering/processing operations.

![GitHub release](https://img.shields.io/github/release/cginternals/gloperate.svg)
[![Travis](https://img.shields.io/travis/cginternals/gloperate/master.svg?style=flat&logo=travis)](https://travis-ci.org/cginternals/gloperate)
[![Appveyor](https://img.shields.io/appveyor/ci/scheibel/gloperate/master.svg?style=flat&logo=appveyor)](https://ci.appveyor.com/project/scheibel/gloperate/branch/master)
[![Tokei](https://tokei.rs/b1/github/cginternals/gloperate)](https://github.com/Aaronepower/tokei)


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
