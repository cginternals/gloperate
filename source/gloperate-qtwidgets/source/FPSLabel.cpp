#include <gloperate-qtwidgets/FPSLabel.h>

#include <QLabel>
#include <QVBoxLayout>

#include <gloperate/tools/FrameCounter.h>


namespace gloperate_qtwidgets
{

FPSLabel::FPSLabel(const gloperate::FrameCounter* counter, QWidget* parent)
: QWidget(parent)
, m_frameCounter(counter)
, m_label(new QLabel(this))
{
    m_updateTimer.setInterval(250);
    m_updateTimer.setSingleShot(false);
    m_updateTimer.start();

    setLayout(new QVBoxLayout);
    layout()->addWidget(m_label);

    connect(&m_updateTimer, &QTimer::timeout, this, &FPSLabel::update);
}

void FPSLabel::update()
{
    m_label->setText(QString("%1 FPS").arg(m_frameCounter->computeFps(), 0, 'f', 1));
}

}  // namespace gloperate_qtwidgets
