#include "radix_sort/platforms/vk/radix_sort_vk.h"

extern const struct radix_sort_vk_target gcn3_u32_target;
extern const struct radix_sort_vk_target gcn3_u64_target;
extern const struct radix_sort_vk_target bifrost4_u32_target;
extern const struct radix_sort_vk_target bifrost4_u64_target;
extern const struct radix_sort_vk_target bifrost8_u32_target;
extern const struct radix_sort_vk_target bifrost8_u64_target;
extern const struct radix_sort_vk_target gen8_u32_target;
extern const struct radix_sort_vk_target gen8_u64_target;
extern const struct radix_sort_vk_target sm35_u32_target;
extern const struct radix_sort_vk_target sm35_u64_target;

radix_sort_vk_target_t const *
radix_sort_vk_target_auto_detect(VkPhysicalDeviceProperties const * props,
                                 uint32_t                           keyval_dwords)
{
  uint32_t vendor_id = props->vendorID;
  uint32_t device_id = props->deviceID;
  switch (vendor_id)
    {
      case 0x10DE:
      default:
        //
        // NVIDIA
        //
        switch (keyval_dwords)
          {
            case 1:
              return &sm35_u32_target;
            case 2:
              return &sm35_u64_target;
          }

      case 0x1002:
        //
        // AMD
        //
        switch (keyval_dwords)
          {
            case 1:
              return &gcn3_u32_target;
            case 2:
              return &gcn3_u64_target;
          }

      case 0x8086:
        //
        // INTEL
        //
        switch (keyval_dwords)
          {
            case 1:
              return &gen8_u32_target;
            case 2:
              return &gen8_u64_target;
          }

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
                    return &bifrost4_u32_target;
                  case 2:
                    return &bifrost4_u64_target;
                }

            case 0x72120000:
            default:
              //
              // BIFROST8
              //
              switch (keyval_dwords)
                {
                  case 1:
                    return &bifrost8_u32_target;
                  case 2:
                    return &bifrost8_u64_target;
                }
          }
    }
  return NULL;
}
