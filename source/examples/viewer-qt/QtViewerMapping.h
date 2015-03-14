#pragma once

#include <gloperate-qt/AbstractQtMapping.h>

#include <memory>

namespace gloperate {
    class CoordinateProvider;
    class WorldInHandNavigation;
    class TrackballNavigation;
}

class QtViewerMapping : public gloperate_qt::AbstractQtMapping
{
public:
    QtViewerMapping(gloperate_qt::QtOpenGLWindow * window);
    virtual ~QtViewerMapping();

    virtual void initializeTools() override;

protected:
    virtual void mapEvent(gloperate::AbstractEvent * event) override;
    virtual void mapEventWorld(gloperate::AbstractEvent * event);
    virtual void mapEventTrackball(gloperate::AbstractEvent * event);

protected:
    enum class NavigationType
    {
        WorldInHand,
        Trackball
    };

protected:
    std::unique_ptr<gloperate::WorldInHandNavigation> m_worldNavigation;
    std::unique_ptr<gloperate::TrackballNavigation> m_trackballNavigation;
    std::unique_ptr<gloperate::CoordinateProvider> m_coordProvider;
    NavigationType m_currentNavigation;
};
