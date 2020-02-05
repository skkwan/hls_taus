#include "BDT.h"
#include "parameters.h"
#include "myproject.h"
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
#define events_in_file (1)
#define taus_per_event (5)


/***********************************************************************/

/* Returns a vector of vector of floats, read in from the specified 
   text file. */

vector<vector<float>> read_vars_from_file(void){
	ifstream inFile;
	inFile.open("input_realVals.txt");//open the input file
	stringstream strStream;
	strStream << inFile.rdbuf();//read the file
	string str = strStream.str();//str holds the content of the file
	string buf; // Have a buffer string
	stringstream ss(str); // Insert the string into a stream
	vector<string> tokens; // Create vector to hold our words
	while (ss >> buf)
		tokens.push_back(buf);
	vector<vector<float> > v;
	int tokens_per_event = taus_per_event*n_features;
	for (int i = 0; i < events_in_file; i++){
		if (tokens[i * tokens_per_event] == "#Event"){
			cout << "Processing event #: " << tokens[i * tokens_per_event + 1] << endl;
		}
		for (int j = 0; j < taus_per_event; j++){
			vector<float> vars;
			for (int k = 0; k < n_features; k++){
				unsigned int idx = i * tokens_per_event + 2 + j*n_features + k;
				stringstream ss2;
				float x;
				ss2 << tokens[idx].c_str() ;
				ss2 >> x;
				cout << x << endl;
				vars.push_back(x);
			}
			v.push_back(vars);
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
	  cout << "Raw value:\t" <<vars_raw[idx_out] << ".\t";
	  //  input_vars[idx_out] = (vars_raw[idx_out] & 0x3FFFF); // Only keep the 18 least significant bits
	  // Conversion to input_t precision:
	  input_vars[idx_out] = vars_raw[idx_out]; 
	  cout << "Converting to:\t" << input_vars[idx_out] << ".\t";
	}
	cout << endl;
}

/***********************************************************************/



int main(){
	vector< vector<float> > vars_raw = read_vars_from_file();
	cout<<"size:"<<vars_raw.size()<<endl;
	for (int i = 0; i < events_in_file; i++){
	  for (int j = 0; j < taus_per_event; j++){
	    input_arr_t v;
	    unpack_input_vars(vars_raw[j], v);
	    score_arr_t score;
	    myproject(v, score);
	    cout << "score:\t" << score[0] << endl;
	  }	
	}
	return 0;
}

/***********************************************************************/
