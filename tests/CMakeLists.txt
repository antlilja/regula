include(FetchContent)
FetchContent_Declare(catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.11.1
)

FetchContent_MakeAvailable(catch2)

add_executable(regula_tests
    src/main.cpp

    src/pattern.cpp
    src/anyof.cpp
)

target_include_directories(regula_tests PRIVATE include/)

target_link_libraries(regula_tests
	  PRIVATE
		    regula::regula
		    Catch2::Catch2
)