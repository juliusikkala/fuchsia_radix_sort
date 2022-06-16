cmake_minimum_required(VERSION 3.16)

project(RadixSort LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)

function(shader header source source_list include_path)
    get_filename_component(shader_bin_name ${source} NAME)
    string(REPLACE "\." "_" shader_bin_name ${shader_bin_name})
    add_custom_command(
        OUTPUT ${header}
        COMMAND glslangValidator --target-env vulkan1.2 --vn ${shader_bin_name}_shader_binary ${CMAKE_CURRENT_SOURCE_DIR}/${source} -o ${CMAKE_CURRENT_BINARY_DIR}/${header} -I${include_path} -Os
        MAIN_DEPENDENCY ${source}
        DEPENDS ${source}
        IMPLICIT_DEPENDS C ${source}
        VERBATIM
    )
    set(${source_list} ${${source_list}} ${header} PARENT_SCOPE)
endfunction()

set(targets
    amd/gcn3
    arm/bifrost4
    arm/bifrost8
    intel/gen8
    nvidia/sm35
)

foreach(target ${targets})
    foreach(keyval IN ITEMS u32 u64)
        get_filename_component(arch ${target} NAME)
        set(a ${arch}_${keyval}_)
        set(i "${CMAKE_CURRENT_SOURCE_DIR}/platforms/vk/targets/vendors/${target}/${keyval}")
        shader(${a}init.comp.h platforms/vk/shaders/init.comp shader_binary ${i})
        shader(${a}fill.comp.h platforms/vk/shaders/fill.comp shader_binary ${i})
        shader(${a}histogram.comp.h platforms/vk/shaders/histogram.comp shader_binary ${i})
        shader(${a}prefix.comp.h platforms/vk/shaders/prefix.comp shader_binary ${i})
        shader(${a}scatter_0_even.comp.h platforms/vk/shaders/scatter_0_even.comp shader_binary ${i})
        shader(${a}scatter_0_odd.comp.h platforms/vk/shaders/scatter_0_odd.comp shader_binary ${i})
        shader(${a}scatter_1_even.comp.h platforms/vk/shaders/scatter_1_even.comp shader_binary ${i})
        shader(${a}scatter_1_odd.comp.h platforms/vk/shaders/scatter_1_odd.comp shader_binary ${i})
        set(target_sources ${target_sources} ${i}/header.c)
    endforeach()
endforeach()

add_library(vk-radix-sort STATIC
    platforms/vk/radix_sort_vk.c
    platforms/vk/radix_sort_vk_ext.h
    platforms/vk/shaders/push.h
    platforms/vk/target.h
    platforms/vk/target.c
    platforms/vk/target_requirements.c
    platforms/vk/target_requirements.h
    common/vk/assert.c
    common/vk/barrier.c
    common/util.c
    ${target_sources}
    ${shader_binary}
)

target_link_libraries(vk-radix-sort PUBLIC Vulkan::Vulkan)
target_include_directories(vk-radix-sort PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/platforms/vk ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(vk-radix-sort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/platforms/vk/include)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if(BUILD_TESTING)
        enable_language(CXX)
        set(common_src
            platforms/vk/tests/radix_sort_vk_bench/cpu_sort.cc
            platforms/vk/tests/radix_sort_vk_bench/radix_sort_vk_bench.c
            platforms/vk/tests/radix_sort_vk_bench/find_target_name.c
            common/vk/assert.c
            common/vk/barrier.c
            common/vk/debug_utils.c
            common/vk/find_mem_type_idx.c
            common/vk/pipeline_cache.c
            common/util.c
        )
        add_executable(benchmark platforms/vk/tests/radix_sort_vk_bench/main.c ${common_src})
        target_link_libraries(benchmark PUBLIC vk-radix-sort)
        target_include_directories(benchmark PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
        target_compile_definitions(benchmark PUBLIC RS_VK_TARGET_ARCHIVE_LINKABLE)

        include(GoogleTest)

        add_executable(unittest platforms/vk/tests/radix_sort_vk_bench/tests/radix_sort_vk_bench_unittests.cc ${common_src})
        target_link_libraries(unittest PUBLIC vk-radix-sort gtest gtest_main)
        target_include_directories(unittest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} platforms/vk/tests/radix_sort_vk_bench)
        target_compile_definitions(unittest PUBLIC RS_VK_TARGET_ARCHIVE_LINKABLE)

        gtest_add_tests(unittest "" AUTO)
    endif()
endif()

