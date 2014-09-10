#include <gloperate/util/StringSourceDecorator.h>
#include <cassert>


namespace gloperate
{


StringSourceDecorator::StringSourceDecorator(AbstractStringSource * source)
: m_internal(source)
{
    assert(source != nullptr);

	m_internal->registerListener(this);

    update();
}

StringSourceDecorator::~StringSourceDecorator()
{
	m_internal->deregisterListener(this);
}

void StringSourceDecorator::notifyChanged(const globjects::Changeable *)
{
	update();
    changed();
}

void StringSourceDecorator::update()
{
}


} // namespace gloperate
