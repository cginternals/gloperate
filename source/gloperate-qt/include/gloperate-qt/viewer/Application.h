
#pragma once

#include <memory>

#include <gloperate/ext-includes-begin.h>
#include <QApplication>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>


class QSettings;


namespace gloperate_qt
{


class GLOPERATE_QT_API Application : public QApplication
{
public:
    static std::unique_ptr<QSettings> settings();


public:
    Application(int & argc, char ** argv);
    virtual ~Application();


private:
    static Application * self;


private:
    QString m_settingsFilePath;
};


} // namespace gloperate_qt
