
file(GLOB_RECURSE base_SRCS ./*.cc)
add_library(zrpc_base ${base_SRCS} ./coroutine/coctx_swap.S)
target_link_libraries(zrpc_base pthread rt)
