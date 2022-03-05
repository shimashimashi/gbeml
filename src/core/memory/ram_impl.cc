#include "core/memory/ram_impl.h"

#include "cassert"

namespace gbeml {

u8 RamImpl::read(u16 addr) const {
  assert(addr < size);
  return data[addr];
}

void RamImpl::write(u16 addr, u8 value) {
  assert(addr < size);
  data[addr] = value;
}

}  // namespace gbeml
