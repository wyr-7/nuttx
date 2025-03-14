/****************************************************************************
 * arch/arm64/src/common/pgalloc.h
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM64_SRC_COMMON_PGALLOC_H
#define __ARCH_ARM64_SRC_COMMON_PGALLOC_H

#ifdef CONFIG_MM_PGALLOC

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <string.h>

#include "addrenv.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_ARCH_PGPOOL_MAPPING
#  error "ARM64 needs CONFIG_ARCH_PGPOOL_MAPPING"
#endif

/****************************************************************************
 * Public Functions Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: arm64_pgvaddr
 *
 * Description:
 *   Get virtual address for pgpool physical address. Note: this function
 *   is minimalistic and is only usable for kernel mappings and only tests
 *   if the paddr is in the pgpool. For user mapped addresses this does not
 *   work.
 *
 * Note:
 *   To get it to work with user addresses, a manual table walk needs to be
 *   implemented. Not too complex, but not needed for anything -> not
 *   implemented.
 *
 * Input Parameters:
 *   paddr - Physical pgpool address
 *
 * Return:
 *   vaddr - Virtual address for physical address
 *
 ****************************************************************************/

#ifdef CONFIG_ARCH_PGPOOL_MAPPING
static inline uintptr_t arm64_pgvaddr(uintptr_t paddr)
{
  if (paddr >= CONFIG_ARCH_PGPOOL_PBASE && paddr < CONFIG_ARCH_PGPOOL_PEND)
    {
      return paddr - CONFIG_ARCH_PGPOOL_PBASE + CONFIG_ARCH_PGPOOL_VBASE;
    }
  else if (paddr >= CONFIG_RAM_START && paddr < CONFIG_RAM_END)
    {
      return paddr - CONFIG_RAM_START + CONFIG_RAM_VSTART;
    }

  return 0;
}
#endif /* CONFIG_ARCH_PGPOOL_MAPPING */

/****************************************************************************
 * Name: arm64_uservaddr
 *
 * Description:
 *   Return true if the virtual address, vaddr, lies in the user address
 *   space.
 *
 ****************************************************************************/

static inline bool arm64_uservaddr(uintptr_t vaddr)
{
  /* Check if this address is within the range of the virtualized .bss/.data,
   * heap, or stack regions.
   */

  return ((vaddr >= ARCH_ADDRENV_VBASE && vaddr < ARCH_ADDRENV_VEND)
#ifdef CONFIG_ARCH_VMA_MAPPING
       || (vaddr >= CONFIG_ARCH_SHM_VBASE && vaddr < ARCH_SHM_VEND)
#endif
      );
}

/****************************************************************************
 * Name: arm64_pgwipe
 *
 * Description:
 *   Wipe a page of physical memory, first mapping it into virtual memory.
 *
 * Input Parameters:
 *   paddr - Physical address of page
 *
 ****************************************************************************/

static inline void arm64_pgwipe(uintptr_t paddr)
{
  uintptr_t vaddr = arm64_pgvaddr(paddr);
  memset((void *)vaddr, 0, MM_PGSIZE);
}

#endif /* CONFIG_MM_PGALLOC */
#endif /* __ARCH_ARM64_SRC_COMMON_PGALLOC_H */
