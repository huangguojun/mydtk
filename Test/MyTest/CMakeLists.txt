
#add_library(sum_integers sum_integers.cpp)
add_executable(mytest 
            mytest.cpp
    )

target_link_libraries(mytest PRIVATE ${QT_LIBRARIES} )


