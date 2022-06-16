// Copyright 2021 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//
// Define header for radix sort target.
//

#include "config.h"
#include "target.h"
static
#include "gcn3_u32_init.comp.h"
static
#include "gcn3_u32_fill.comp.h"
static
#include "gcn3_u32_histogram.comp.h"
static
#include "gcn3_u32_prefix.comp.h"
static
#include "gcn3_u32_scatter_0_even.comp.h"
static
#include "gcn3_u32_scatter_0_odd.comp.h"

//
//
//

const struct radix_sort_vk_target gcn3_u32_target =
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
    .keyval_dwords         = RS_KEYVAL_DWORDS,

    .histogram =  {
      .workgroup_size_log2 = RS_HISTOGRAM_WORKGROUP_SIZE_LOG2,
      .subgroup_size_log2  = RS_HISTOGRAM_SUBGROUP_SIZE_LOG2,
      .block_rows          = RS_HISTOGRAM_BLOCK_ROWS
    },

    .prefix =  {
      .workgroup_size_log2 = RS_PREFIX_WORKGROUP_SIZE_LOG2,
      .subgroup_size_log2  = RS_PREFIX_SUBGROUP_SIZE_LOG2
    },

    .scatter = {
      .workgroup_size_log2 = RS_SCATTER_WORKGROUP_SIZE_LOG2,
      .subgroup_size_log2  = RS_SCATTER_SUBGROUP_SIZE_LOG2,
      .block_rows          = RS_SCATTER_BLOCK_ROWS
    }
  },

  .modules = {
    .module_count = 6,
    .module_size = {
      sizeof(init_comp_shader_binary),
      sizeof(fill_comp_shader_binary),
      sizeof(histogram_comp_shader_binary),
      sizeof(prefix_comp_shader_binary),
      sizeof(scatter_0_even_comp_shader_binary),
      sizeof(scatter_0_odd_comp_shader_binary),
      0,
      0
    },
    .module_data = {
      init_comp_shader_binary,
      fill_comp_shader_binary,
      histogram_comp_shader_binary,
      prefix_comp_shader_binary,
      scatter_0_even_comp_shader_binary,
      scatter_0_odd_comp_shader_binary,
      (void*)0,
      (void*)0
    }
  }
};

//
//
//
