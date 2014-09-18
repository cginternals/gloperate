#include <gloperate/pipelines/InputSlot.h>

#include <iostream>

namespace gloperate {

void printWritingOnMessage(const AbstractInputSlot* slot) {
    std::cout << "Trying to write on an input slot " << slot->asPrintable() << "." << std::endl;
}

void printIncompatibleMessage(const AbstractInputSlot* slot, const std::string& typeName,
                              const AbstractData & data) {
    std::cout
        << "Trying to connect incompatible type "
        << data.qualifiedName() << " (" << data.type() << ")"
        << " to "
        << slot->qualifiedName() << " (" << typeName << ")"
        << std::endl;
}

} // namespace gloperate
