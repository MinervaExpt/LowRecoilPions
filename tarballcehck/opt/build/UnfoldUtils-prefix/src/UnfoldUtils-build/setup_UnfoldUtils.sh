#Make this package relocatable by making PREFIX overrideable.  Default to the old behavior of setting up from a fixed location.
#You might use this feature to run on the FermiGrid for example.
PREFIX=${MINERVA_PREFIX:-"/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt"}

#Install directory is filled in by CMake at project configure time.
export UNFOLDUTILSROOT=${PREFIX}/lib
export LD_LIBRARY_PATH=$UNFOLDUTILSROOT:$LD_LIBRARY_PATH
export PATH=${PREFIX}/bin:$PATH
export PYTHONPATH=${UNFOLDUTILSROOT}/python:${PYTHONPATH}

export UNFOLDUTILSTYPE="STANDALONE"
export UNFOLDUTILSVERSION="ROOT6"
