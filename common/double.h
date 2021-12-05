#ifndef FONTUS_DOUBLE_H
#define FONTUS_DOUBLE_H

#include <limits>

namespace fontus {

template <typename T>
bool equals(const T& left, const T& right) {
    return std::abs(left - right) <= std::numeric_limits<T>::epsilon() *
      std::max(std::abs(left), std::abs(right));
}

} // fontus

#endif /* FONTUS_DOUBLE_H */
