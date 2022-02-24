#ifndef GBEMU_RAM_IMPL_H_
#define GBEMU_RAM_IMPL_H_

#include <vector>

#include "memory/ram.h"
#include "types/types.h"

namespace gbemu {

class RamImpl : public Ram {
 public:
  RamImpl(u32 size_) : size(size_) { data.resize(size); }
  virtual u8 read(u16 addr) const override;
  virtual void write(u16 addr, u8 value) override;

 private:
  u32 size;
  std::vector<u8> data;
};

}  // namespace gbemu

#endif  // GBEMU_RAM_IMPL_H_
