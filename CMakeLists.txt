cmake_minimum_required (VERSION 3.10.2)

project (rbtree_test)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")


set (SRC

        rbtree.c
        ./test/test.c)

add_executable (test ${SRC})

