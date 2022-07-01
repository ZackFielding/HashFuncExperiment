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

void findIntersection(double* hashArray,
		int* c_array, const int& c_arraySize,
	   	std::pair<int, int*>& pair){
	
	int keyReturned {};
	for(size_t o {0}; o < c_arraySize; ++o){
		keyReturned = hashing(c_array[o]);
		if(hashArray[keyReturned*4] == c_array[o] ||
			hashArray[keyReturned*4+2] == c_array[o]){
			pair.second[o] = c_array[o];
			++(pair.first);
			}
		}
}

void generateMap(std::map<int, bool> &map, 
		int* array, const int& arraySize){
	
	for(size_t i {0}; i < arraySize; ++i)
		map.insert({array[i], true});

	return;	
}

int main(){

	int array1[] {1,2,3,4,5}, array2[] {0,2,6,8,9}, array3[5]{0};
	const int array1size {5}, array2size {5}, array3size{5};

	std::map<int, bool> map;
	generateMap(map, array1, array1size); // generate ordered map	

	double* myMap {nullptr};
	myMap = customMap(array1, array1size, hashing, keyIndex);	

	std::pair<int, int*> arrayInterPair ({0, array3});
	findIntersection(myMap, array2, array2size, arrayInterPair);

	if(arrayInterPair.first > 0){
		std::cout << "Array intersection found: [";
		for(size_t a {0}; a < arrayInterPair.first; ++a){
			std::cout << arrayInterPair.second[a];
			if(a < arrayInterPair.first - 1)
				std::cout << " ";
		}	
		std::cout << "].\n";
	}	

	// free heap
	for(double* &ptr : hashArrayVector)
		delete [] ptr;

	return 0;
}
