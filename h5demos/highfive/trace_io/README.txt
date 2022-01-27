This jupyter code looks the at the dump of the "strace" that is run on the ConverHDF5_MPI_ver4 code in the LCRC machine.

An example of running this code in serial (i.e. no MPI I/O) is:

strace -T -ttt -o dump.txt ./ConvertHDF5_MPI_ver4 ../rfiles/For_TMVA_DATA_minervame5A6A6B_kludged_red.root 1 blaa.h5

This will produce an output file called dump.txt which is the input for the jupyter-notebook "analyze_io_with_timestamp.py.ipynb".

More info on how to analyze strace outputs are here:
https://www.admin-magazine.com/HPC/Articles/Tuning-I-O-Patterns-in-C
AND HERE
https://www.pcmag.com/encyclopedia/term/file-io

In summary, we want to know the proc_index that is returned when the system opens the file (when calling the function [open(fname,...,..)=proc_index]).
That proc_index can be then traced to see the number of various I/O calls made related to the filename objects.

There are other standard I/O calls made to open and read various c++ libraries and ROOT libraries (which are not important to study the improvement of the I/O calls when modifying the package).

