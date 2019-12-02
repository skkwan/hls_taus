#include "BDT.h"
#include "parameters.h"
#include "myproject.h"
int main(){
	input_arr_t x = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	score_arr_t score;
	myproject(x, score);
	std::cout<< "debug" <<std::endl;
	for(int i = 0; i < n_classes; i++){
		std::cout << score[i] << ", ";
	}
	std::cout << std::endl;
	return 0;
}
