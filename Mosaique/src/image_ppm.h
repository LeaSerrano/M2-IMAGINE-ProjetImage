#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <math.h>
#include <vector>
#include "./../lib/CImg/CImg.h"
using namespace cimg_library;

// Macro implementing a generic behaviour in case of an error
#define Error(mes, rv) { printf((mes)); printf(" ( in function %s ) \n", __func__); return (rv); }

// Macro implementing a generic behaviour in case of an memory allocation
#define try_malloc(var, type, size, rv) if( !( (var) = (type*) malloc(sizeof(type)*size) ) ) Error("Allocation Error !", (rv))

typedef unsigned char OCTET;


void ignorer_commentaires(FILE * f);


OCTET* load_pgm( std::string path, int* dimX, int* dimY );

OCTET* load_ppm( std::string path, int* dimX, int* dimY );

OCTET* load_Img( std::string path, int* dimX, int* dimY );


int write_pgm( OCTET* ImgOut, std::string ImgOutStr, int dimX, int dimY );

int write_ppm( OCTET* ImgOut, std::string ImgOutStr, int dimX, int dimY );

int write_Img( OCTET* ImgOut, std::string ImgOutStr, int dimX, int dimY );


bool endsWith(std::string const & value, std::string const & ending);