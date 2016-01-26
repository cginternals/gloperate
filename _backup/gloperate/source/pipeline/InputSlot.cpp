
#include <gloperate/pipeline/InputSlot.h>

#include <iostream>


namespace gloperate 
{


void printIncompatibleMessage(
    const AbstractInputSlot * slot, 
    const std::string & typeName,                          
    const AbstractData & data) 
{
    std::cout
        << "Trying to connect incompatible type "
        << data.qualifiedName() << " (" << data.type() << ")"
        << " to "
        << slot->qualifiedName() << " (" << typeName << ")"
        << std::endl;
}


} // namespace gloperate
