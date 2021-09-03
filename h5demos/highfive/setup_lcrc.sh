#load the required libraries
#module restore highfivemodule
module load gcc/8.2.0-g7hppkz intel-mpi/2018.4.274-ozfo327 hdf5/1.10.5-vozfsah
#module load hdf5/1.10.5-vozfsah
#export MPICH_INC=/blues/gpfs/software/centos7/spack/opt/spack/linux-centos7-x86_64/gcc-8.2.0/mpich-3.3-ipsk4eg/include
export MPICH_INC=/blues/gpfs/software/centos7/spack/opt/spack/linux-centos7-x86_64/gcc-8.2.0/intel-mpi-2018.4.274-ozfo327/compilers_and_libraries_2018.5.274/linux/mpi/intel64/include

#export MPICH_LIB=/blues/gpfs/software/centos7/spack/opt/spack/linux-centos7-x86_64/gcc-8.2.0/mpich-3.3-ipsk4eg/lib
export MPICH_LIB=/blues/gpfs/software/centos7/spack/opt/spack/linux-centos7-x86_64/gcc-8.2.0/intel-mpi-2018.4.274-ozfo327/compilers_and_libraries_2018.5.274/linux/mpi/intel64/lib

export HDF5_INC=/blues/gpfs/software/centos7/spack/opt/spack/linux-centos7-x86_64/gcc-8.2.0/hdf5-1.10.5-vozfsah/include/
export HDF5_LIB=/blues/gpfs/software/centos7/spack/opt/spack/linux-centos7-x86_64/gcc-8.2.0/hdf5-1.10.5-vozfsah/lib/
export HIGHFIVE_INC=/home/abashyal/root_serialization/HighFive/include
export DK2NU_INC=/home/abashyal/dk2nu/include
export DK2NU_LIB=/home/abashyal/dk2nu
