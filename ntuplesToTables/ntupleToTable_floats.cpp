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

#define n_events (1)
#define n_taus (20) 
#define n_features (7)


/************************************************************/

vector<vector<Double_t>> floatTableFromTree(bool writeEventNumber, bool writeTMVAScore)
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
  
  Int_t event;
  Double_t l1Discriminant;

  // Set branch addresses
  inputTree->SetBranchAddress("event", &event);

  inputTree->SetBranchAddress("l1Pt", &l1Pt);
  inputTree->SetBranchAddress("l1Eta", &l1Eta);
  inputTree->SetBranchAddress("l1StripPt", &l1StripPt);
  inputTree->SetBranchAddress("l1DM", &l1DM);
  inputTree->SetBranchAddress("l1PVDZ", &l1PVDZ);
  inputTree->SetBranchAddress("l1HoE", &l1HoE);
  inputTree->SetBranchAddress("l1EoH", &l1EoH);

  inputTree->SetBranchAddress("l1Discriminant", &l1Discriminant);

  // Declare table
  vector<vector<Double_t>> v;

  inputTree->GetEntry(0);
  int currentEvent = event;
  
  for (int i = 0; i < n_taus; i++)
    {
      inputTree->GetEntry(i);

      vector<Double_t> vars;

      if (writeEventNumber)
	vars.push_back((Double_t) event);

      vars.push_back(l1Pt);
      vars.push_back(l1Eta);
      vars.push_back(l1StripPt);
      vars.push_back(l1DM);
      vars.push_back(l1PVDZ);
      vars.push_back(l1HoE);
      vars.push_back(l1EoH);

      if (writeTMVAScore)
	vars.push_back(l1Discriminant);
      
      v.push_back(vars);

    }

  return v;
}


/************************************************************/

/* Write table (vector of vectors) of unsigned ints to
   output .txt file outFile.  */
int writeTableToText(vector<vector<Double_t>> table, string header,
		     TString outFile)
{
  // Create file
  FILE *fp = fopen(outFile, "w");
  if (fp == NULL)
    {
      printf("ERROR opening .txt file; exiting");
      return 1;
    }

  // Write the header
  fprintf(fp, "%s", header.c_str());

  for (int i = 0; i < table.size(); i++)
    {
      for (int j = 0; j < table[i].size(); j++)
	{
	  //	  fprintf(fp, "%08x", (unsigned int) table[i][j] & 0xFF);
	  //	  printf("Converting %f to %i, which is %08x in hex.\n", table[i][j], (unsigned int) table[i][j], (unsigned int) table[i][j] & 0xFF);
	  fprintf(fp, "%f", table[i][j]);
	  
	  // Put tab in-between the entries
	  if (j < (table[i].size() - 1))
	    fprintf(fp, "\t");
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
  vector<vector<Double_t>> vars = floatTableFromTree(false, false);
  vector<vector<Double_t>> eventVarsScore = floatTableFromTree(true, true);

  string header1 = ""; // "#l1Pt\t l1Eta\t l1StripPt\t l1DM\t l1PVDZ\t l1HoE\t l1EoH\n";
  string header2 = "#Event\t l1Pt\t l1Eta\t l1StripPt\t l1DM\t l1PVDZ\t l1HoE\t l1EoH\t l1Discriminant\n";

  writeTableToText(vars, header1, "input_realVals.txt");
  writeTableToText(eventVarsScore, header2, "all_realVals.txt");

  return 0;
}
#endif
