
# file(GLOB_RECURSE base_SRCS ./*.cc)
# add_library(zrpc_base ${base_SRCS} ./coroutine/coctx_swap.S)
# target_link_libraries(zrpc_base pthread rt)

file(GLOB_RECURSE zrpc_SRCS ./*.cc)
add_library(zrpc ${zrpc_SRCS} ./base/coroutine/coctx_swap.S)
target_link_libraries(zrpc pthread rt)


# add_subdirectory(base)
# file(GLOB zrpc_SRCS ./*.cc)
# add_library(zrpc ${zrpc_SRCS})
# target_link_libraries(zrpc zrpc_base)

