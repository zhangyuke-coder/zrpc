
add_executable(mutex_test mutex_test.cc)
target_link_libraries(mutex_test zrpc)

add_executable(log_test log_test.cc)
target_link_libraries(log_test zrpc)


add_executable(coroutine_test coroutine_test.cc)
target_link_libraries(coroutine_test zrpc)


add_executable(loop_test loop_test.cc)
target_link_libraries(loop_test zrpc)

add_executable(config_test config_test.cc)
target_link_libraries(config_test zrpc)
