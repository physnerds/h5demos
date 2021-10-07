cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(h5demos)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_XX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug) #change this to release for release build:
find_package(MPI REQUIRED)
find_package(ROOT REQUIRED COMPONENTS Core RIO Tree) 
find_package(HDF5 REQUIRED)

message("STATUS OF HDF5_FOUND IS ${HDF5_FOUND}")
set(HDF5_PREFER_PARALLEL TRUE)

message(STATUS "C++ compiler is : ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ compiler identification is : ${CMAKE_CXX_COMPILER_ID}")

include_directories (SYSTEM 
		    ${MPI_CXX_INCLUDE_DIRS}
		    ${MPI_C_INCLUDE_DIRS}
		    ${HDF5_INCLUDE_DIRS}
		    ${HDF5_CXX_INCLUDE_DIRS}
		    ${ROOT_INCLUDE_DIRS}
                    $ENV{HIGHFIVE_INC}
                    $ENV{DK2NU_INC})
		    

set (libraries ${libraries}
               ${ROOT_LIBRARIES}
	       ${MPI_CXX_LIBRARIES}
	       ${MPI_C_LIBRARIES}
	       ${HDF5_LIBRARIES}
	       ${HDF5_CXX_LIBRARIES}
               )






add_executable(ConvertHDF5_MPI_ver4 ConvertHDF5_MPI_ver4.cc)
target_compile_options(ConvertHDF5_MPI_ver4 PRIVATE ${HDF5_CXX_COMPILER_EXECUTABLE})
target_link_libraries(ConvertHDF5_MPI_ver4 ${libraries})

add_executable(test_parallel_hdf5 test_parallel_hdf5.cc)
target_compile_options(test_parallel_hdf5 PRIVATE ${HDF5_CXX_COMPILER_EXECUTABLE})
target_link_libraries(test_parallel_hdf5 ${libraries})

add_executable(read_h5_deserialize_MPI_1D read_h5_deserialize_MPI_1D.cc)
target_compile_options(read_h5_deserialize_MPI_1D PRIVATE ${HDF5_CXX_COMPILER_EXECUTABLE})
target_link_libraries(read_h5_deserialize_MPI_1D ${libraries})

