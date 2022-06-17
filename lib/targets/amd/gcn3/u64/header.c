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

const struct radix_sort_vk_target gcn3_u64_target =
{
  .magic = RS_HEADER_MAGIC,

  .extensions.named = {

  },

  .features.named = {
    .shaderInt16                  = 1,
    .shaderInt64                  = 1,
    .bufferDeviceAddress          = 1,
    .vulkanMemoryModel            = 1,
    .vulkanMemoryModelDeviceScope = 1,
  },

  .config = {
    .keyval_dwords         = 2,
    .disable_int64         = 0,

    .fill = {
      .workgroup_size_log2 = 7,
      .block_rows          = 8,
    },

    .histogram =  {
      .workgroup_size_log2 = 8,
      .subgroup_size_log2  = 6,
      .block_rows          = 14,
      .disable_smem_histogram = 0
    },

    .prefix =  {
      .workgroup_size_log2 = 8,
      .subgroup_size_log2  = 6
    },

    .scatter = {
      .workgroup_size_log2 = 8,
      .subgroup_size_log2  = 6,
      .block_rows          = 14,
      .enable_broadcast    = 0,
      .disable_reorder     = 0
    }
  }
};

//
//
//
