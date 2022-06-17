#include "target.h"

#include "common/macros.h"
#include "common/util.h"
#include "radix_sort/radix_sort_vk.h"
static
#include "build/i64_u32_init.comp.h"
  static
#include "build/i64_u64_init.comp.h"
  static
#include "build/noi64_u32_init.comp.h"
  static
#include "build/noi64_u64_init.comp.h"
  static
#include "build/i64_u32_fill.comp.h"
  static
#include "build/i64_u64_fill.comp.h"
  static
#include "build/noi64_u32_fill.comp.h"
  static
#include "build/noi64_u64_fill.comp.h"
  static
#include "build/i64_u32_histogram.comp.h"
  static
#include "build/i64_u64_histogram.comp.h"
  static
#include "build/noi64_u32_histogram.comp.h"
  static
#include "build/noi64_u64_histogram.comp.h"
  static
#include "build/i64_u32_prefix.comp.h"
  static
#include "build/i64_u64_prefix.comp.h"
  static
#include "build/noi64_u32_prefix.comp.h"
  static
#include "build/noi64_u64_prefix.comp.h"
  static
#include "build/i64_u32_scatter.comp.h"
  static
#include "build/i64_u64_scatter.comp.h"
  static
#include "build/noi64_u32_scatter.comp.h"
  static
#include "build/noi64_u64_scatter.comp.h"

  extern const struct radix_sort_vk_target gcn3_u32_target;
extern const struct radix_sort_vk_target   gcn3_u64_target;
extern const struct radix_sort_vk_target   bifrost4_u32_target;
extern const struct radix_sort_vk_target   bifrost4_u64_target;
extern const struct radix_sort_vk_target   bifrost8_u32_target;
extern const struct radix_sort_vk_target   bifrost8_u64_target;
extern const struct radix_sort_vk_target   gen8_u32_target;
extern const struct radix_sort_vk_target   gen8_u64_target;
extern const struct radix_sort_vk_target   sm35_u32_target;
extern const struct radix_sort_vk_target   sm35_u64_target;

radix_sort_vk_target_t
radix_sort_vk_target_auto_detect(VkPhysicalDeviceProperties const *         props,
                                 VkPhysicalDeviceSubgroupProperties const * subgroup_props,
                                 uint32_t                                   keyval_dwords)
{
  uint32_t vendor_id = props->vendorID;
  uint32_t device_id = props->deviceID;
  // Defensive default target.
  radix_sort_vk_target_t target = {
    .magic = RS_HEADER_MAGIC,

    .extensions.named = {},

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
        .workgroup_size_log2 = 2,
        .block_rows          = 32,
      },

      .histogram = {
        .workgroup_size_log2 = 2,
        .subgroup_size_log2  = 2,
        .block_rows          = 11,
        .disable_smem_histogram = 1
      },

      .prefix = {
        .workgroup_size_log2 = 7,
        .subgroup_size_log2  = 2
      },

      .scatter = {
        .workgroup_size_log2 = 5,
        .subgroup_size_log2  = 2,
        .block_rows          = 7,
        .enable_broadcast    = 0,
        .disable_reorder     = 1
      }
    }
  };

  // Find vendor-specific tuned values.
  switch (vendor_id)
    {
      case 0x10DE:
        //
        // NVIDIA
        //
        switch (keyval_dwords)
          {
            case 1:
              target = sm35_u32_target;
              break;
            case 2:
              target = sm35_u64_target;
              break;
            default:
              break;
          }
        break;

      case 0x1002:
        //
        // AMD
        //
        switch (keyval_dwords)
          {
            case 1:
              target = gcn3_u32_target;
              break;
            case 2:
              target = gcn3_u64_target;
              break;
            default:
              break;
          }
        break;

      case 0x8086:
        //
        // INTEL
        //
        switch (keyval_dwords)
          {
            case 1:
              target = gen8_u32_target;
              break;
            case 2:
              target = gen8_u64_target;
              break;
            default:
              break;
          }
        break;

      case 0x13B5:
        //
        // ARM MALI
        //
        switch (device_id)
          {
            case 0x70930000:
              //
              // BIFROST4
              //
              switch (keyval_dwords)
                {
                  case 1:
                    target = bifrost4_u32_target;
                    break;
                  case 2:
                    target = bifrost4_u64_target;
                    break;
                  default:
                    break;
                }

            case 0x72120000:
              //
              // BIFROST8
              //
              switch (keyval_dwords)
                {
                  case 1:
                    target = bifrost8_u32_target;
                    break;
                  case 2:
                    target = bifrost8_u64_target;
                    break;
                  default:
                    break;
                }
          }
        break;
      default:
        break;
    }

  if (subgroup_props)
    {
      uint32_t subgroup_log2                     = msb_idx_u32(subgroup_props->subgroupSize);
      target.config.histogram.subgroup_size_log2 = subgroup_log2;
      target.config.prefix.subgroup_size_log2    = subgroup_log2;
      target.config.scatter.subgroup_size_log2   = subgroup_log2;
      target.config.init.workgroup_size_log2 =
        MAX_MACRO(uint32_t, target.config.init.workgroup_size_log2, subgroup_log2);
      target.config.fill.workgroup_size_log2 =
        MAX_MACRO(uint32_t, target.config.fill.workgroup_size_log2, subgroup_log2);
      target.config.histogram.workgroup_size_log2 =
        MAX_MACRO(uint32_t, target.config.histogram.workgroup_size_log2, subgroup_log2);
      target.config.prefix.workgroup_size_log2 =
        MAX_MACRO(uint32_t, target.config.prefix.workgroup_size_log2, subgroup_log2);
      target.config.scatter.workgroup_size_log2 =
        MAX_MACRO(uint32_t, target.config.scatter.workgroup_size_log2, subgroup_log2);
    }
  return target;
}

