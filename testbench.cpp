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

#define events_in_file (1)
#define taus_per_event (5)
#define n_features (7)

/***********************************************************************/

/* Returns a vector of vector of floats, read in from the specified 
   text file. */

vector<vector<float>> read_vars_from_file(void){
  ifstream inFile;
  inFile.open("/afs/cern.ch/work/s/skkwan/public/triggerDevel/hls_taus/input_realVals.txt");//open the input file

  vector<string> tokens; // Create vector to hold all of the tokens
  
  string line;
  string substr;
  while (getline(inFile, line, '\n'))
    {
      std::cout<< line << std::endl;
      istringstream ss(line);

      while (getline(ss, substr, '\t'))
	{
	  tokens.push_back(substr);
	  std::cout << substr << std::endl;
	}
    }

  // Now parse the tokens into a 2D array
  vector<vector<float> > v;
  int tokens_per_event = taus_per_event*n_features;

  // Loop throgh events in file
  for (int i = 0; i < events_in_file; i++)
    {
      // Loop through taus in each event
      for (int j = 0; j < taus_per_event; j++)
	{
	  vector<float> vars;
	  for (int k = 0; k < n_features; k++)
	    {
	      unsigned int idx = (i * tokens_per_event) + (j * n_features) + k;
	      std::cout << "idx: " << idx << ". ";
	      stringstream ss2;
	      float x;
	      ss2 << tokens[idx].c_str() ;
	      ss2 >> x;
	      cout << x << " ";
	      vars.push_back(x);
	      
	    }
	  v.push_back(vars);
	  cout << endl;
	}
    }
  return v;
}

/***********************************************************************/

/* Takes a vector of floats vars_raw and stores them in an input_arr_t
   object input_vars. */

void unpack_input_vars(vector<float>  vars_raw, input_arr_t input_vars){
  for (unsigned int idx_out = 0; idx_out < n_features; idx_out++){
    //input_vars[idx_out] = vars_raw[idx_out];
    cout << "Raw value: " <<vars_raw[idx_out] << ". ";
    //  input_vars[idx_out] = (vars_raw[idx_out] & 0x3FFFF); // Only keep the 18 least significant bits

    // Conversion to input_t precision:
    input_vars[idx_out] = vars_raw[idx_out]; 
    cout << "Converting to: " << input_vars[idx_out] << "\n";
  }
  cout << endl;
}

/***********************************************************************/


int main(){
  vector< vector<float> > vars_raw = read_vars_from_file();
  cout<<"size:"<<vars_raw.size()<<endl;

  // Initialize file to write to
  FILE *fOut;
  fOut = fopen("/afs/cern.ch/work/s/skkwan/public/triggerDevel/hls_taus/hlsBDTscore.txt", "w");
  if (fOut == NULL) 
    std::cout << "WARNING: Error opening file" << std::endl;
  fprintf(fOut, "# HLS BDT score\n");

  for (int i = 0; i < events_in_file; i++){
    for (int j = 0; j < taus_per_event; j++){
      input_arr_t v;
      unpack_input_vars(vars_raw[j], v);
      score_arr_t score;
      myproject(v, score);
      cout << "score:\t" << score[0] << endl;

      // Also write the score to the file, casting it to a float
      fprintf(fOut, "%f\n", (float) score[0]);
    }
  }
  return 0;
}


/***********************************************************************/
