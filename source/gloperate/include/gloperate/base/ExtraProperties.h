
#pragma once


#include <cppassist/io/FilePath.h>

#include <cppexpose/typed/GetTyped.h>


namespace cppexpose
{


/**
*  @brief
*    Filename type
*/
template <typename BASE>
class TypedFilename : public cppexpose::Typed<cppassist::FilePath, BASE>
{
public:
    TypedFilename()
    {
    }

    virtual ~TypedFilename()
    {
    }

    bool isString() const override
    {
        return true;
    }

    std::string toString() const override
    {
        return this->value().path();
    }

    bool fromString(const std::string & string) override
    {
        this->setValue(cppassist::FilePath(string));
        return true;
    }

    Variant toVariant() const override
    {
        return Variant::fromValue<std::string>(this->toString());
    }

    bool fromVariant(const Variant & variant) override
    {
        this->fromString(variant.value<std::string>());
        return true;
    }

    virtual std::string typeName() const override
    {
        return "filename";
    }
};

template <typename BASE>
struct GetTyped<cppassist::FilePath, BASE>
{
    using Type = TypedFilename<BASE>;
};


} // namespace cppexpose
