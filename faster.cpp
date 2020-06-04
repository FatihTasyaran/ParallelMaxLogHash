#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <random>
#include <bitset>
#include <math.h>

//#define totalShingles (1ULL<<32)-1
#define totalShingles (1<<31)-1


std::random_device rd;
std::mt19937 e{rd()}; // or std::default_random_engine e{rd()};
std::uniform_int_distribution<int> dist{1, totalShingles};

#define K 1024
#define NO_THREADS 8

void hash_parameter(int* randList){

  int cursor = 0;
  int randIndex = dist(e);

  randList[cursor] = randIndex;

  int k = K;
  
  while(k > 0){
    randList[cursor] = dist(e);
    cursor += 1;
    k -= 1;
  } 
}

void insert_one_set(int* set, int* randomNoA, int* randomNoB, char* &setVals, std::bitset<K> &setSigs, int data_cardinal){
  
  for(int i = 0; i < K; i++){
    setVals[i] = -1;
    //std::cout << "Setting: " << (int)setAvals[i] << std::endl;
  }
    

  //For Set A
  for(int i = 0; i < data_cardinal; i++){
    //std::cout << "data cardinal: " << i << std::endl;
    for(int x = 0;  x < K; x++){
      
      double temp = (randomNoA[x] * set[i] + randomNoB[x]) % totalShingles;
      
      
      if(temp <= 0)
	temp += totalShingles;
      
      temp /= totalShingles;
      
      //std::cout << "Temp: " << temp << std::endl;
      
      double log_temp = log2(temp);
      log_temp = -1*log_temp;
      
      //std::cout << "Log Temp: " << log_temp << std::endl;
      int hash_val = ceil(log_temp);
      
      if(hash_val > setVals[x]){
	setVals[x] = hash_val;
	setSigs[x] = 1;
	//std::cout << "Just setting: " << (int)setVals[x] << " ||| " << setSigs[x] << std::endl; 
      }

      else if(hash_val == setVals[x]){
	setSigs[x] = 0;
      }       
      
    }
  }
  /*
  for(int i = 0; i < 10; i++){
    std::cout << "In Insert: " << setVals[i] << " " << setSigs[i] << std::endl;
  }
  */
  
}

