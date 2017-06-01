
#pragma once


#include <QMouseEvent>

#include <glm/fwd.hpp>

#include <gloperate/input/constants.h>

#include <gloperate-qt/gloperate-qt_api.h>


class QImage;


namespace gloperate_qt
{


/**
*  @brief
*    Utility class to convert from Qt to gloperate data types
*/
class GLOPERATE_QT_API Converter
{
public:
    /**
    *  @brief
    *    Convert image into OpenGL compatible format
    *
    *  @param[in] image
    *    Input image
    *
    *  @return
    *    Image in GL format
    */
    static QImage convert(const QImage & image);

    /**
    *  @brief
    *    Convert position from QPoint to GLM vector
    *
    *  @param[in] point
    *    Position as QPoint
    *
    *  @return
    *    Position as 2D vector
    */
    static glm::ivec2 fromQPoint(const QPoint & point);

    /**
    *  @brief
    *    Convert mouse button ID
    *
    *  @param[in] point
    *    Qt mouse button ID
    *
    *  @return
    *    gloperate mouse button ID
    */
    static gloperate::MouseButton fromQtMouseButton(Qt::MouseButton button);

    /**
    *  @brief
    *    Convert key code
    *
    *  @param[in] key
    *    Qt key code
    *  @param[in] mods
    *    Qt key modifiers
    *
    *  @return
    *    gloperate key code
    */
    static gloperate::Key fromQtKeyCode(int key, int mods);

    /**
    *  @brief
    *    Convert key modifiers
    *
    *  @param[in] mods
    *    Qt key modifiers
    *
    *  @return
    *    gloperate key modifiers
    */
    static gloperate::KeyModifier fromQtModifiers(int mods);
};


} // namespace gloperate_qt
