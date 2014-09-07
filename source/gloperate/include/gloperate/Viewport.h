#pragma once


#include <globjects-base/Referenced.h>
#include <gloperate/gloperate_api.h>


namespace gloperate
{


class GLOPERATE_API Viewport
{


public:
    Viewport();
    Viewport(int width, int height);
    Viewport(int x, int y, int width, int height);
    Viewport(const Viewport & viewport);
    ~Viewport();

    Viewport & operator =(const Viewport & viewport);
    bool operator ==(const Viewport & viewport) const;

    int x() const;
    void setX(int x);

    int y() const;
    void setY(int y);

    int width() const;
    void setWidth(int width);

    int height() const;
    void setHeight(int height);

    void setViewport(int x, int y, int width, int height);


protected:
    int m_x;
    int m_y;
    int m_width;
    int m_height;


};


} // namespace gloperate
