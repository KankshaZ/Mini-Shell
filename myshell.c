#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


#define MAX_BUFFER 1024                        
#define MAX_ARGS 64                         
#define SEPARATORS " \t\n"                     
   
int main (int argc, char ** argv)
{
    char buf[MAX_BUFFER];                     
    char * args[MAX_ARGS];                     // pointers to arg strings           
    char ** arg;                               // working pointer thru args
    char * prompt = "kavach$ " ;             
    int status, ret, pid;
    int fd0,fd1,i,in=0,out=0;
    char input[64],output[64];


    
    char * preStatement = "A Simple Shell program created by Kanksha.\nVersion 0.1\n==========================================================================\n" ;

    fputs (preStatement, stdout);  

    //keep reading input till the quit command or eof

    while (!feof(stdin)) { 
    

  
        fputs (prompt, stdout);                
        if (fgets (buf, MAX_BUFFER, stdin )) { 

            //tokenizing input to args array
            arg = args;
            *arg++ = strtok(buf,SEPARATORS);   // tokenize input
            while ((*arg++ = strtok(NULL,SEPARATORS)));
                                              

            //if there is something inside
            if (args[0]) {                     
                

                if (!strcmp(args[0],"quit")) 
                {
                    fputs("Thanks for using our shell!\n", stdout);
                    break;   
                }  

                
                pid = fork();
                if(pid == 0)
                {
                    
                    //check if '>' or '<' are there for I/O redirection
                    for(i=0;args[i]!='\0';i++)
                    {
                        if(strcmp(args[i],"<")==0)
                        {        
                            args[i]=NULL;
                            strcpy(input,args[i+1]);
                            in=1;           
                        }               

                        if(strcmp(args[i],">")==0)
                        {      
                            args[i]=NULL;
                            strcpy(output,args[i+1]);
                            out=1;
                        }         
                    }

                    
                    if(in)
                    {   

                        
                        int fd0;
                        if ((fd0 = open(input, O_RDONLY, 0)) < 0) {
                            perror("Couldn't open input file");
                            exit(0);
                        }           
                        
                        dup2(fd0, 0); 

                        close(fd0); 
                    }

                    
                    if (out)
                    {

                        int fd1 ;
                        if ((fd1 = creat(output , 0644)) < 0) {
                            perror("Couldn't open the output file");
                            exit(0);
                        }          

                        dup2(fd1, STDOUT_FILENO); 
                        close(fd1);
                    }

                    execvp(*args, args);
                    printf("Error, sigh. Fix it and try again dost.\n");
                    exit(pid);
                }
                wait(&status);
            }
        }
    }
    return 0; 
}
