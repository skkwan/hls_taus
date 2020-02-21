#include "BDT.h"
#include "parameters.h"
#include "myproject.h"
#include <vector>
#include <array>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
#define n_taus (10)
#define n_tauBranches (9)  // event, seven BDT variables, and the TMVA discriminant
#define n_variables (7)

/***********************************************************************/

/* Returns a vector of vector of floats, read in from the specified 
   text file. */

vector<vector<float>> read_vars_from_file(void){

  // Open the input file
  ifstream inFile;
  inFile.open("input_realVals.txt");
  
  // Skip the first 1000 characters or the first newline character,
  // whichever comes first.
  inFile.ignore(1000, '\n');

  stringstream strStream;
  strStream << inFile.rdbuf();//read the file
  string str = strStream.str();//str holds the content of the file
  string buf; // Have a buffer string
  stringstream ss(str); // Insert the string into a stream
  vector<string> tokens; // Create vector to hold our words
  while (ss >> buf)
    tokens.push_back(buf);
  vector<vector<float> > v;
  
  for (int i = 0; i < n_taus ; i++){
    vector<float> v_row;
    for (int j = 0; j < n_tauBranches; j++){
      unsigned int idx = (i * n_tauBranches) + j;
      stringstream ss2;
      float x;
      ss2 << tokens[idx].c_str();
      ss2 >> x;
      v_row.push_back(x);

      if (j == (n_tauBranches - 1))
	printf("Last row element is %f\n", x);
      else
	printf("Row %i, column %i, variable is %f\n", i, j, x);
      
    }
    v.push_back(v_row);
  }
  
  return v;
}

/***********************************************************************/

/* Takes a vector of floats vars_raw and stores them in an input_arr_t
   object input_vars. */

void unpack_input_vars(vector<float> row_raw, input_arr_t input_vars, int event, float l1Discriminant){

  event = (int) row_raw[0];
  l1Discriminant = (float) row_raw[n_tauBranches - 1];

  printf("[Unpack:] Event is %i. \n", event);

  // v keeps track of which BDT input variable we are on.
  for (unsigned int v = 0; v < n_variables; v++)
    {
      //input_vars[idx_out] = vars_raw[idx_out];
      //cout << "Raw value:\t" <<vars_raw[idx_out] << ".\t";
      
      //  input_vars[idx_out] = (vars_raw[idx_out] & 0x3FFFF); // Only keep the 18 least significant bits

      // +1 offset because Event number is in the first column.
      printf("[Unpack:] Variable number %i = %f\n", v, row_raw[v+1]);
      input_vars[v] = row_raw[v+1]; 

      // cout << "Converting to:\t" << input_vars[idx_out] << ".\t";
    }
  printf("[Unpack:] l1Discriminant is %f. \n", l1Discriminant);
}

/***********************************************************************/



int main(){
  vector< vector<float> > vars_raw = read_vars_from_file();
  
  FILE *fOut;
  
  fOut = fopen("output.txt", "w");
  
  if (fOut == NULL) 
    {
      printf("\n Error opening file");
    }
  
  //	cout<<"size:"<<vars_raw.size()<<endl;
  
  // Write header
  fprintf(fOut, "# Event  l1Pt  l1Eta  l1StripPt  l1DM  l1PVDZ  l1HoE  l1EoH  l1Discriminant  csim-l1Discriminant\n");

  for (int i = 0; i < n_taus; i++){
    int event;
    input_arr_t v;
    float l1Discriminant;
    unpack_input_vars(vars_raw[i], v, event, l1Discriminant);

    fprintf(fOut, "%i ", event);
    printf("[[[Main:]]] Event %i\n", event);
    for (unsigned int j = 0; j < n_variables; j++)
      {
	printf("[[[Main:]]] Row %i and variable %j = %f\n", i, j, (float) v[i][j]);
	
	fprintf(fOut, "%18.8f ", (float) v[i][j]); 
      }
    
    fprintf(fOut, "%f", l1Discriminant);
    printf("[[[Main:]]] Discr is %f\n", l1Discriminant); 
    
    score_arr_t score;
    myproject(v, score);
    printf("[[[Main:]]] Score is %f\n", (float) score[0]);
    fprintf(fOut, "%18.8f\n", (float) score[0]);
  }	

  fclose(fOut);
  return 0;
}

/***********************************************************************/
