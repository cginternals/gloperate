
#pragma once

#include <gloperate/viewer/input.h>

#include <gloperate-glfw/gloperate-glfw_api.h>


namespace gloperate_glfw
{


/**
*  @brief
*    Convert GLFW mouse button into gloperate mouse button
*/
GLOPERATE_GLFW_API gloperate::MouseButton fromGLFWMouseButton(int button);

/**
*  @brief
*    Convert GLFW key code into gloperate key code
*/
GLOPERATE_GLFW_API gloperate::Key fromGLFWKeyCode(int key);


} // namespace gloperate_glfw
