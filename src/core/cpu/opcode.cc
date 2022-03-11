#include "core/cpu/opcode.h"

#include <cstdio>

namespace gbeml {

u8 Opcode::get() const { return value.get(); }

u8 Opcode::slice(u8 from, u8 to) const { return value.slice(from, to); }

}  // namespace gbeml
