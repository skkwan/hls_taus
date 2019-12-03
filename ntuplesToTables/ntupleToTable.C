/************************************************************/
/* ntupleToTable.C                                          */
/* Author: Stephanie Kwan                                   */
/************************************************************/

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <math.h>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#ifndef NTUPLE_TO_TABLE_C_INCL
#define NTUPLE_TO_TABLE_C_INCL

/************************************************************/

//int writeNtupleToText(TTree *inputTree, bool isHex,
//		      TString outfile)
int ntupleToTable(void)
{
  // Create file
  FILE *fp = fopen("table.txt", "w");
  if (fp == NULL)
    {
      printf("ERROR opening file; exiting");
      return 1;
    }

  TFile *inputFile = TFile::Open("/afs/cern.ch/work/s/skkwan/public/triggerDevel/apr2019/CMSSW_10_5_0_pre1/src/L1Trigger/phase2L1BTagAnalyzer/tau_exercise/ntuples/newTauZL1PtCut10/afs/cern.ch/work/s/skkwan/public/triggerDevel/apr2019/CMSSW_10_5_0_pre1/src/L1Trigger/phase2L1BTagAnalyzer/tau_exercise/ntuples/newTauZL1PtCut10/ggH.root");
  TTree *inputTree = (TTree*) inputFile->Get("L1TauAnalyzer/efficiencyTree");

  // Declare variables to read from TTree
  Double_t l1Pt, l1Eta, l1StripPt;
  Double_t l1DM;
  Double_t l1PVDZ;
  Double_t l1HoE, l1EoH;
  Double_t l1TauZ;
  Double_t l1ChargedIso;

  // Set branch addresses
  inputTree->SetBranchAddress("l1Pt", &l1Pt);
  inputTree->SetBranchAddress("l1Eta", &l1Eta);
  inputTree->SetBranchAddress("l1StripPt", &l1StripPt);
  inputTree->SetBranchAddress("l1DM", &l1DM);
  inputTree->SetBranchAddress("l1PVDZ", &l1PVDZ);
  inputTree->SetBranchAddress("l1HoE", &l1HoE);
  inputTree->SetBranchAddress("l1EoH", &l1EoH);
  inputTree->SetBranchAddress("l1TauZ", &l1TauZ);
  inputTree->SetBranchAddress("l1ChargedIso", &l1ChargedIso);

  // Loop through TTree
  //  for (Int_t i = 0; i < inputTree->GetEntries(); i++ ) {
  for (Int_t i = 0; i < 5; i++) {
    inputTree->GetEntry(i);

    fprintf(fp, "%018x \n", l1Pt);

  }
  
  return 0;
}

#endif
