#ifndef _TESTROUTINECUDA_
#define _TESTROUTINECUDA_

#ifdef __CUDACC__
#define CUDA_CALLABLE_MEMBER __host__ __device__
#define CUDA_CALLABLE_MEMBER_DEVICE __device__
#else
#define CUDA_CALLABLE_MEMBER
#define CUDA_CALLABLE_MEMBER_DEVICE
#endif

#include <algorithm>
#include "Particle.h"
#include "Boundary.h"
struct test_routinecuda { 
   double x;
   double y;
   double z;
   int nx;
   int nz;

   double* gridxp;
   double* gridzp;
   double* datap; 
    
   test_routinecuda(double _x, double _y, double _z,int _nx, int _nz,
           double* _gridxp,double* _gridzp,double* _datap) : 
       x(_x), y(_y), z(_z),nx(_nx), nz(_nz), gridxp(_gridxp),
       gridzp(_gridzp), datap(_datap) {}


CUDA_CALLABLE_MEMBER_DEVICE    
void operator()(double &d) const { 
    //double tmp = gridzp[0];
    //d = tmp;
    d = interp2dcuda(x,y,z,nx,nz,gridxp,gridzp,datap);
}
};

#endif
