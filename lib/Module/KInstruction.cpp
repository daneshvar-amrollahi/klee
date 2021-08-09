//===-- KInstruction.cpp --------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "klee/Internal/Module/KInstruction.h"
#include <string>

using namespace llvm;
using namespace klee;

/***/
KInstruction::KInstruction()
  : total_accessed_offsets(0) { }

KInstruction::~KInstruction() {
  delete[] operands;
}

std::string KInstruction::getSourceLocation() const {
  if (!info->file.empty())
    return info->file + ":" + std::to_string(info->line) + " " +
           std::to_string(info->column);
  else return "[no debug info]";
}

/// leak-amt
bool KInstruction::logObjectAccess(const ObjectState* os, unsigned offset) {
  auto accessed_os_kvp = this->accessed_object_offsets.find(os);

  if (accessed_os_kvp == this->accessed_object_offsets.end()) {
    this->accessed_object_offsets.insert({ os, {} });
    accessed_os_kvp = this->accessed_object_offsets.find(os);
    accessed_os_kvp->second.insert(offset);
  
    this->total_accessed_offsets++;

    return true;
  }

  auto accessed_os = accessed_os_kvp->second;

  if (accessed_os.find(offset) == accessed_os.end()) {
    accessed_os.insert(offset);

    this->total_accessed_offsets++;
  
    return true;
  }

  return false;
}