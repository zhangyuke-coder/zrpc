file(GLOB base_SRCS ./*.cc)
add_library(zrpc_base ${base_SRCS})
target_link_libraries(zrpc_base pthread rt)
# add_subdirectory(tests)