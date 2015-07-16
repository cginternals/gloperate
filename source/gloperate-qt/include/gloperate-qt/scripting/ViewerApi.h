
#pragma once


#include <reflectionzeug/Object.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt {


class Viewer;


/**
*  @brief
*    Viewer functions for scripting interface
*/
class GLOPERATE_QT_API ViewerApi : public reflectionzeug::Object
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewer
    *    Pointer to the main viewer window
    */
    ViewerApi(Viewer * viewer);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ViewerApi();

    // Viewer API
    void loadPainter(const std::string & name);


protected:
    Viewer * m_viewer;  ///< Pointer to the main viewer window
};


} // namespace gloperate_qt
