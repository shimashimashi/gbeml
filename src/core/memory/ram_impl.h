#ifndef GBEML_RAM_IMPL_H_
#define GBEML_RAM_IMPL_H_

#include <vector>

#include "core/memory/ram.h"
#include "core/types/types.h"

namespace gbeml {

class RamImpl : public Ram {
 public:
  RamImpl(u32 size_) : size(size_) { data.resize(size); }
  virtual u8 read(u16 addr) const override;
  virtual void write(u16 addr, u8 value) override;

 private:
  u32 size;
  std::vector<u8> data;
};

}  // namespace gbeml

#endif  // GBEML_RAM_IMPL_H_
