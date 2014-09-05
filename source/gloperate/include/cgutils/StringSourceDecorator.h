#pragma once

#include <globjects-base/ChangeListener.h>
#include <globjects-base/ref_ptr.h>
#include <globjects-base/AbstractStringSource.h>

#include <cgutils/cgutils_api.h>

namespace cgutils 
{

class CGUTILS_API StringSourceDecorator : public glo::AbstractStringSource, protected glo::ChangeListener
{
public:
    StringSourceDecorator(glo::AbstractStringSource * source);

    virtual void update();
protected:
    virtual ~StringSourceDecorator();

    virtual void notifyChanged(const Changeable * changeable) override;
protected:
    glo::ref_ptr<glo::AbstractStringSource> m_internal;
};

} // namespace gloutils
