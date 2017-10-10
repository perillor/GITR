#!/bin/bash
source ../env.gpufusion.sh

~/cmake/cmake-3.7.0-rc1-Linux-x86_64/bin/cmake -DTHRUST_INCLUDE_DIR=$CUDA_PATH/include \
    -DNETCDF_DIR=$NETCDF \
    -DNETCDF_CXX_ROOT=$NETCDFCXX4 \
    -DLIBCONFIGPP_LIBRARY=$LIBCONFIGDIR/$LIBCONFIGLIB \
    -DUSE_CUDA=1 \
    -DUSEMPI=0 \
    -DUSE_BOOST=1 \
    -DUSEIONIZATION=0 \
    -DUSERECOMBINATION=0 \
    -DUSEPERPDIFFUSION=0 \
    -DUSECOULOMBCOLLISIONS=0 \
    -DUSETHERMALFORCE=0 \
    -DUSESURFACEMODEL=0 \
    -DUSESHEATHEFIELD=0 \
    -DBIASED_SURFACE=0 \
    -DUSEPRESHEATHEFIELD=0 \
    -DBFIELD_INTERP=0 \
    -DLC_INTERP=0 \
    -DGENERATE_LC=0 \
    -DEFIELD_INTERP=0 \
    -DPRESHEATH_INTERP=0 \
    -DDENSITY_INTERP=0 \
    -DTEMP_INTERP=0 \
    -DFLOWV_INTERP=0 \
    -DGRADT_INTERP=0 \
    -DODEINT=0 \
    -DFIXEDSEEDS=0 \
    -DPARTICLESEEDS=0 \
    -DGEOM_TRACE=1 \
    -DGEOM_HASH=0 \
    -DGEOM_HASH_SHEATH=0 \
    -DPARTICLE_TRACKS=1 \
    -DPARTICLE_SOURCE=0 \
    -DSPECTROSCOPY=0 \
    -DUSE3DTETGEOM=0 \
    -DUSECYLSYMM=0 \
    -DCHECK_COMPATIBILITY=1 \
    ..
