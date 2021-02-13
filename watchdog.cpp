/**
 * @author Mehmet Yasin Seremet
 * Bogazici University Computer Engineering Student, 2017400102
 */

/**
 * This project aims that creating a watchdog which keeps track of its child processes and if one of them is killed,
 * recreate it with some special cases. Firstly, watchdog creates the given number of child processes. Afterwards, it
 * sleeps until a child process is killed. Then, it does its recreation&printing job according to specified information.
 * When the instructions are completed, watchdog is killed by executor.
 */

/**
 * watchdog.cpp consists of a global variable and 2 methods. Global variable is to access the output file.
 * In the main method, watchdog creates processes and sleeps until one of them is terminated. When one child is killed,
 * watchdog does the required recreation&printing job. There is special case for P1. At the end, watchdog is killed by
 * executor.
 */
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <csignal>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <map>
#include <bits/stdc++.h>

using namespace std;

string watchdogTxt; //!< output txt file of watchdog

/**
 * Function to handle the received signal. While creating processes, watchdog waits the signal informing
 * that the process is creation is OK to prevent synchronization problems. At the end, it handles SIGTERM.
 *
 * @param signum signal number related to the received signal
 */
void signalHandler( int signum ) {

    //opening required txt file
    ofstream watchdogOutput;
    watchdogOutput.open (watchdogTxt, ios::app);

    if(signum==15){ //terminating process and writing required information if SIGTERM is signalled
        watchdogOutput<<"Watchdog is terminating gracefully"<<endl;
        exit(signum);
    }
    else{
    }
}
/**
 * Firstly, it writes its own info to pipe. Afterwards, it creates the given number of processes and write their info to
 * pipe. Then, it sleeps until a process is killed. When a process is killed, it does the recreation&printing job
 * according to which process is killed. If P1 is killed, other processes are also killed and recreated. At the end,
 * watchdog is killed by executor.
 *
 * @param argc count of arguments
 * @param argv list of arguments
 * @return -
 */
int main(int argc, char *argv[]) {

    //directing signals
    signal(SIGUSR1, signalHandler);
    signal(SIGTERM, signalHandler);

    //saving the number of processes and preparing the files to write
    int numbOfProcesses = stoi(argv[1]);
    ofstream watchdogOutput,processOutput;
    watchdogTxt=argv[3];
    watchdogOutput.open(watchdogTxt);
    processOutput.open(argv[2]);

    //preparing pipe
    int unnamedPipe;
    char* myfifo = (char*) "/tmp/myfifo";
    unnamedPipe = open(myfifo,O_WRONLY);

    pid_t pid; // process id
    pid_t childPidArray[numbOfProcesses+1]; // array to store child pids

    pid_t watchdogPid = getpid(); //pid of watchdog
    string processInfo = "P"+to_string(0)+" "+to_string(watchdogPid);

    // copying the string to char_array and writing to pipe
    int n = processInfo.length();
    char char_array[n + 1];
    strcpy(char_array, processInfo.c_str());
    write(unnamedPipe,char_array, n+1);


    //creating given number of processes and printing required information
    for(int i=1;i<=numbOfProcesses;i++) {

        pid = fork();

        if (pid < 0){ // error occurred returns -1
            fprintf(stderr, "Fork Failed");
            return 1;
        }
        else if (pid == 0){ // execution of the child process
            execlp("./process", "./process", (char*)to_string(i).c_str(), (char*)argv[2], (char*)NULL);
        }
        else{ // execution of the parent process
            pause();

            childPidArray[i]=pid;
            string processInfo = "P"+to_string(i)+" "+to_string(pid);

            // copying the string to char_array and writing to pipe
            int n = processInfo.length();
            char char_array[n + 1];
            strcpy(char_array, processInfo.c_str());
            write(unnamedPipe,char_array,n+1);

            watchdogOutput<<"P" << i << " is started and it has a pid of " << pid << endl;
        }
    }

    pid_t deadChildId; //pid of the dead child

    //waiting until a child is terminated
    while((deadChildId=wait(NULL))>0) {

        if(childPidArray[1]==deadChildId){
            watchdogOutput<<"P1 is killed, all processes must be killed"<<endl;
            watchdogOutput<<"Restarting all processes"<<endl;

            for(int i=2;i<=numbOfProcesses;i++){ //killing rest of the processes
                kill(childPidArray[i],SIGTERM);
                usleep(300000);
            }

            //recreating the processes
            for(int i=1;i<=numbOfProcesses;i++) {

                pid = fork();

                if (pid < 0){ // error occurred returns -1
                    fprintf(stderr, "Fork Failed");
                    return 1;
                }
                else if (pid == 0){ // execution of the child process
                    execlp("./process", "./process", (char*)to_string(i).c_str(), (char*)argv[2], (char*)NULL);
                }
                else{ // execution of the parent process
                    pause();

                    childPidArray[i]=pid;
                    string processInfo = "P"+to_string(i)+" "+to_string(pid);

                    // copying the string to char_array and writing to pipe
                    int n = processInfo.length();
                    char char_array[n + 1];
                    strcpy(char_array, processInfo.c_str());
                    write(unnamedPipe,char_array,n+1);

                    watchdogOutput<<"P" << i << " is started and it has a pid of " << pid << endl;
                }
            }
        }
        else{
            int tempProcessID=0;
            for(int i=1;i<=numbOfProcesses;i++){ //checking which child is terminated
                if(childPidArray[i]==deadChildId){
                    tempProcessID=i;
                }
            }

            if(tempProcessID==0){

            }
            else {
                watchdogOutput << "P" << tempProcessID << " is killed" << endl;
                watchdogOutput << "Restarting P" << tempProcessID << endl;

                pid = fork();

                if (pid < 0) { // error occurred returns -1
                    fprintf(stderr, "Fork Failed");
                    return 1;
                }
                else if (pid == 0) { // execution of the child process
                    execlp("./process", "./process", (char*)to_string(tempProcessID).c_str(), (char*)argv[2], (char*)NULL);
                }
                else { // execution of the parent process
                    pause();

                    childPidArray[tempProcessID] = pid;
                    string processInfo = "P" + to_string(tempProcessID) + " " + to_string(pid);

                    // copying the string to char_array and writing to pipe
                    int n = processInfo.length();
                    char char_array[n + 1];
                    strcpy(char_array, processInfo.c_str());
                    write(unnamedPipe, char_array, n + 1);

                    watchdogOutput << "P" << tempProcessID << " is started and it has a pid of " << pid << endl;
                }
            }
        }
    }

    close(unnamedPipe);

    //waiting signals and preventing exiting the program
    while(1){
        usleep(500000);
    }


}