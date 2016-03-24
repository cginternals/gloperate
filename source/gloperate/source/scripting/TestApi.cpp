
#include <gloperate/scripting/TestApi.h>

#include <globjects/base/baselogging.h>


namespace gloperate
{


TestApi::TestApi(const std::string & name, int value)
: reflectionzeug::Object(name)
, m_value(value)
{
    // Register properties
    addProperty<int>("value", this, &TestApi::value, &TestApi::setValue);

    // Register functions
    addFunction("sayHello", this, &TestApi::sayHello);
}

TestApi::~TestApi()
{
}

int TestApi::value() const
{
    globjects::info() << "value()";

    return m_value;
}

void TestApi::setValue(int v)
{
    globjects::info() << "setValue()";

    m_value = v;
}

void TestApi::sayHello(const std::string & msg, int count)
{
    int num = (count > 0) ? count : 1;

    for (int i=0; i<num; i++) {
        globjects::info() << msg;
    }
}


} // namespace gloperate
