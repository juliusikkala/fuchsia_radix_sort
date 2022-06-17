// Copyright 2021 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//
// Define header for radix sort target.
//

#include "target.h"

//
//
//

const struct radix_sort_vk_target bifrost8_u64_target =
{
  .magic = RS_HEADER_MAGIC,

  .extensions.named = {

  },

  .features.named = {
    .shaderInt16                  = 1,
    .bufferDeviceAddress          = 1,
    .vulkanMemoryModel            = 1,
    .vulkanMemoryModelDeviceScope = 1,
  },

  .config = {
    .keyval_dwords         = 2,
    .disable_int64         = 1,

    .fill = {
      .workgroup_size_log2 = 3,
      .block_rows          = 32,
    },

    .histogram =  {
      .workgroup_size_log2 = 3,
      .subgroup_size_log2  = 3,
      .block_rows          = 22,
      .disable_smem_histogram = 1
    },

    .prefix =  {
      .workgroup_size_log2 = 7,
      .subgroup_size_log2  = 3
    },

    .scatter = {
      .workgroup_size_log2 = 6,
      .subgroup_size_log2  = 3,
      .block_rows          = 9,
      .enable_broadcast    = 1,
      .disable_reorder     = 1
    }
  }
};

//
//
//
