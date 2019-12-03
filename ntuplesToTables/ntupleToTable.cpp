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

#define events_in_file (1)
#define taus_per_event (5)
#define n_features (9)


/************************************************************/v

vector<vector<unsigned int>> uintTableFromTree(void)
{
  // Create file
  FILE *fp = fopen("table.txt", "w");
  if (fp == NULL)
    {
      printf("ERROR opening file; exiting");
      return 1;
    }

  TFile *inputFile = TFile::Open("/afs/cern.ch/work/s/skkwan/public/triggerDevel/apr2019/CMSSW_10_5_0_pre1/src/L1Trigger/phase2L1BTagAnalyzer/tau_exercise/ntuples/newTauZL1PtCut10/ggH.root");
  TTree *inputTree = (TTree*) inputFile->Get("L1TauAnalyzer/efficiencyTree");


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

  // Declare table
  vector<vector<unsigned int>> v;
  int tokens_per_event = taus_per_event * n_features;

  for (int i = 0; i < 5; i++)
    {
      inputTree->GetEntry(i);
      
      for (int j = 0; j < n_features; j++)
	{
	vector<unsigned int>  vars;
	// Need to decide how to convert floats to unsigned ints!!
	
	// Push back to vars
      }
      v.push_back(vars);
    }

  //  for (Int_t i = 0; i < inputTree->GetEntries(); i++ ) {                                                        
  return v;
}


/************************************************************/

/* Write table (vector of vectors) of unsigned ints to
   output .txt file outFile.  */
int writeTableToText(vector<vector<unsigned int>> table,
		     TString outFile)
{
  // Create file
  FILE *fp = fopen(outFile, "w");
  if (fp == NULL)
    {
      printf("ERROR opening .txt file; exiting");
      return 1;
    }


  return 0;
}

/************************************************************/

int main()
{
  vector<vector<unsigned int>> vars = uintTableFromTree();
  writeTableToText(vars, "output.txt");

  return 0;
}
#endif
