#include "h1.cuh"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <netcdf>
#include "Boundary.h"
#include "Particle.h"
#if USE_BOOST
	#include "boost/multi_array.hpp"
	#include "boost/filesystem.hpp"
#endif
#include "io.hpp"
#ifdef __CUDACC__
#include <thrust/host_vector.h>
#endif

#ifdef __CUDACC__
#define CUDA_CALLABLE_MEMBER __host__ __device__
#define CUDA_CALLABLE_MEMBER_DEVICE __device__
#else
#define CUDA_CALLABLE_MEMBER
#define CUDA_CALLABLE_MEMBER_DEVICE
#endif

using namespace std;
using namespace netCDF;
using namespace exceptions;

//IO

int read_ar2Input( string fileName, float *Bfield[]) {

    // Check input file exists

    ifstream file(fileName.c_str());
    if(!file.good()) {
        cout<<"ERROR: Cannot file input file ... "<<fileName<<endl;
        exit(1);
    }

    NcFile nc(fileName.c_str(), NcFile::read);

    NcDim nc_nR(nc.getDim("nR"));
    NcDim nc_nZ(nc.getDim("nZ"));
    
    int nR = nc_nR.getSize(); 
    int nZ = nc_nZ.getSize(); 

    NcVar nc_r(nc.getVar("r"));
    NcVar nc_z(nc.getVar("z"));

    vector<float> r;
    r.resize(nR);
    nc_r.getVar(&r[0]);

    vector<float> z;
    z.resize(nZ);
    nc_z.getVar(&z[0]);


    // Allocate contiguous 2D array for netcdf to work
    float **br = new float*[nR];
    br[0] = new float[nR*nZ];
    for(int i=0; i<nR; i++){
        br[i] = &br[0][i*nZ];
    }


    NcVar nc_br(nc.getVar("br"));

    nc_br.getVar(br[0]);

    for(int i=0; i<nR; i++){
        for(int j=0; j<nZ; j++){
           Bfield[i][j] = br[j][i]; 
        }
    }

    return(0);

}


int read_profileNs( string fileName, string nxName, string nzName,int &n_x,int &n_z ) {

    // Check input file exists

    ifstream file(fileName.c_str());
    if(!file.good()) {
        cout<<"ERROR: Cannot file input file ... "<<fileName<<endl;
        exit(1);
    }

    NcFile nc(fileName.c_str(), NcFile::read);

    NcDim nc_nx(nc.getDim(nxName));
    NcDim nc_nz(nc.getDim(nzName));
    
    n_x = nc_nx.getSize(); 
    n_z = nc_nz.getSize(); 


    return(0);

}


int read_profiles( string fileName, int &n_x, int &n_z,string gridxName, std::vector<float>& gridx,string gridzName,
          std::vector<float>& gridz,string dataName, std::vector<float>& data) {

    // Check input file exists

    ifstream file(fileName.c_str());
    if(!file.good()) {
        cout<<"ERROR: Cannot file input file ... "<<fileName<<endl;
        exit(1);
    }

    NcFile nc(fileName.c_str(), NcFile::read);

    NcVar nc_gridx(nc.getVar(gridxName));
    NcVar nc_gridz(nc.getVar(gridzName));

    nc_gridx.getVar(&gridx[0]);
    nc_gridz.getVar(&gridz[0]);

    NcVar nc_ne(nc.getVar(dataName));
    nc_ne.getVar(&data[0]);

    return(0);

}

int read_profile2d( string fileName,string dataName, std::vector<float>& data) {

    // Check input file exists

    ifstream file(fileName.c_str());
    if(!file.good()) {
        cout<<"ERROR: Cannot file input file ... "<<fileName<<endl;
        exit(1);
    }

    NcFile nc(fileName.c_str(), NcFile::read);


    NcVar nc_ne(nc.getVar(dataName));
    nc_ne.getVar(&data[0]);

    return(0);

}
int read_profile1d( string fileName,string gridxName, std::vector<float>& gridx) {

    // Check input file exists

    ifstream file(fileName.c_str());
    if(!file.good()) {
        cout<<"ERROR: Cannot file input file ... "<<fileName<<endl;
        exit(1);
    }

    NcFile nc(fileName.c_str(), NcFile::read);

    NcVar nc_gridx(nc.getVar(gridxName));

    nc_gridx.getVar(&gridx[0]);


    return(0);

}
void OUTPUT(char outname[],int nX, int nY, float **array2d)
{
       ofstream outfile;
				//Output


			outfile.open (outname );
			
				 for(int i=1 ; i<=nX ; i++)
				{
				outfile << "Dep( " << i<< ",:) = [ " ;
					for(int j=0 ; j<nY ; j++)
					{
					outfile << array2d[i-1][j] << "  " ;
					//std::cout << r[i] << std::endl;
					}
					outfile << "  ];" << std::endl;
				}
			outfile.close();	
		
		
}

void OUTPUT2d(std::string folder,std::string outname,int nX, int nY, float *array2d)
{
       ofstream outfile;
#if USE_BOOST	
			//Output
        boost::filesystem::path dir(folder);

            if(!(boost::filesystem::exists(dir)))
            {
              std::cout<<"Doesn't Exists"<<std::endl;
              if (boost::filesystem::create_directory(dir))
              {
              std::cout << " Successfully Created " << std::endl;
              }
            }
#endif
            std::string full_path = folder + "/" + outname;
			outfile.open (full_path );
			
				 for(int i=1 ; i<=nX ; i++)
				{
				outfile << "Dep( " << i<< ",:) = [ " ;
					for(int j=0 ; j<nY ; j++)
					{
					outfile << array2d[(i-1)*nY + j] << "  " ;
					//std::cout << r[i] << std::endl;
					}
					outfile << "  ];" << std::endl;
				}
			outfile.close();	
		
		
}

void OUTPUT1d(std::string folder,std::string outname,int nX, float *array2d)
{
       ofstream outfile;
#if USE_BOOST
				//Output
        boost::filesystem::path dir(folder);

            if(!(boost::filesystem::exists(dir)))
            {
             // std::cout<<"Doesn't Exists"<<std::endl;
              if (boost::filesystem::create_directory(dir))
              {
              //std::cout << " Successfully Created " << std::endl;
              }
            }
#endif
            std::string full_path = folder + "/" + outname;
			outfile.open (full_path );
			
				outfile << "nX " << "  = [ " ;
				 for(int i=0 ; i<nX ; i++)
				{
					outfile << array2d[i] << "  " ;
				}
					outfile << "  ];" << std::endl;
			outfile.close();	
		
		
}
