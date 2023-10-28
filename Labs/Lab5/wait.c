#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char* argv[]){

printf("hello world (pid:%d)\n", (int) getpid());
int rc = fork();
int rc1 = fork();
int rc2 = fork();
int rc3 = fork();

if(rc<0){

fprintf(stderr, "fork failed\n");
exit(1);
}else if(rc == 0)
{
printf("hello world (pid:%d)\n", (int) getpid());
}
else{
int wc = wait(NULL);

printf("hello, I am parent of %d (wc: %d) (pid: %d)\n", rc, wc,(int) getpid());


}




if(rc1<0){

fprintf(stderr, "fork failed\n");
exit(1);
}else if(rc1 == 0)
{
printf("hello world (pid:%d)\n", (int) getpid());
}
else{
int wc1 = wait(NULL);

printf("hello, I am parent of %d (wc: %d) (pid: %d)\n", rc1, wc1,(int) getpid());


}





if(rc2<0){

fprintf(stderr, "fork failed\n");
exit(1);
}else if(rc2 == 0)
{
printf("hello world (pid:%d)\n", (int) getpid());
}
else{
int wc2 = wait(NULL);

printf("hello, I am parent of %d (wc: %d) (pid: %d)\n", rc2, wc2,(int) getpid());


}




return 0;

}
