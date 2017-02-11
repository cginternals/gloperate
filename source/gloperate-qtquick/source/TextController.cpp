
#include <gloperate-qtquick/TextController.h>

#include <QQuickTextDocument>
#include <QTextDocument>
#include <QDebug>

#include <gloperate-qt/scripting/ECMA26251SyntaxHighlighter.h>


using namespace gloperate_qt;


namespace gloperate_qtquick
{


TextController::TextController(QQuickItem * parent)
: QQuickItem(parent)
, m_target(nullptr)
, m_textDocument(nullptr)
, m_highlighter(nullptr)
{
}

TextController::~TextController()
{
    cleanUp();
}

const QQuickItem * TextController::target() const
{
    return m_target;
}

QQuickItem * TextController::target()
{
    return m_target;
}

void TextController::setTarget(QQuickItem * target)
{
    // Destroy old data
    cleanUp();

    // Set new target
    m_target = target;
    if (!m_target) {
        return;
    }

    // Get text document
    QVariant value = m_target->property("textDocument");
    if (value.canConvert<QQuickTextDocument*>())
    {
        QQuickTextDocument * textDocument = value.value<QQuickTextDocument*>();
        if (textDocument)
        {
            // Store text document
            m_textDocument = textDocument->textDocument();

            // Create syntax highlighter
            m_highlighter = new ECMA26251SyntaxHighlighter;
            m_highlighter->setDocument(m_textDocument);
        }
    }
}

void TextController::cleanUp()
{
    // Destroy syntax highlighter
    if (m_highlighter) {
        delete m_highlighter;
        m_highlighter = nullptr;
    }

    // Reset stored data
    m_textDocument = nullptr;
}


} // namespace gloperate_qtquick
