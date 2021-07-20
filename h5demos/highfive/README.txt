mpirun --host localhost -np 4 ./ConvertHDF5_MPI_ver3 ../rfiles/rootfilename.root 1 blaa.h5

###To deserialize the hdf5 file (for now)
./read_h5_deserialize_MPI_1D <hdf5filename.h5>

You can check the hostname by doing cat /etc/hosts

The one with 127.0.0.1 <host_name> should be used.


