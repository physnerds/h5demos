cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(h5demos CXX C)
set(CMAKE_C_FLAGS $ENV{CFLAGS})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_BUILD_TYPE Debug) #change this to release for release build
set(H5PCC_CXX_COMPILE_FLAG "/Users/abashyal/opt/miniconda3/envs/argonne_stuff/bin/h5pcc")
find_package(MPI REQUIRED)
find_package(ROOT REQUIRED COMPONENTS)
find_package(HDF5 REQUIRED)

message("STATUS OF HDF5_FOUND IS ${HDF5_FOUND}")
set(HDF5_PREFER_PARALLEL TRUE)

message(STATUS "C++ compiler is : ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ compiler identification is : ${CMAKE_CXX_COMPILER_ID}")

include_directories (SYSTEM 
                    #$ENV{MPICH_INC}
                    #$ENV{HDF5_INC}
		    ${MPI_CXX_INCLUDE_DIRS}
		    ${MPI_C_INCLUDE_DIRS}
		    ${HDF5_INCLUDE_DIRS}
		    #${HDF5_CXX_INCLUDE_DIRS}
		    ${ROOT_INCLUDE_DIRS}
                    $ENV{HIGHFIVE_INC}
                    #$ENV{ROOTSYS}/include
                    $ENV{CMSSW_BASE}/src
                    $ENV{DK2NU_INC})
		    
#link_directories ($ENV{ROOTSYS}/lib)

set (libraries ${libraries}
               ${ROOT_LIBRARIES}
	       ${MPI_CXX_LIBRARIES}
	       ${MPI_C_LIBRARIES}
	       ${HDF5_LIBRARIES}
	       #${HDF5_CXX_LIBRARIES}
               #$ENV{MPICH_LIB}/libmpi.so
               #$ENV{HDF5_LIB}/libhdf5.so
               $ENV{DK2NU_LIB}/libdk.so 
               Core
               RIO
               Tree
               Hist)


#add_executable(test_deserialize test_deserialize.cc)
#target_link_libraries(test_deserialize ${libraries})

#add_executable(read_h5_deserialize read_h5_deserialize.cc)
#target_link_libraries(read_h5_deserialize ${libraries})

#add_executable(read_h5_deserialize_all read_h5_deserialize_all.cc)
#target_link_libraries(read_h5_deserialize_all ${libraries})

add_executable(ConvertHDF5 ConvertHDF5.cc)
target_link_libraries(ConvertHDF5 ${libraries})

add_executable(ConvertHDF5_MPI_ver2 ConvertHDF5_MPI_ver2.cc)
#target_compile_options(ConvertHDF5_MPI PRIVATE ${MPI_CXX_COMPILE_FLAGS})
target_compile_options(ConvertHDF5_MPI_ver2 PRIVATE ${HDF5_CXX_COMPILER_EXECUTABLE})
target_link_libraries(ConvertHDF5_MPI_ver2 ${libraries})

add_executable(ConvertHDF5_MPI_ver3 ConvertHDF5_MPI_ver3.cc)
target_compile_options(ConvertHDF5_MPI_ver3 PRIVATE ${HDF5_CXX_COMPILER_EXECUTABLE})
target_link_libraries(ConvertHDF5_MPI_ver3 ${libraries})

add_executable(test_tmpi test_tmpi.cc)
target_compile_options(test_tmpi PRIVATE ${HDF5_CXX_COMPILER_EXECUTABLE})
target_link_libraries(test_tmpi ${libraries})

add_executable(h5_extend h5_extend.cc)
target_compile_options(h5_extend PRIVATE ${HDF5_CXX_COMPILER_EXECUTABLE})
target_link_libraries(h5_extend ${libraries})

add_executable(read_h5_deserialize_MPI_1D read_h5_deserialize_MPI_1D.cc)
target_compile_options(read_h5_deserialize_MPI_1D PRIVATE ${HDF5_CXX_COMPILER_EXECUTABLE})
target_link_libraries(read_h5_deserialize_MPI_1D ${libraries})

add_executable(run_scan run_scan.cc)
target_compile_options(run_scan PRIVATE ${HDF5_CXX_COMPILER_EXECUTABLE})
target_link_libraries(run_scan ${libraries})
