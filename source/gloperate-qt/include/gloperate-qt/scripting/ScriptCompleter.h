
#pragma once


#include <QCompleter>

#include <gloperate-qt/gloperate-qt_api.h>


class QStandardItemModel;
class QString;
class QStringList;


namespace gloperate_qt
{


/**
*  @brief
*    Base class for text completion
*/
class GLOPERATE_QT_API ScriptCompleter : public QCompleter
{
public:
    /**
    *  @brief
    *    Constructor
    */
    ScriptCompleter();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ScriptCompleter();

    /**
    *  @brief
    *    Add word to auto-completion
    *
    *  @param[in] word
    *    Word to add
    */
    void registerWord(const QString & word);

    /**
    *  @brief
    *    Remove word from auto-completion
    *
    *  @param[in] word
    *    Word to remove
    */
    void deregisterWord(const QString & word);

    /**
    *  @brief
    *    Add words to auto-completion
    *
    *  @param[in] word
    *    List of words to add
    */
    void registerWords(const QStringList & words);

    /**
    *  @brief
    *    Remove words from auto-completion
    *
    *  @param[in] word
    *    List of words to remove
    */
    void deregisterWords(const QStringList & words);


protected:
    QStandardItemModel * m_model; ///< Qt item model that contains the word list
};


} // namespace gloperate_qt
