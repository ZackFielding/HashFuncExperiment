#import <iostream>
#import <cmath> 
#import <limits>
#import <map>
#import <thread>
#import <vector>

	// global var
std::vector<double*>hashArrayVector; 

double* customMap (int *array, const int &arraySize, 
		int(*hashing)(const int &),
		void(*keyIndex)(int, int, double *)){

	const int hashArraySize {42}; //set to 42 for simplicity
	static int currentVector {0};
	hashArrayVector.push_back(new double [hashArraySize]);
		//vec containing double ptrs to heap

		// initialize all values to NAN
	for(size_t n {0}; n < hashArraySize; ++n)
		hashArrayVector.at(currentVector)[n] = 
			std::numeric_limits<double>::quiet_NaN();

		//i (hashing()) -> hash current value to get key
		//ii (keyIndex())-> index the key into the double[] on heap 
	for(size_t k {0}; k < arraySize; ++k)
		keyIndex(hashing(array[k]), array[k], 
				hashArrayVector.at(currentVector));  
	
	++currentVector;
	return hashArrayVector.at(currentVector-1);
}

int hashing(const int &toHash){

	const int key {toHash * toHash};	
	if(key < 10)
		return key;
	else if(key >= 10 && key <= 99)
		return (key % 10);
	else if(key == 100)
	   return (key % 100);
	else{
		std::cerr << "Error in key determination for " 
			<< toHash << std::endl;	
		return -1; //throw error down the line
	}
}

void keyIndex(int key, int toHash, double* hashArray){

	if(std::isnan(hashArray[key*4])){
		hashArray[key*4] = toHash;
		hashArray[key*4+1] = 1;
	}else if(std::isnan(hashArray[key*4+2])){
		hashArray[key*4+2] = toHash;
		hashArray[key*4+3] = 1;	
	}else{
		std::cerr << "Error in key index ..." <<
			toHash << std::endl;
	}

}

bool findValue(int key, double* hashArray){

	int keyReturned {hashing(key)};

	if(hashArray[keyReturned*4] == key)
		return true;
	else if(hashArray[keyReturned*4+2] == key)
		return true;
	else
		return false;
}

void generateMap(std::map<int, bool> &map, 
		int* array, const int& arraySize){
	
	for(size_t i {0}; i < arraySize; ++i)
		map.insert({array[i], true});

	return;	
}

int main(){

	int array1[] {1,2,3,4,5}, array2[] {0,2,6,8,9}, array3[5]{0};
	const int array1size {5};

	std::map<int, bool> map;
	generateMap(map, array1, array1size); // generate ordered map	

	double* myMap {nullptr};
	myMap = customMap(array1, array1size, hashing, keyIndex);	

	// free heap
	for(double* &ptr : hashArrayVector)
		delete [] ptr;

	return 0;
}
