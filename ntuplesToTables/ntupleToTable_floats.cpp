/************************************************************/
/* ntupleToTable.cpp                                        */
/* Author: Stephanie Kwan                                   */
/************************************************************/

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <math.h>
#include <stdio.h>

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
#define taus_per_event (330)
#define n_features (9)


/************************************************************/

vector<vector<Double_t>> floatTableFromTree(void)
{
  // Create file
  FILE *fp = fopen("table.txt", "w");
  if (fp == NULL)
    {
      printf("ERROR opening file");
    }

  //  TFile *inputFile = TFile::Open("/afs/cern.ch/work/s/skkwan/public/triggerDevel/apr2019/CMSSW_10_5_0_pre1/src/L1Trigger/phase2L1BTagAnalyzer/tau_exercise/ntuples/2020_Feb_06-ggHtautau_200PU_MINI.root");
  TFile *inputFile = TFile::Open("analyzer-GluGluHToTauTau_M125_14TeV_powheg_pythia8.root");
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
  vector<vector<Double_t>> v;
  int tokens_per_event = taus_per_event * n_features;

  for (int i = 0; i < (events_in_file * taus_per_event) ; i++)
    {
      inputTree->GetEntry(i);

      vector<Double_t>  vars;
      
      vars.push_back(l1Pt);
      vars.push_back(l1Eta);
      vars.push_back(l1StripPt);
      vars.push_back(l1DM);
      vars.push_back(l1PVDZ);
      vars.push_back(l1HoE);
      vars.push_back(l1EoH);
      vars.push_back(l1TauZ);
      vars.push_back(l1ChargedIso);
      
      // Push back to vars
      
      v.push_back(vars);
    }

  //  for (Int_t i = 0; i < inputTree->GetEntries(); i++ ) {                                                        
  return v;
}


/************************************************************/

/* Write table (vector of vectors) of unsigned ints to
   output .txt file outFile.  */
int writeTableToText(vector<vector<Double_t>> table,
		     TString outFile)
{
  // Create file
  FILE *fp = fopen(outFile, "w");
  if (fp == NULL)
    {
      printf("ERROR opening .txt file; exiting");
      return 1;
    }

  for (int i = 0; i < table.size(); i++)
    {
      for (int j = 0; j < table[i].size(); j++)
	{
	  //	  fprintf(fp, "%08x", (unsigned int) table[i][j] & 0xFF);
	  //	  printf("Converting %f to %i, which is %08x in hex.\n", table[i][j], (unsigned int) table[i][j], (unsigned int) table[i][j] & 0xFF);
	  fprintf(fp, "%f", table[i][j]);
	  // Put space in-between the entries
	  if (j < (table[i].size() - 1))
	    fprintf(fp, " ");
	}
      
      // Put newline after each row
      if (i < (table.size() - 1))
	fprintf(fp, "\n");
    }
  fprintf(fp, "\n");

  return 0;
}

/************************************************************/

int ntupleToTable_floats()
{
  vector<vector<Double_t>> vars = floatTableFromTree();

  
  writeTableToText(vars, "outputFloats.txt");

  return 0;
}
#endif
