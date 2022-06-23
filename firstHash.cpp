#import <iostream>
#import <cmath> 
#import <limits>

// [x] fixed return 0 issue
// [x] added setNAN() to set entire array to NANs
// [] feed arrays in
// [] multi thread compare

void setNAN(double* array, const int &arraySize){

	for(size_t n {0}; n < arraySize; ++n)
		array[n] = std::numeric_limits<double>::quiet_NaN();
	return;
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

void keyIndex(int key, int &toHash, double* hashArray){

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

int main(){
	// determine intersection of two arrays
	int array1[] {1,2,3,4,5}, array2[] {0,2,6,8,9},
		array3[5]{0};

	//1d array
	const int hashArraySize {44};
	double hashArray[hashArraySize] {}; 
	setNAN(hashArray, hashArraySize); // set all values to NAN

	for(size_t h {0}; h < 5; ++h){
		keyIndex(hashing(array1[h]), array1[h], hashArray);
	}	
	
	int array3size {0};
	for(size_t j {0}; j < 5; ++j){
		if(findValue(array2[j], hashArray)){
			array3[array3size] = array2[j];
			++array3size;
		}
	}
	
	std::cout << "Array intersection: ";
	for(size_t p {0}; p < array3size; ++p)
		std::cout << array3[p] << " ";
	std::cout << std::endl;

	return 0;
}
