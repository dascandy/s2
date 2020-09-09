#pragma once

namespace s2 {

template <typename A>
A& min(A& a, A& b) { return (a < b) ? a : b; }
template <typename A>
const A& min(const A& a, const A& b) { return (a < b) ? a : b; }

}
