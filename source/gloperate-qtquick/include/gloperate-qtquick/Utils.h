
#pragma once


#include <memory>
#include <string>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace gloperate
{
    class Environment;
    class AbstractCanvas;
    class Stage;
}


namespace gloperate_qtquick
{


/**
*  @brief
*    Utility function for qt quick and OpenGL integration
*/
class GLOPERATE_QTQUICK_API Utils
{
public:
    /**
    *  @brief
    *    Initialize glbinding and globjects in the current context
    */
    static void initContext();

    /**
    *  @brief
    *    Registers current context for globjects.
    *
    *  @remarks
    *    This function is necessary to handle the deletion of the QML objects
    *    From the main thread instead of the QSGRenderThread.
    */
    static void registerGlobjectsContext();

    /**
    *  @brief
    *    Set current context for globjects.
    *
    *  @remarks
    *    This function is necessary to handle the usage of OpenGL objects
    *    From event threads.
    */
    static void setGlobjectsContext();

    /**
    *  @brief
    *    Clear screen (call glClear)
    *
    *  @param[in] red
    *    Red color component (0..1)
    *  @param[in] green
    *    Green color component (0..1)
    *  @param[in] blue
    *    Blue color component (0..1)
    *  @param[in] alpha
    *    Alpha color component (0..1)
    *  @param[in] clearDepthBuffer
    *    If 'true', the depth buffer will also be cleared, otherwise only the color buffer
    */
    static void clearScreen(float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f, bool clearDepthBuffer = true);

    // [TODO] Only for testing
    static std::unique_ptr<gloperate::Stage> createRenderStage(gloperate::Environment * environment, const std::string & name);
    static std::unique_ptr<gloperate::AbstractCanvas> createCanvas(gloperate::Environment * environment, std::unique_ptr<gloperate::Stage> && renderStage);
};


} // namespace gloperate_qtquick
