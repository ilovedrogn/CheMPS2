project (CheMPS2)

option (MKL "Compile using the MKL" OFF)

if(MKL)
    cmake_minimum_required (VERSION 2.8.11)
else(MKL)
    cmake_minimum_required (VERSION 2.8)
endif(MKL)

option (BUILD_DOCUMENTATION "Use Doxygen to create a HTML/PDF manual" OFF)
set (CMAKE_VERBOSE_MAKEFILE OFF)

find_package (OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif (OPENMP_FOUND)

include (CheckCXXCompilerFlag)

check_cxx_compiler_flag (-xHost HAS_XHOST)
check_cxx_compiler_flag (-march=native HAS_MARCH_NATIVE)

if (HAS_XHOST)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -xHost")
elseif (HAS_MARCH_NATIVE)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif ()

check_cxx_compiler_flag (-ipo HAS_IPO)

if (HAS_IPO)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ipo")
endif (HAS_IPO)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2")

find_package (HDF5 REQUIRED)

add_subdirectory (CheMPS2)
add_subdirectory (tests)

if (BUILD_DOCUMENTATION)
   find_package (Doxygen)
   if (NOT DOXYGEN_FOUND)
      message (FATAL_ERROR "Doxygen is needed to build the documentation. Please install it correctly.")
   endif (NOT DOXYGEN_FOUND)
   configure_file (${CheMPS2_SOURCE_DIR}/CheMPS2/Doxyfile.in ${CheMPS2_BINARY_DIR}/Doxyfile  @ONLY IMMEDIATE)
   add_custom_target (doc COMMAND ${DOXYGEN_EXECUTABLE} ${CheMPS2_BINARY_DIR}/Doxyfile SOURCES ${CheMPS2_BINARY_DIR}/Doxyfile)
endif (BUILD_DOCUMENTATION)

