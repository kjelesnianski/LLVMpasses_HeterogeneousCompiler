#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "randdp.h"

#define NX 20
#define NY 200
#define NZ 300

//static test vars
static long iamverylong __attribute__ ((used)); //should go in BSS
static long metoo __attribute__ ((used)) = 5555555555555555; //should go in DATA
static double someconstant __attribute__ ((used))= 22.111101111; //should go in DATA
static const double constantstatic __attribute__ ((used)) = 22.111101111; //should go in RODATA
static int aqqq __attribute__ ((used)); //should go in BSS

//TLS __thread initialization image test vars
__thread long blubluthread;
__thread int babablutthread = 5556;
__thread int aqqq_threader = 22223;
__thread double somethreaddouble;

//normal test vars
long blublu;
long blublu0 = 0;
long blubluNULL = NULL;
//static long metoo __attribute__ ((packed, used)) = 5555555555555555;
long const nene = 2222222222222;
char* ddd= "this train man";
//static double someconstant __attribute__ ((aligned, packed)) = 22.111101111;
double const someconstantconst = 22.111101111;
double otherconstand = 999.555555;
double const otherconstandconst = 999.555555;
//static int aqqq __attribute__ ((aligned, packed, used));
int bwwww;


char bar(char b){
	printf("%c",b);
	b+= 7;
	return b;
}

//static int foo(int f) __attribute__ ((used));
static char foo(char f){
	printf("%d\n",f);
	char res = f +4;
	char new;
	int i;
	for(i = 0; i < NX; ++i){
          new =bar(f);
          //printf("hello %d\n",new);
        }
	return res;
}

double dou(double d){
	double rs = 3.5;
	int i,j;
	someconstant -= 12;
	for(i=0;i<10;++i){
	  for(j=0;j<2;++j){
	    rs += someconstant;
	    rs -= someconstantconst;
	    rs += someconstant;
	    rs += constantstatic;
	  }
	  rs-= d;
	}
	rs -= 100.5;
	rs = randlc(&rs,15);
	return rs;
}

long actuallyUse(long input){
  printf("hi there long static");
  long res_long = input + metoo;
  printf("%c",ddd);
  return res_long;
}
	

int main(int argc, char* argv[]){
  
  iamverylong = 888888888888888;
  blublu = 1;

  int iter = 2;
  aqqq = 7;
  char somechar = 7;
  printf("\n\n CHRIS Parallel Benchmarks (EEE44.4-SER-C) - Benchmark\n\n");
  printf(" Size                : %4dx%4dx%4d\n", NX, NY, NZ);
  printf(" Iterations          :     %10d\n", iter);
  printf(" VERY          :     %lu\n", iamverylong);
  printf(" some static          :     %f\n", someconstant);
  int new = foo(aqqq);
  printf("\n\n"); 
  char nextchar =  bar(somechar);
  printf("Next Character:%c\n",nextchar);

  long actual = actuallyUse(9);
  actual+= iamverylong;
  actual-= metoo;
  printf("Next Long:%lu\n",actual);
  
  double dd = dou(4.2);
  printf("Next Double:%f\n",dd);


  char prev = nextchar;
  for(iter = 0; iter < NX; ++iter){
  	char new = foo(prev);
	printf("hello %d\n",new);
	prev = new;
  }
  
  printf("Benchmark Complete\n");
  return 0;
}

