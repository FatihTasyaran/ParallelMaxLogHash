#include <iostream>
#include <string>
#include <fstream>
#include <omp.h>
#include <random>
#include <bitset>
#include <math.h>

//#define totalShingles (1ULL<<32)-1
#define totalShingles (1<<31)-1


std::random_device rd;
std::mt19937 e{rd()}; // or std::default_random_engine e{rd()};
std::uniform_int_distribution<int> dist{1, totalShingles};

void hash_parameter(int* randList, int k){

  int cursor = 0;
  int randIndex = dist(e);

  randList[cursor] = randIndex;
 
  while(k > 0){
    randList[cursor] = dist(e);
    cursor += 1;
    k -= 1;
  } 
}

void MaxLog(int* setA, int* setB, int* randomNoA, int* randomNoB,
	    char* setAvals, char* setBvals,
	    std::bitset<128> &setAsigs, std::bitset<128> &setBsigs, int cardinal, int k){

  for(int i = 0; i < k; i++){
    setAvals[i] = -1;
    //std::cout << "Setting: " << (int)setAvals[i] << std::endl;
    setBvals[i] = -1;
  }
    

  //For Set A
  for(int i = 0; i < cardinal; i++){
    for(int x = 0;  x < k; x++){
      
      double temp = (randomNoA[x] * setA[i] + randomNoB[x]) % totalShingles;

      if(temp <= 0)
	temp += totalShingles;
      
      temp /= totalShingles;
      
      //std::cout << "Temp: " << temp << std::endl;
      
      double log_temp = log2(temp);
      log_temp = -1*log_temp;
      
      //std::cout << "Log Temp: " << log_temp << std::endl;
      int hash_val = ceil(log_temp);
      
      if(hash_val > setAvals[x]){
	setAvals[x] = hash_val;
	setAsigs[x] = 1;
	//std::cout << "Just setting: " << setAsigs[x] << std::endl; 
      }
      
      else if(hash_val == setAvals[x]){
	setAsigs[x] = 0;
      }       
      
    }
  }


  //For Set B
  for(int i = 0; i < cardinal; i++){
    for(int x = 0;  x < k; x++){
      
      double temp = (randomNoA[x] * setB[i] + randomNoB[x]) % totalShingles;

      if(temp <= 0)
	temp += totalShingles;
      
      temp /= totalShingles;
      
      //std::cout << "Temp: " << temp << std::endl;
      
      double log_temp = log2(temp);
      log_temp = -1*log_temp;
      
      //std::cout << "Log Temp: " << log_temp << std::endl;
      int hash_val = ceil(log_temp);
      
      if(hash_val > setBvals[x]){
	setBvals[x] = hash_val;
	setBsigs[x] = 1;
      }
      
      else if(hash_val == setBvals[x]){
	setBsigs[x] = 0;
      }       
      
    }
  }


  for(int i = 0; i < k; i++){
    std::cout << "SetA val " << i << ": " << (int)setAvals[i] << " sig: " << setAsigs[i] << std::endl;
  }
  
}

double estimate(char* setAvals, char* setBvals, std::bitset<128> setAsigs, std::bitset<128> setBsigs, int k){

  double jaccard_similarity = 0;
  int con = 0;
  
  for(int i = 0; i < k ; i++){

    //std::cout << "i:" << i << "setAvals[i]: " << (int)setAvals[i] << " setAsigs[i]: " << setAsigs[i] <<
    //  "     setBvals[i]: " << (int)setBvals[i] << " setBsigs[i]: " << setBsigs[i] << std::endl;

    if((setAvals[i] > setBvals[i]) && (setAsigs[i] == 1)){
      con += 1;
    }
    else if((setAvals[i] < setBvals[i]) && (setBsigs[i] == 1)){
      con += 1;
    }
  }

  double dk = k;
  double cofactor = 1.0/0.7213;
  
  jaccard_similarity = 1.0 - con*(1/dk) * (1/0.7213);

  //std::cout << "con: " << con << " dk: " << dk << std::endl;
  //std::cout << "Similarity is: " << jaccard_similarity << std::endl;

  return jaccard_similarity;
  
}


int main(int argc, char** argv){

  std::fstream filer(argv[1]);
  int k = std::atoi(argv[2]);

  int cardinal;
  int value;

  filer >> cardinal;

  std::cout << "Cardinality of stream: " << cardinal << std::endl;

  int* setA = new int[cardinal];
  int* setB = new int[cardinal];

  for (int i = 0; i < cardinal; i++){
    filer >> value;
    setA[i] = value;
    filer >> value;
    setB[i] = value;
  }

  /*
  for (int i = 0; i < cardinal; i++){
    std::cout << "i: " << i << " a: " << setA[i] << " b: " << setB[i] << std::endl;
  }
  */

  int* randomNoA = new int[k+1];
  int* randomNoB = new int[k+1];

  hash_parameter(randomNoA, k);
  hash_parameter(randomNoB, k);

  /*for(int i = 0; i < k; i++){
    std::cout << "Rand a: " << randomNoA[i] << " Rand b: " << randomNoB[i] << std::endl;
    }*/

  char* setAvals = new char[k];
  std::bitset<128> setAsigs;

  char* setBvals = new char[k];
  std::bitset<128> setBsigs;

  
  double start = omp_get_wtime();
  MaxLog(setA, setB, randomNoA, randomNoB, setAvals, setBvals, setAsigs, setBsigs, cardinal, k);
  double duration = omp_get_wtime() - start;
  
  double estimation = estimate(setAvals, setBvals, setAsigs, setBsigs, k);

  std::cout << "Estimation: " << estimation << std::endl;
  std::cout << "Estimated Time: " << duration << std::endl;
    
  return 0;

}
