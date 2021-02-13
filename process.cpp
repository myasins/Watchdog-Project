/**
 * @author Mehmet Yasin Seremet
 * Bogazici University Computer Engineering Student, 2017400102
 */

/**
 * process.cpp consists of 3 global variables and 3 methods. Global variables are to store the incoming signals,
 * to access the process number and name of the output file. In the main method, process.cpp waits for the signals.
 * Then, it handles the signals in signalHandler methods by only printing the related values or terminating itself.
 */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <csignal>
#include <cstdio>
#include <stdio.h>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>
#include <bits/stdc++.h>

using namespace std;

queue<int> waitingSignals; //!< storing the incoming signals from executor
string processNumb; //!< storing process number
string txtFile; //!< name of the output txt file of the process

/**
 * Function to handle the received signal. It prints the value of the signal. If the signal is SIGTERM, it terminates
 * the process and prints the required message.
 *
 * @param signum signal number related to the received signal
 */

void signalHandler2( int signum ) {

    //opening required txt file
    ofstream processOutput;
    processOutput.open (txtFile, ios::app);

    if(signum!=15){ //just printing the signal value
        processOutput << "P"<<processNumb<<" received signal "<< signum <<endl;
    }
    else if(signum==15) { //printing required information and terminating the process
        processOutput << "P" << processNumb << " received signal " << signum << ", terminating gracefully" << endl;
        exit(signum);
    }
}

/**
 * Function to store the incoming signals in the waitingSignals queue firstly.
 *
 * @param signum signal number related to the received signal
 */
void signalHandler(int signum) {
    waitingSignals.push(signum);
}

/**
 * After the process is created, it prints its id and condition. Then, it just waits for signals to handle
 * according to description. Terminates itself in case of SIGTERM, only prints the value in case of the others.
 *
 * @param argc count of arguments
 * @param argv list of arguments
 * @return -
 */
int main(int argc, char *argv[]) {

    //directing signals
    signal(SIGHUP, signalHandler);
    signal(SIGINT, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGTRAP, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGBUS, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGXCPU, signalHandler);


    //writing the required output to process_out.txt
    ofstream processOutput;
    processNumb = argv[1];
    txtFile = argv[2];
    processOutput.open (txtFile, ios::app);
    processOutput<<"P"<<processNumb<<" is waiting for a signal"<<endl;

    //sending signal to watchdog to inform that the process creation is OK
    kill(getppid(), SIGUSR1);


    //waiting and directing signals to the handler -- preventing exiting the program
    while(1){
        usleep(300000);
        if(waitingSignals.empty()){
            continue;
        }
        int signum = waitingSignals.front();
        waitingSignals.pop();
        signalHandler2(signum);
    }

    processOutput.close();

    return 0;
}


