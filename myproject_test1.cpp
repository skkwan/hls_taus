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
#define taus_per_event (2)

vector<vector<unsigned int>> read_vars_from_file(void){
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
	vector<vector<unsigned int> > v;
	int tokens_per_event = taus_per_event*n_features;
	for (int i = 0; i < events_in_file; i++){
		if (tokens[i * tokens_per_event] == "#Event"){
			cout << "Processing event #: " << tokens[i * tokens_per_event + 1] << endl;
		}
		for (int j = 0; j < taus_per_event; j++){
			vector<unsigned int>  vars;
			for (int k = 0; k < n_features; k++){
				unsigned int idx = i * tokens_per_event + 2 + j*n_features + k;
				stringstream ss2;
				unsigned int x;
				ss2 << hex << tokens[idx].c_str() ;
				ss2 >> x;
				cout << x << endl;
				vars.push_back(x);
			}
			v.push_back(vars);
		}
	}
	return v;
}

void unpack_input_vars(vector<unsigned int>  vars_raw, input_arr_t input_vars){
	for (unsigned int idx_out = 0; idx_out < n_features; idx_out++){
	  //input_vars[idx_out] = vars_raw[idx_out];
	  input_vars[idx_out] = (vars_raw[idx_out] & 0x3FFFF); // Only keep the 18 least significant bits
	  cout << input_vars[idx_out] << endl;
	}
}

int main(){
	vector< vector< unsigned int> > vars_raw = read_vars_from_file();
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
