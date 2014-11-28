#pragma once

#include <gloperate-qt/AbstractQtMapping.h>

#include <memory>

namespace gloperate {
    class CoordinateProvider;
    class WorldInHandNavigation;
}

using namespace gloperate;
using namespace gloperate_qt;

class QtViewerMapping : public AbstractQtMapping
{
public:
    QtViewerMapping(QtOpenGLWindow * window);
    virtual ~QtViewerMapping();

    virtual void initializeTools() override;

protected:
    virtual void mapEvent(AbstractEvent * event) override;

protected:
    std::unique_ptr<WorldInHandNavigation> m_navigation;
    std::unique_ptr<CoordinateProvider> m_coordProvider;
};
