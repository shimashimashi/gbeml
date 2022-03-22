#include "core/memory/ram_impl.h"

#include <glog/logging.h>

namespace gbeml {

u8 RamImpl::read(u16 addr) const {
  DCHECK(addr < size);
  return data[addr];
}

void RamImpl::write(u16 addr, u8 value) {
  DCHECK(addr < size);
  data[addr] = value;
}

}  // namespace gbeml
