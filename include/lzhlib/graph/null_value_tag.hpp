#ifndef LZHLIB_GRAPH_NULL_VALUE_TAG_HPP
#define LZHLIB_GRAPH_NULL_VALUE_TAG_HPP

namespace lzhlib
{
    // Tag that selects the specialisation of detail::vertex / detail::edge which stores no
    // value at all. It lives in its own header so that both can include it: while it was
    // defined in vertex.hpp, edge.hpp could only forward declare it, and a by value parameter
    // of an incomplete type is not allowed in a function definition, so edge.hpp did not
    // compile on its own.
    class null_value_tag
    {
        explicit null_value_tag() = default;
    };
}

#endif //LZHLIB_GRAPH_NULL_VALUE_TAG_HPP
