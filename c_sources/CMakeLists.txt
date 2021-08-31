CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

project(ed247SimulinkAdapter)

set(CMAKE_CXX_STANDARD 11)
set(DEPS_DIR "${PROJECT_SOURCE_DIR}/../..")

OPTION(DEFINE_DEBUG
  "Build the project using debugging code"
  OFF)
IF(DEFINE_DEBUG)
	add_definitions(-DDEBUG)
ENDIF(DEFINE_DEBUG)

include_directories(
	"${PROJECT_SOURCE_DIR}/include"
	"${DEPS_DIR}/ED247_LIBRARY/_install/inc"
	)

link_directories(
	"${DEPS_DIR}/ED247_LIBRARY/_install/lib"
	)

IF(WIN32)
	include_directories("${DEPS_DIR}/libxml2/include")
	link_directories("${DEPS_DIR}/libxml2/lib")
ELSE(WIN32)
	include_directories("/usr/local/include/libxml2")
	link_directories("/usr/local/lib")
ENDIF(WIN32)

# Download and unpack googletest at configure time
configure_file(${DEPS_DIR}/googletest/CMakeLists.txt
		googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
		${CMAKE_BINARY_DIR}/googletest-build)

# The gtest/gmock targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
	include_directories("${gtest_SOURCE_DIR}/include"
			"${gmock_SOURCE_DIR}/include")
endif()

# TAP for GTest
include_directories(
	"${DEPS_DIR}/gtest-tap-listener/src"
	)

# Now simply link your own targets against gtest, gmock,
# etc. as appropriate  
enable_testing()

set(SOURCES 
		${PROJECT_SOURCE_DIR}/tests/ed247_cmd_xml_test.cpp 
		${PROJECT_SOURCE_DIR}/tests/ed247_interface_test.cpp 
		#${PROJECT_SOURCE_DIR}/tests/issues_test.cpp 
		${PROJECT_SOURCE_DIR}/tests/a429_test.cpp 
		#${PROJECT_SOURCE_DIR}/tests/a664_test.cpp 
		#${PROJECT_SOURCE_DIR}/tests/a825_test.cpp 
		#${PROJECT_SOURCE_DIR}/tests/nad_test.cpp 
		${PROJECT_SOURCE_DIR}/tests/testutils.cpp 
		${PROJECT_SOURCE_DIR}/tests/tools_test.cpp 
		${PROJECT_SOURCE_DIR}/tests/main.cpp
		)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} PUBLIC gtest)

add_test(NAME ${CMAKE_PROJECT_NAME} COMMAND ${CMAKE_PROJECT_NAME})

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_PROJECT_NAME}_lib)

# Google test requires the CMAKE_INSTALL_PREFIX, instead it use a default path which may do not exist
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}")
