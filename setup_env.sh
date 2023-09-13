#Extra set up to build standalone MINERvA macros with CMake.
#Needed to detect LCG (= LHC Computing Grid?) components that
#I think our ROOT installation was configured to link against.
#Pass -d on the command line to find debug builds.
#Copied from Andrew Olivier's work to mine.

MINERVA_VERSION="v22r1p1"
CMTCONFIG="x86_64-slc7-gcc49"
MINERVA_INSTALL_DIR="/cvmfs/minerva.opensciencegrid.org/minerva"

LCG_BUILD_TYPE="opt"
if [ "$1" = "-d" ]
then
  echo "Setting up in debug mode"
  LCG_BUILD_TYPE="dbg"
fi

#Set up xrootd for TNetXNGFile
#export LD_LIBRARY_PATH=${MINERVA_INSTALL_DIR}/software_releases/${MINERVA_VERSION}/lcg/external/xrootd/3.3.6/${CMTCONFIG}-${LCG_BUILD_TYPE}/lib64:${LD_LIBRARY_PATH}

#Set up gccxml for Reflex dictionary generation (needed for CINT and python bindings in PlotUtils)
export PATH=${MINERVA_INSTALL_DIR}/software_releases/${MINERVA_VERSION}/lcg/external/LCGCMT/LCGCMT_61/LCG_Settings/../../../gccxml/0.9.0_20150423/${CMTCONFIG}-${LCG_BUILD_TYPE}/bin:${PATH}

#Set up the GNU Scientific Library.
#Our ROOT installation on the GPVMs seems to be linked against it in some weird way that I don't understand.
export LD_LIBRARY_PATH=${MINERVA_INSTALL_DIR}/software_releases/${MINERVA_VERSION}/lcg/external/GSL/1.10/${CMTCONFIG}-${LCG_BUILD_TYPE}/lib:${LD_LIBRARY_PATH}