void MaxLog(int* setA, int* setB, int* randomNoA, int* randomNoB,
	    char* setAvals, char* setBvals,
	    std::bitset<K> &setAsigs, std::bitset<K> &setBsigs, int cardinal){

  for(int i = 0; i < K; i++){
    setAvals[i] = -1;
    //std::cout << "Setting: " << (int)setAvals[i] << std::endl;
    setBvals[i] = -1;
  }
    

  //For Set A
  for(int i = 0; i < cardinal; i++){
    for(int x = 0;  x < K; x++){
      
      double temp = (randomNoA[x] * setA[i] + randomNoB[x]) % totalShingles;

      if(temp <= 0)
	temp += totalShingles;
      
      temp /= totalShingles;
      
      //std::cout << "Temp: " << temp << std::endl;
      
      double log_temp = log2(temp);
      log_temp = -1*log_temp;
      
      //std::cout << "Log Temp: " << log_temp << std::endl;
      int hash_val = ceil(log_temp);
      //std::cout << "Hash val: " << hash_val << " " << (int)setAvals[x] << std::endl;
      
      std::cout << "Hash val: " << hash_val << " " << (int)setAvals[x] << std::endl;
      if(hash_val > setAvals[x]){
	setAvals[x] = hash_val;
	setAsigs[x] = 1;
	std::cout << "Just setting: " << setAsigs[x] << std::endl; 
      }
      
      else if(hash_val == setAvals[x]){
	setAsigs[x] = 0;
      }       
      
    }
  }


  //For Set B
  for(int i = 0; i < cardinal; i++){
    for(int x = 0;  x < K; x++){
      
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

  /*
  for(int i = 0; i < k; i++){
    std::cout << "SetA val " << i << ": " << (int)setAvals[i] << " sig: " << setAsigs[i] << std::endl;
    }*/
  
}



double estimate(char* setAvals, char* setBvals, std::bitset<K> setAsigs, std::bitset<K> setBsigs){

  double jaccard_similarity = 0;
  int con = 0;

  
  for(int i = 0; i < K ; i++){
    //std::cout << "i: " << i << std::endl;
    //std::cout << "i:" << i << "setAvals[i]: " << (int)setAvals[i] << " setAsigs[i]: " << setAsigs[i] <<
    //"     setBvals[i]: " << (int)setBvals[i] << " setBsigs[i]: " << setBsigs[i] << std::endl;
  
    if((setAvals[i] > setBvals[i]) && (setAsigs[i] == 1)){
      con += 1;
    }
    else if((setAvals[i] < setBvals[i]) && (setBsigs[i] == 1)){
      con += 1;
    }
    else{
      //std::cout << "setAvals: " << (int)setAvals[i] << " " << (int)setBvals[i] << " " << setAsigs[i] << " " << setBsigs[i] << std::endl;
      ;
    }
  }
  

  double dk = K;
  double cofactor = 1.0/0.7213;

  //std::cout << "Con: " << con << std::endl;
  
  jaccard_similarity = 1.0 - con*(1/dk) * (1/0.7213);

  //std::cout << "con: " << con << " dk: " << dk << std::endl;
  //std::cout << "Similarity is: " << jaccard_similarity << std::endl;

  return jaccard_similarity;
  
}


int main(int argc, char** argv){

  //std::fstream filer(argv[1]);
  //int k = std::atoi(argv[2]);
  //int k = 1024;
  
  int data_cardinal;
  int set_cardinal;
  int value;
  
  //filer >> set_cardinal;

  std::ifstream file(argv[1]);
  std::stringstream ss;

  int start_counter = 0;
  int set_counter = -1;
  int card_counter = 0;

  int** sets;
  int* set_sizes;
  
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      //std::cout << line << std::endl;
      if(start_counter == 0){
	ss << line;
	ss >> value;
	set_cardinal = value;
	sets = new int*[set_cardinal];
	set_sizes = new int[set_cardinal];
	start_counter++;
      }
      else{
	std::stringstream sss(line);
	int set_cardinality_counter = 0;
	int cursor = 0;
	while(!sss.eof()){
	  sss >> value;
	  //std::cout << "value: " << value << std::endl;
	  if(set_cardinality_counter == 0){
	    //std::cout << "Allocating: "  << value << std::endl;
	    sets[set_counter] = new int[value];
	    set_sizes[set_counter] = value;
	    set_cardinality_counter++;
	  }
	  else{
	    //std::cout << "About to explode" << std::endl;
	    sets[set_counter][cursor] = value;
	    cursor++;
	  }
	}
      }
      set_counter++;
    }
    file.close();
  }
  

  //std::cout << "Cardinality of stream: " << data_cardinal << std::endl;
  std::cout << "Cardinality of set size: " << set_cardinal << std::endl;

  int* randomNoA = new int[K+1];
  int* randomNoB = new int[K+1];

  hash_parameter(randomNoA);
  hash_parameter(randomNoB);

  /*for(int i = 0; i < k; i++){
    std::cout << "Rand a: " << randomNoA[i] << " Rand b: " << randomNoB[i] << std::endl;
    }*/

  char** setVals = new char*[set_cardinal];

  for(int i = 0; i < set_cardinal; i++){
    setVals[i] = new char[K];
  }

  std::bitset<K>* setSigs = new std::bitset<K>[set_cardinal];

    
  /*
  char* setAvals = new char[k];
  std::bitset<16> setAsigs;

  char* setBvals = new char[k];
  std::bitset<16> setBsigs;
  */
  /*
  for(int i = 0; i < 10; i++){
    std::cout << sets[0][i] << std::endl;
  }
  */
  double sims_zero[128];
  
  double start = omp_get_wtime();
#pragma omp parallel num_threads(NO_THREADS)
  {
#pragma omp  for schedule(guided)
    for(int i = 0; i < set_cardinal; i++){
      insert_one_set(sets[i], randomNoA, randomNoB, setVals[i], setSigs[i], set_sizes[i]);
      //std::cout << "Sketched set " << i << std::endl;
    }
  }
  double insert = omp_get_wtime() - start;

  double cp = omp_get_wtime();
#pragma omp parallel num_threads(NO_THREADS)
  {
#pragma omp  for schedule(guided)
    for(int i = 1; i < set_cardinal; i++){
      double estimation = estimate(setVals[0], setVals[i], setSigs[0], setSigs[i]);
      sims_zero[i-1] = estimation;
    }
  }
  double estimate_duration = omp_get_wtime() - cp;
  

  for(int i = 1; i < set_cardinal; i++){
    std::cout << "Vs " << i << ": " << sims_zero[i] << std::endl;
  }
  
  std::cout << "Insert Time: " << insert << std::endl;
  std::cout << "Estimate Time: " << estimate_duration << std::endl;
  
  return 0;

}
