cmake_minimum_required(VERSION 3.20)

project(cppParser)

#set()

## GOOGLE TEST REQUIRED CODE

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

## GOOGLE TEST REQUIRED END

add_library(
	cppParser 
	"src/tokenizer.cpp"
)
target_include_directories(
	cppParser 
	PRIVATE "include/"
)
#add_executable(
#	cppParser
#	
#	"src/main.cpp"
#	"src/tokenizer.cpp"
#)
#target_link_libraries(
#	cppParser 
#	PUBLIC TOKENIZER
#)


## TESTING 

enable_testing()

add_executable(
  tokenizer_test
   "src/tests/tokenizer_test.cpp")
target_link_libraries(
	tokenizer_test
	cppParser
	GTest::gtest_main
)
target_include_directories(
	tokenizer_test
	PRIVATE "include/"
)

include(GoogleTest)
gtest_discover_tests(tokenizer_test)
