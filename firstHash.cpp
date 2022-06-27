#import <iostream>
#import <cmath> 
#import <limits>
#import <map>
#import <thread>
#import <vector>

// [] fix compiler error related to returning std::map<>
// [] make sure both custom and std::map hashing works
// [] adjust threading

	// global var
std::vector<double*>hashArrayVector; 

double* customMap (int *array, const int &arraySize, int(*hashing)(const int &),
		void(*keyIndex)(int, int, double *)){

	const int hashArraySize {42}; //set to 42 for simplicity
	static int currentVector {0};
	hashArrayVector.push_back(new double [hashArraySize]);

	for(size_t n {0}; n < hashArraySize; ++n)
		hashArrayVector.at(currentVector)[n] = std::numeric_limits<double>::quiet_NaN();

	for(size_t k {0}; k < arraySize; ++k)
		keyIndex(hashing(array[k]), array[k], hashArrayVector.at(currentVector));  
	
	++currentVector;
	return hashArrayVector.at(currentVector-1); // -1 to account for previous++	
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
		std::cerr << "Error in key determination for " << toHash << std::endl;	
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

void customHashRun(double* customHashArray, int array1[], const int &array1size, 
		int(*hashing)(const int &), void(*keyIndex)(int, int, double *)){

	customHashArray = customMap(array1, array1size, hashing, keyIndex);
	std::cout << "Custom hash func completed." << std::endl;	
	return;
}

// compiler errror related to returning std::map<>
std::map<int, int> standardMapRun(int array1[], int array1size){
	
	std::map<int, int> standardMap(array1[0], array1[array1size-1]);
	std::cout << "Standard map completed." << std::endl;
	return standardMap;
}

int main(){

	int array1[] {1,2,3,4,5}, array2[] {0,2,6,8,9}, array3[5]{0};
	const int array1size {5};

	double *customHashArray {nullptr};
	std::map<int, int> mapRecieve;

	std::thread customThread (customHashRun, customHashArray, array1, array1size,
			hashing, keyIndex);
	mapRecieve = standardMapRun(array1, array1size);	

	customThread.join(); // wait for thread 2 to finish

	// free heap
	for(double* ptr : hashArrayVector)
		delete [] ptr;

	return 0;
}
