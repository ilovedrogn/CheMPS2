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

#include <stdlib.h> /*srand, rand*/
#include <iostream>
#include <time.h> /*time*/
#include <math.h>
#include <string.h>
#include <sys/stat.h>

#include "DMRG.h"

using namespace std;

int main(void){

   cout.precision(15);
   srand(time(NULL));

   //The path to the matrix elements
   string matrixelements = "../../tests/matrixelements/N2_N14_S0_d2h_I0.dat";
   struct stat stFileInfo;
   int intStat = stat(matrixelements.c_str(),&stFileInfo);
   if (intStat != 0){
      cout << "Please set the correct relative path to tests/matrixelements/N2_N14_S0_d2h_I0.dat in tests/test5.cpp for the compiled binary test5 to work." << endl;
      return 628788;
   }
   
   //The Hamiltonian
   CheMPS2::Hamiltonian * Ham = new CheMPS2::Hamiltonian(matrixelements);
   
   //The targeted state
   int TwoS = 0;
   int N = 14;
   int Irrep = 0;
   CheMPS2::Problem * Prob = new CheMPS2::Problem(Ham, TwoS, N, Irrep);
   Prob->SetupReorderD2h();
   
   //The optimization scheme
   int D = 1000;
   double Econv = 1e-12;
   int maxSweeps = 100;
   double noisePrefactor = 0.0;
   CheMPS2::ConvergenceScheme * OptScheme = new CheMPS2::ConvergenceScheme(1);
   OptScheme->setInstruction(0,D,Econv,maxSweeps,noisePrefactor);
   
   //Run ground state calculation
   CheMPS2::DMRG * theDMRG = new CheMPS2::DMRG(Prob,OptScheme);
   double Energy0 = theDMRG->Solve();
   
   //Calculate the first two excited states
   theDMRG->activateExcitations(10);
   theDMRG->newExcitation(20.0);
   double Energy1 = theDMRG->Solve();
   theDMRG->newExcitation(20.0);
   double Energy2 = theDMRG->Solve();
   
   //Clean up
   if (CheMPS2::DMRG_storeMpsOnDisk){ theDMRG->deleteStoredMPS(); }
   if (CheMPS2::DMRG_storeRenormOptrOnDisk){ theDMRG->deleteStoredOperators(); }
   delete theDMRG;
   delete OptScheme;
   delete Prob;
   delete Ham;
   
   //Check succes
   bool OK1 = (fabs(Energy0 + 107.648250974014)<1e-10)? true : false;
   bool OK2 = (fabs(Energy1 + 106.944757308768)<1e-10)? true : false;
   bool OK3 = (fabs(Energy2 + 106.92314213886 )<1e-10)? true : false;
   
   bool success = (OK1 && OK2 && OK3);
   cout << "================> Did test 5 succeed : ";
   if (success){ cout << "yes" << endl; }
   else { cout << "no" << endl; }

   return 0;

}


