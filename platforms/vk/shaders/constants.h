#ifndef SRC_GRAPHICS_LIB_COMPUTE_RADIX_SORT_PLATFORMS_VK_SHADERS_CONSTANTS_H_
#define SRC_GRAPHICS_LIB_COMPUTE_RADIX_SORT_PLATFORMS_VK_SHADERS_CONSTANTS_H_

layout(constant_id = 0) const uint RS_FILL_WORKGROUP_SIZE               = 1;
layout(constant_id = 1) const uint RS_FILL_BLOCK_ROWS                   = 10;
layout(constant_id = 2) const uint RS_HISTOGRAM_WORKGROUP_SIZE          = 1;
layout(constant_id = 3) const uint RS_HISTOGRAM_SUBGROUP_SIZE           = 1;
layout(constant_id = 4) const uint RS_HISTOGRAM_BLOCK_ROWS              = 1;
layout(constant_id = 5) const uint RS_PREFIX_WORKGROUP_SIZE             = 1;
layout(constant_id = 6) const uint RS_PREFIX_SUBGROUP_SIZE              = 1;
layout(constant_id = 7) const uint RS_SCATTER_WORKGROUP_SIZE            = 1;
layout(constant_id = 8) const uint RS_SCATTER_SUBGROUP_SIZE             = 1;
layout(constant_id = 9) const uint RS_SCATTER_BLOCK_ROWS                = 1;
layout(constant_id = 10) const uint RS_SCATTER_ENABLE_BROADCAST_MATCH   = 0;
layout(constant_id = 11) const uint RS_HISTOGRAM_DISABLE_SMEM_HISTOGRAM = 0;
layout(constant_id = 12) const uint RS_SCATTER_DISABLE_REORDER          = 0;

#endif  // SRC_GRAPHICS_LIB_COMPUTE_RADIX_SORT_PLATFORMS_VK_SHADERS_CONSTANTS_H_
