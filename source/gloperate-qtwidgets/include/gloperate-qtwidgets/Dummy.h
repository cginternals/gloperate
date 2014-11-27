#pragma once

#include <QWidget>

#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>

class Ui_Dummy;

namespace gloperate_qtwidgets
{

class GLOPERATE_QTWIDGETS_API Dummy: public QWidget
{
    Q_OBJECT

public:
    Dummy(QWidget * parent = nullptr);
    virtual ~Dummy();

protected:
    const QScopedPointer<Ui_Dummy> m_ui;
};

} // namespace gloperate_qtwidgets