const struct radix_sort_vk_target_modules radix_sort_u32_modules_i64 = {
  .module_count = 6,
  .module_size  = { sizeof(i64_u32_init_comp_shader_binary),
                    sizeof(i64_u32_fill_comp_shader_binary),
                    sizeof(i64_u32_histogram_comp_shader_binary),
                    sizeof(i64_u32_prefix_comp_shader_binary),
                    sizeof(i64_u32_scatter_comp_shader_binary),
                    sizeof(i64_u32_scatter_comp_shader_binary),
                    0,
                    0 },
  .module_data  = { i64_u32_init_comp_shader_binary,
                    i64_u32_fill_comp_shader_binary,
                    i64_u32_histogram_comp_shader_binary,
                    i64_u32_prefix_comp_shader_binary,
                    i64_u32_scatter_comp_shader_binary,
                    i64_u32_scatter_comp_shader_binary,
                    (void *)0,
                    (void *)0 }
};

const struct radix_sort_vk_target_modules radix_sort_u64_modules_i64 = {
  .module_count = 8,
  .module_size  = { sizeof(i64_u64_init_comp_shader_binary),
                    sizeof(i64_u64_fill_comp_shader_binary),
                    sizeof(i64_u64_histogram_comp_shader_binary),
                    sizeof(i64_u64_prefix_comp_shader_binary),
                    sizeof(i64_u64_scatter_comp_shader_binary),
                    sizeof(i64_u64_scatter_comp_shader_binary),
                    sizeof(i64_u64_scatter_comp_shader_binary),
                    sizeof(i64_u64_scatter_comp_shader_binary) },
  .module_data  = { i64_u64_init_comp_shader_binary,
                    i64_u64_fill_comp_shader_binary,
                    i64_u64_histogram_comp_shader_binary,
                    i64_u64_prefix_comp_shader_binary,
                    i64_u64_scatter_comp_shader_binary,
                    i64_u64_scatter_comp_shader_binary,
                    i64_u64_scatter_comp_shader_binary,
                    i64_u64_scatter_comp_shader_binary }
};

const struct radix_sort_vk_target_modules radix_sort_u32_modules_noi64 = {
  .module_count = 6,
  .module_size  = { sizeof(noi64_u32_init_comp_shader_binary),
                    sizeof(noi64_u32_fill_comp_shader_binary),
                    sizeof(noi64_u32_histogram_comp_shader_binary),
                    sizeof(noi64_u32_prefix_comp_shader_binary),
                    sizeof(noi64_u32_scatter_comp_shader_binary),
                    sizeof(noi64_u32_scatter_comp_shader_binary),
                    0,
                    0 },
  .module_data  = { noi64_u32_init_comp_shader_binary,
                    noi64_u32_fill_comp_shader_binary,
                    noi64_u32_histogram_comp_shader_binary,
                    noi64_u32_prefix_comp_shader_binary,
                    noi64_u32_scatter_comp_shader_binary,
                    noi64_u32_scatter_comp_shader_binary,
                    (void *)0,
                    (void *)0 }
};

const struct radix_sort_vk_target_modules radix_sort_u64_modules_noi64 = {
  .module_count = 8,
  .module_size  = { sizeof(noi64_u64_init_comp_shader_binary),
                    sizeof(noi64_u64_fill_comp_shader_binary),
                    sizeof(noi64_u64_histogram_comp_shader_binary),
                    sizeof(noi64_u64_prefix_comp_shader_binary),
                    sizeof(noi64_u64_scatter_comp_shader_binary),
                    sizeof(noi64_u64_scatter_comp_shader_binary),
                    sizeof(noi64_u64_scatter_comp_shader_binary),
                    sizeof(noi64_u64_scatter_comp_shader_binary) },
  .module_data  = { noi64_u64_init_comp_shader_binary,
                    noi64_u64_fill_comp_shader_binary,
                    noi64_u64_histogram_comp_shader_binary,
                    noi64_u64_prefix_comp_shader_binary,
                    noi64_u64_scatter_comp_shader_binary,
                    noi64_u64_scatter_comp_shader_binary,
                    noi64_u64_scatter_comp_shader_binary,
                    noi64_u64_scatter_comp_shader_binary }
};
