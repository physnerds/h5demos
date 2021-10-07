########################################################START OF THE READ ME FILE#########################
####ORIGINAL AUTHOR SABA SERISH##########################
######COLLECTIVE IO AND FURTHER IMPROVEMENT IMPLEMENTED BY AMIT BASHYAL (abashyal@anl.gov)##############################

1. Information
This package is gives an example of how we can deserialize a ROOT Tree with multiple branches using the HDF5
collective IO. Given an input ROOT File, the branches are written into individual data-sets.

The following informations are written in the HDF5 file for reading back entries from each branch:

Branch-Name and the entries: The Dataset with the name of the branch contains the entries that were read from the input ROOT File. This data-set is basically a 1 D array of buffers (characters).

Offset : This dataset "branch-name_offset" contains the start position of each buffers. Note that if the buffers were written in a batch, for example 2 entries at a time, it records the start position of every two entries in the
branch-name" data-set.

Info: (This is sort of Meta-data for each data-set). This dataset "branch-name_info" contains the type of the 
data-sets. If the ROOT branch is a fundamental type (i.e Int_t, Float_t or Bool_t) it saves this information. 
If the ROOT branch is a ROOT::TClass, then it saves the class-name. The idea is to provide the users with 
the required information when reading back the entries from the HDF5 File.

Event-Info: This is the Meta-data data-set that contains: 
       Total MPI-Processes used to write the HDF5 File.
       Total Number of Events.
       Batch-size (i.e the number of events that were being written at a time*)


****************************************************************************************************
2. INSTALLATION OF THE PACKAGE

Both Local installation and the installation in the ANL LCRC machines are supported. 

