/*
 * Copyright (c) 2006, 2021, Oracle and/or its affiliates. All rights reserved.
 * Copyright (c) 2021, Huawei Technologies Co., Ltd. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#include "precompiled.hpp"
#include "asm/register.hpp"
#include "runtime/os.hpp"
#include "runtime/os.inline.hpp"
#include "runtime/vm_version.hpp"

#include <sys/auxv.h>

uint32_t VM_Version::get_current_vector_length() {
  assert(_features & CPU_V, "should not call this");
//  return (uint32_t)read_csr(CSR_VLENB);

// https://lists.gnu.org/archive/html/qemu-riscv/2021-11/msg00386.html
//  csr_read(0xc22);
  return 0;
}

void VM_Version::get_os_cpu_info() {
  _features = 0;

  unsigned long auxv;
  if(elf_aux_info(AT_HWCAP, &auxv, sizeof(unsigned long)) != 0) {
    auxv = 0;
  }

  static_assert(CPU_I == HWCAP_ISA_I, "Flag CPU_I must follow Bsd HWCAP");
  static_assert(CPU_M == HWCAP_ISA_M, "Flag CPU_M must follow Bsd HWCAP");
  static_assert(CPU_A == HWCAP_ISA_A, "Flag CPU_A must follow Bsd HWCAP");
  static_assert(CPU_F == HWCAP_ISA_F, "Flag CPU_F must follow Bsd HWCAP");
  static_assert(CPU_D == HWCAP_ISA_D, "Flag CPU_D must follow Bsd HWCAP");
  static_assert(CPU_C == HWCAP_ISA_C, "Flag CPU_C must follow Bsd HWCAP");

  // RISC-V has four bit-manipulation ISA-extensions: Zba/Zbb/Zbc/Zbs.
  // Availability for those extensions could not be queried from HWCAP.
  // TODO: Add proper detection for those extensions.
  _features = auxv & (
      HWCAP_ISA_I |
      HWCAP_ISA_M |
      HWCAP_ISA_A |
      HWCAP_ISA_F |
      HWCAP_ISA_D |
      HWCAP_ISA_C);
}
