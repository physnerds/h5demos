mpirun --host localhost -np 4 ./ConvertHDF5_MPI_ver3 ../rfiles/rootfilename.root 1 blaa.h5

###To deserialize the hdf5 file (for now)
./read_h5_deserialize_MPI_1D <hdf5filename.h5>

Use the setup scripts setup_stuff.sh and setup_lcrc.sh to setup the required environmental variables.

In the source directory (where this README file lives):

$mkdir build;cd build

Then do the build:

$cmake -DCMAKE_CXX_COMPILER=/blues/gpfs/software/centos7/spack/opt/spack/linux-centos7-x86_64/gcc-8.2.0/intel-mpi-2018.4.274-ozfo327/compilers_and_libraries_2018.5.274/linux/mpi/intel64/bin/mpicxx -DCMAKE_C_COMPILER=/blues/gpfs/software/centos7/spack/opt/spack/linux-centos7-x86_64/gcc-8.2.0/intel-mpi-2018.4.274-ozfo327/compilers_and_libraries_2018.5.274/linux/mpi/intel64/bin/mpicc -DCMAKE_PREFIX_PATH=/cvmfs/atlas-nightlies.cern.ch/repo/sw/master_Athena_x86_64-centos7-gcc8-opt/sw/lcg/releases/LCG_100_ATLAS_7/jsonmcpp/3.9.1/x86_64-centos7-gcc8-opt/lib64/cmake/ ..

$make;

To test without MPI:
$./Convert_MPI_ver4 ../rfiles/For_TMVA_DATA_minervame5A6A6B_kludged_red.root 1 blaa.h5

To test with the MPI:
(In this example running 3 MPI processes)
$mpirun -np 3 ./ConvertHDF5_MPI_ver4 ../rfiles/For_TMVA_DATA_minervame5A6A6B_kludged_red.root 1 blaa.h5



