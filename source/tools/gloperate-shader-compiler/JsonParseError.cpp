#include "JsonParseError.h"


JsonParseError::JsonParseError()
: JsonParseError(NoError)
{
}

JsonParseError::JsonParseError(Type type)
: JsonParseError(type, QString())
{
}

JsonParseError::JsonParseError(Type type, const QString & info)
: m_type(type)
, m_info(info)
{
}

JsonParseError::Type JsonParseError::type() const
{
    return m_type;
}

const QString & JsonParseError::info() const
{
    return m_info;
}

JsonParseError::operator bool() const
{
    return !(m_type == NoError);
}
