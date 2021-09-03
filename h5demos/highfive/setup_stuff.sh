export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
asetup Athena,master,latest
#asetup Athena,master,r07-13
#export HIGHFIVE_INC=/home/abashyal/root_serialization/HighFive/include
export HDF5_LIB=/cvmfs/atlas-nightlies.cern.ch/repo/sw/master_Athena_x86_64-centos7-gcc8-opt/sw/lcg/releases/LCG_98python3_ATLAS_8/hdf5/1.10.5/x86_64-centos7-gcc8-opt/lib/
export HDF5_INC=/cvmfs/atlas-nightlies.cern.ch/repo/sw/master_Athena_x86_64-centos7-gcc8-opt/sw/lcg/releases/LCG_98python3_ATLAS_8/hdf5/1.10.5/x86_64-centos7-gcc8-opt/include/
export JSON_CMAKE=/cvmfs/atlas-nightlies.cern.ch/repo/sw/master_Athena_x86_64-centos7-gcc8-opt/sw/lcg/releases/LCG_100_ATLAS_7/jsonmcpp/3.9.1/x86_64-centos7-gcc8-opt/lib64/cmake/
export ZSTD_CMAKE=/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/zstd/1.4.5/lib/cmake/


