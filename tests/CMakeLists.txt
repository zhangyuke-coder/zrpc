
add_executable(mutex_test mutex_test.cc)
target_link_libraries(mutex_test zrpc)

add_executable(log_test log_test.cc)
target_link_libraries(log_test zrpc)


add_executable(coroutine_test coroutine_test.cc)
target_link_libraries(coroutine_test zrpc)