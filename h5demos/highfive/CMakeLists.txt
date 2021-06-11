cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(h5demos CXX)
set(CMAKE_C_FLAGS $ENV{CFLAGS})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories (SYSTEM 
                    $ENV{MPICH_INC}
                    $ENV{HDF5_INC}
                    $ENV{HIGHFIVE_INC}
                    $ENV{ROOTSYS}/include
                    $ENV{CMSSW_BASE}/src
                    $ENV{DK2NU_INC})
link_directories ($ENV{ROOTSYS}/lib)
set (libraries ${libraries} 
               $ENV{MPICH_LIB}/libmpi.so
               $ENV{HDF5_LIB}/libhdf5.so
               $ENV{DK2NU_LIB}/libdk.so 
               Core
               RIO
               Tree
               Hist)


add_executable(test_deserialize test_deserialize.cc)
target_link_libraries(test_deserialize ${libraries})

add_executable(read_h5_deserialize read_h5_deserialize.cc)
target_link_libraries(read_h5_deserialize ${libraries})

#add_executable(read_h5_deserialize_all read_h5_deserialize_all.cc)
#target_link_libraries(read_h5_deserialize_all ${libraries})

add_executable(ConvertHDF5 ConvertHDF5.cc)
target_link_libraries(ConvertHDF5 ${libraries})
