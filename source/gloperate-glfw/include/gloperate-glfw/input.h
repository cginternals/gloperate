
#pragma once


#include <gloperate/input/constants.h>

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

/**
*  @brief
*    Convert GLFW modifier code into gloperate modifier code
*/
GLOPERATE_GLFW_API gloperate::KeyModifier fromGLFWModifier(int modifier);


} // namespace gloperate_glfw
