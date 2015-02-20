#pragma once

#include <QWidget>
#include <QTimer>

#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>


class QLabel;

namespace gloperate
{
class FrameCounter;
}

namespace gloperate_qtwidgets
{

class GLOPERATE_QTWIDGETS_API FPSLabel : public QWidget
{
    Q_OBJECT

public:
    explicit FPSLabel(const gloperate::FrameCounter* counter = nullptr, QWidget* parent = nullptr);

public slots:
    void update();

private:
    const gloperate::FrameCounter* m_frameCounter;
    QTimer m_updateTimer;
    QLabel* m_label;
};

}  // namespace gloperate_qtwidgets
