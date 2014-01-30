/*
 * test_hash_pseudo_random.cpp
 *
 *  Created on: 2014.01.21.
 *      Author: mrita
 */

#include <iostream>
#include <fstream>

#include "../../main/common/random/pseudo_random.h"
#include "../../main/common/random/hash_pseudo_random.h"

using namespace std;

void test01(){
  HashPseudoRandom random(12); 
  ofstream myfile ("test_hash_output1.txt");
  if (myfile.is_open()){
    for(long n=1; n<1000000000000000000; n*=2){
      myfile<<"n="<<n<<endl;
      for(short i=0; i<100; i++ ){
	myfile<<"i="<<i<<"\t";
	for(short j=0; j<10; j++){
	  myfile<<random.get(i,j,n)<< " ";
	}
	myfile<<endl;
      }
      myfile<<endl;
    }
    myfile.close();
  }
  else cout << "Unable to open file";
}

void test02(){
  HashPseudoRandom random(29); 
  ofstream myfile ("test_hash_output2.txt");
  if (myfile.is_open()){
    for(long n=1; n<1000000000000000000; n*=2){ //100000000000001
      for(short i=0; i<100; i++ ){
	for(short j=0; j<10; j++){
	  myfile<<random.get(i,j,n)<< endl;
	}
      }
    }
    myfile.close();
  }
  else cout << "Unable to open file";
}

void test03(){ 
//itt latszik az oka az azonos hash ertekeknek: tulcsordulas
  long LLL = ((1L<<32)-1);
  long tar;
  for(long n=4294967250; n<4294967350; n++){ 
    tar = n & LLL;
    cout << tar <<"\t"; 
  }
  cout<<endl;
  // cout <<endl<< LLL <<endl;
}

void test04(){
  long LLL = ((1L<<32)-1);
  long tar;
  for(long n=1; n<1000000000000000000; n*=2){
    if (n < 4294967295){
     tar = n & LLL;
    } else {
      tar = n >> 32;
    }
     cout << tar <<"\t";
  }
  cout<<endl;
}

void test05() {
	HashPseudoRandom random(13);
	printf("Test05.\n");
	printf("%u.\n", random.get(0, 0, 998));
	//printf("%d\n", random.get());

}

int main (int argc, char **argv) {
  test01();
  test02();
  // test03();
  test04();
  test05();
  return 0;
}
