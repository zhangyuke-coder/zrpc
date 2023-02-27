add_subdirectory(base)
file(GLOB zrpc_SRCS ./*.cc)
add_library(zrpc ${zrpc_SRCS})
target_link_libraries(zrpc zrpc_base)
