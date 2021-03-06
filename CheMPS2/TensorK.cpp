/*
   CheMPS2: a spin-adapted implementation of DMRG for ab initio quantum chemistry
   Copyright (C) 2013, 2014 Sebastian Wouters

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <stdlib.h>
#include <math.h>

#include "TensorK.h"
#include "Lapack.h"

CheMPS2::TensorK::TensorK(const int indexIn, const int IdiffIn, const SyBookkeeper * denBKIn) : TensorSwap(indexIn, IdiffIn, true, denBKIn){ }

CheMPS2::TensorK::~TensorK(){ }

void CheMPS2::TensorK::construct(TensorT * denT){

   Clear();

   for (int ikappa=0; ikappa<nKappa; ikappa++){

      const int IDR = denBK->directProd(Idiff,sectorI1[ikappa]);
      int dimUR = denBK->gCurrentDim(index,   sectorN1[ikappa],   sectorTwoS1[ikappa], sectorI1[ikappa]);
      int dimDR = denBK->gCurrentDim(index,   sectorN1[ikappa]+1, sectorTwoSD[ikappa], IDR             );
      int dimL = denBK->gCurrentDim(index-1, sectorN1[ikappa], sectorTwoS1[ikappa], sectorI1[ikappa]);

      if (dimL>0){

         double * BlockTup   = denT->gStorage(sectorN1[ikappa], sectorTwoS1[ikappa], sectorI1[ikappa], sectorN1[ikappa], sectorTwoS1[ikappa], sectorI1[ikappa]);
         double * BlockTdown = denT->gStorage(sectorN1[ikappa], sectorTwoS1[ikappa], sectorI1[ikappa], sectorN1[ikappa]+1, sectorTwoSD[ikappa], IDR);

         char trans = 'T';
         char notrans = 'N';
         double alpha = 1.0;
         double beta = 1.0; //add
         dgemm_(&trans,&notrans,&dimUR,&dimDR,&dimL,&alpha,BlockTup,&dimL,BlockTdown,&dimL,&beta,storage+kappa2index[ikappa],&dimUR);

      }
   }

}