Required packages are: 
   CMake version>3.10 (https://cmake.org)

   Standard MPI 
   MPICH (https://www.mpich.org)

   ROOT (https://root.cern.ch)

   HDF5 with Parallel option (https://support.hdfgroup.org/HDF5/PHDF5/)
   
I would highly recommend the CMake build of each of these packages (instead of source or binaries). The CMakeList of this package assumes that all the required packages were built with CMake. 

  In the LCRC machines, these environment can be setup using the modules. A setup script is available in the lcrc branch of this code. More on this later.

/////////////////////////////////////////////////////////////////////////////////////////////////////

2.a INSTALLATION OF THE PACKAGE (LOCAL MACHINE)

   1. Install all the required packages. 
  
   2. Make sure the ROOT environment is setup. 
  
   3. Download the mpi_final branch of the code from the git repository:
      https://github.com/physnerds/trial_h5demos.git
      $git clone -b mpi_final https://github.com/physnerds/trial_h5demos.git
 
    4. Go to trial_h5demos/h5demos/highfive/ where you will find the CMakeLists.txt
  
   5. Create build directory inside highfive: 
      $mkdir build
      $cd build
   
   6. Do the cmake build
      $cmake ..

   7. If you have installed all the packages but the cmake cannot find those packages you, alternately you can 
      give the prefix paths to those packages

      $cmake .. -DCMAKE_PREFIX_PATH=/path/to/ROOT/cmake -DCMAKE_PRFIX_PATH=/path/to/MPICH/cmake \\
       -DCMAKE_PREFIX_PATH=/path/to/HDF5 
   
   8. If everything is done properly, you will get a Makefile inside the build directory.
      $make
   
   

//////////////////////////////////////////////////////////////////////////////////////////////////////

2.b INSTALLATION OF THE PACKAGE (LCRC MACHINE)
  *THIS WAS TESTED IN THE heplogin.lcrc machine
   
  1. Download the mpi_lcrc branch of the code:
     $git clone -b mpi_lcrc https://github.com/physnerds/trial_h5demos.git   
    
  3. Inside the trial_h5demos/h5demos/highfive directory, you will find "setup_stuff.sh"
     $source setup_stuff.sh

  4. Setup the environment to point the headers and libraries of the dependencides:
     $source setup_lcrc.sh

  5. Create a build directory
    $mkdir build
    $cd build

  6. For some reason the ROOT package complains about not finding a json package. So we set the 
     path to the json file cmake during the build time.
     $cmake .. -DCMAKE_PREFIX_PATH=/cvmfs/atlas-nightlies.cern.ch/repo/sw/master_Athena_x86_64-centos7-gcc8-opt/sw/lcg/releases/LCG_100_ATLAS_7/jsonmcpp/3.9.1/x86_64-centos7-gcc8-opt/lib64/cmake/
     $make



//////////////////////////////////////////////////////////////////////////////////////////////////////////


3. RUNNING THE CODE

   ***********************************************************
   *FIRST A VERY SIMPLE TEST CODE
   **********************************************************
   If everything has worked so far, you will see 3 executables in the build directory:

   a. test_parallel_hdf5: This is a test script and a simpler example where random numbers are written into an HDF5 file in parallel. 
    
    To run this code, simply do:
    $mpirun -np mpi_processes ./test_parallel_hdf5  
    where mpi_processes is the number of mpi-processes that you want to use to do the IO parallely. 
    You can give any value greater than 0 (1 is basically serial IO & maybe not so large number)
    It creates a "test_file.hdf5" file
     
    **********
    In the mac you might have to add --host=localhost as well in the mpirun arguments. 
    **********
   
     Do h5ls to see inside the hdf5 file:
     $h5ls test_file.hdf5
     You will see something like this:
     run                      Group
     This basically means inside the hdf5 file there a group called "run"
     If Inside run, there is a subgroup called lumi ($h5ls test_file.hdf5/run)

     There is a test_dataset DATASET inside lumi. It shows the size of the data-set and the maximum 
     possible size (in case you want to write new events in this existing data-set).     
     $h5ls test_file.hdf5/run/lumi
     test_data                Dataset {9/Inf}

     
     If you want to all the content of this hdf5 file:
     $h5dump test_file.hdf5
   
HDF5 "test_file.hdf5" {
GROUP "/" {
   GROUP "run" {
      ATTRIBUTE "/run" {
         DATATYPE  H5T_STD_I32LE
         DATASPACE  SCALAR
         DATA {
         (0): 0
         }
      }
      GROUP "lumi" {
         ATTRIBUTE "lumi" {
            DATATYPE  H5T_STD_I32LE
            DATASPACE  SCALAR
            DATA {
            (0): 0
            }
         }
         DATASET "test_data" {
            DATATYPE  H5T_STD_I8LE
            DATASPACE  SIMPLE { ( 9 ) / ( H5S_UNLIMITED ) }
            DATA {
            (0): 7, 7, 5, 93, 52, 35, 88, 46, 40
            }
         }
      }
   }
}
}
   
    
///////////////////////////////////////////////////////////////////////////////////////////////////////
  
  **************************************************
  * THE MAIN CODE
  **************************************************

  b. ConvertHDF5_MPI_ver4
     This is the main example code that actually reads the input ROOT file with multiple TBranch and writes them 
     into the data-sets in parallel. If you try to run this code serially, you will see that it needs 
     certain arguments.
  
     $./ConvertHDF5_MPI_ver4 
      Please specify input file, batch-size and outputfilename

     input file: The input ROOT File that we want to write into the HDF5 file. Inside the
                 h5demos/highfive/rfiles directory, there is a test ROOT file with 100 entries and few 
                 branches that should work with the code as it is.
     batch-size: The number of entries we want to write into the HDF5 File at a time. For example 
                 batch-size 2 means the IO happens after every 2 entries are read from the ROOT File.
     outputfilename: The name of the output HDF5 File.


     Example of Running the code parallely (from h5demos/highfive/build directory):
     $mpirun -np 3 ./ConvertHDF5_MPI_ver4 ../rfiles/For_TMVA_DATA_minervame5A6A6B_kludged_small_stats.root 4 blaa.h5

     This will do the read the ROOT file  with 3 parallel processes. The ROOT to HDF5 IO happens after every 4 events   are collected. 
     
     Lets look at the output of this file:
     /////////////////////////////////////////////
     $h5ls blaa.h5/run/lumi
     Q2                       Dataset {800/Inf}
     Q2_info                  Dataset {8}
     Q2_offset                Dataset {27/Inf}
     True_proton_T            Dataset {800/Inf}
     True_proton_T_info       Dataset {8}
     True_proton_T_offset     Dataset {27/Inf}
     cos_theta                Dataset {800/Inf}
     cos_theta_info           Dataset {8}
     cos_theta_offset         Dataset {27/Inf}
     eventinfo                Dataset {3}***********
    ...
    /////////////////////////////////////////////////////////////
     
     There are over 21 branches in the input ROOT file TTree and for each branches, it creates 3 data-sets 
     as mentioned above. For example:
     Q2 is the actual data-set corresponding to the Q2 TBranch of the input ROOT file.
     It contains the buffer of the events from the input ROOT File.
     
     Q2_info contains the information about the type of the data-set (Float_t in this case).
     
     Q2_offset contains the start position of each buffer. Note that we ran the code with batch-size of 4.
     Hence, it will contain the start position of every 4th event buffer. The last few offsets could be for 
     less than or more than 4 entries to adjust the parallel IO. Later on, we will see how we can use the offset 
     info to deserialize into events while still taking care of these adjustment.
    
              DATASET "Q2_offset" {
            DATATYPE  H5T_STD_I32LE
            DATASPACE  SIMPLE { ( 27 ) / ( H5S_UNLIMITED ) }
            DATA {
            (0): 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384,
            (13): 416, 448, 480, 512, 544, 576, 608, 640, 672, 704, 736, 768,
            (25): 784, 792
            }

     As you can see all but the last 3 members in these arrays are the multiple of 32. Since in this case, 
     the buffer size of each event is 8, the buffer size for a batch of 4 elements is 32. The last 3 members
     are readjusted to have 2 and 1 events buffers.

    
    *****There is an additional dataset called eventInfo which contains 3 information:
    1. Batch size used to create the hdf5 file.
    2. Number of Parallel processes used.
    3. Total number of entries/events that were written into the HDF5 File. 
   
     
/////////////////////////////////////////////////////////////////////////////////////////////////
 
   c. read_h5_deserialize_MPI_1D
      
      This can read back the events from the output HDF5 file that was just created. 
      To run this code:  
      $./read_h5_deserialize_MPI_1D blaa.h5 [name of the file we just created].
      
      Name of the data-set to be read is hardcoded in the code (but can be changed anyway) and it reads
      one data-set at a time. Basically this is an example of reading back the buffers from the HDF5 
      file to get the events.

      You will see something like this:
      *********************************************
      Type of Data is Float_t 

      4 Batch 
      3 MPI-Size 
      100 Total Events 

      Q2 0 0.234274
      Q2 1 1.37531
      Q2 2 0.0863408
      Q2 3 0.33115
      Q2 4 0.173052
      ***********************************************




########################################################END OF THE READ ME FILE#########################
