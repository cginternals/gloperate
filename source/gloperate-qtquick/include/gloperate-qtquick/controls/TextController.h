
#pragma once


#include <QQuickItem>


class QTextDocument;
class QSyntaxHighlighter;


namespace gloperate_qtquick
{


/**
*  @brief
*    Qt quick item to extend options for TextEdit (e.g., syntax highlighting)
*/
class TextController : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* target READ target WRITE setTarget)


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] parent
    *    Parent item (can be null)
    */
    TextController(QQuickItem * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~TextController();

    /**
    *  @brief
    *    Get target
    *
    *  @return
    *    Target TextEdit item (can be null)
    */
    QQuickItem * target() const;

    /**
    *  @brief
    *    Set target
    *
    *  @param[in] target
    *    Target TextEdit item (can be null)
    */
    void setTarget(QQuickItem * target);


protected:
    /**
    *  @brief
    *    Destroy all previous data
    */
    void cleanUp();


protected:
    QQuickItem         * m_target;       ///< Target TextEdit item that is extended (can be null)
    QTextDocument      * m_textDocument; ///< Text document of that item (can be null)
    QSyntaxHighlighter * m_highlighter;  ///< Syntax highlighter (can be null)
};


} // namespace gloperate_qtquick
