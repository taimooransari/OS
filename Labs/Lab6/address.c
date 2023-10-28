#include <stdio.h>
#include <stdlib.h>


void address(int* x, int* y){
int * guess = x + 1;
if(*guess == *y){
printf("%d Value for Y guessed correctly\n", *guess);}
else{

printf("%d Value for Y guessed wrong, actual value is %d.\n", *guess, *y);
}

*y = 100;
if(*guess == *y){
printf("%d Value for Y guessed correctly\n", *guess);}
else{

printf("%d Value for Y guessed wrong, actual value is %d.\n", *guess, *y);
}

} 



int main(int argc, char* argv[]){

int x = atoi(argv[1]);
int y = atoi(argv[2]);

address(&x, &y);



	return 0;

}


