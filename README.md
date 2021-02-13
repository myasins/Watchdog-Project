# Watchdog-Project
Implementation of watchdog concept with multiprocesses

executor.cpp is the ready one not belonging to me.
watchdog.cpp and process.cpp are my codes.

The general information about the project is below:

Project Description
The project is to be designed in a ﬂexible manner. That means, you will have N processes
and a watchdog, but those N processes will be created by running the same executable ﬁle,
process.c/process.cpp. During the evaluation, we may take N to be 3 or maybe even 120. So,
do not develop your code depending on a speciﬁc value of N.
You are expected to have three C/C++ programs; process, watchdog, and executor with the
properties speciﬁed below. Note that, executor program is provided to you, and you should not
alter anything on the original program.

executor.cpp
1. File Properties:
• The name of the ﬁle is executor.cpp. (This ﬁle is already provided to you. Do not
modify.)
2. Operations:
• Program reads instructions from instructions.txt.
• Creates a named pipe to be shared with watchdog program.
• Reads “# <PID>” tuples from the read end of the pipe (P1 3564, P2 3565 etc.).
• Detects whether it is a signal or wait command.
• Sends the intended signal to the given process.
• When instructions are completed, ﬁrst it kills the watchdog process, then all of the
other running processes by sending the SIGTERM signal.
2.2 process.c/process.cpp
1. File Properties:
• The name of the ﬁle must be process.c/process.cpp.
2. Process deﬁnition:
• Assume you are asked to create a system with N processes. Name the processes as
P1, P2, P3, P4, ... , PN.
• All processes should work concurrently.
• Process P1 should be considered as the head process (not parent).
• For the sake of the scenario, we assume the head is such a critical process that, if it
fails, all other processes should be terminated, P1 be re-created, and then all remaining
processes P2, P3, ..., and PN should be created following P1.
3. Operations:
• Each process is forked from the watchdog process (which is described in the following
sections).
• Each process should display a message (as explained below) indicating that it has
started and then go to sleep until it receives a signal.
• Each process must handle the signals given in Table 1.
• Processes handle signals by only printing the value of the corresponding signal.
• Speciﬁc to SIGTERM signal, the process should terminate after printing the required
message.
• The instructions ﬁle has the signals and wait commands listed line by line. A sample
instruction ﬁle is shown in Table 2. For instance, if the instruction is ”SIGHUP P3”,
P3 receives a SIGHUP signal. If the instruction is ”wait 1”, all the processes should
sleep for 1 second.2.3 watchdog.c/watchdog.cpp
1. File properties:
• The name of the ﬁle must be watchdog.c/watchdog.cpp.
• Main method should be in the watchdog ﬁle.
2. Operations:
• The watchdog program opens the named pipe.
• The watchdog process writes its ID to the named pipe as P0 <PID>.
• The watchdog should be the parent process, so each process should be created using
fork() from the watchdog.
• The watchdog process writes the ID of each newly forked processes to the named pipe
as P# <PID>.
• The watchdog process should initiate each process from the process executable using
exec().
• If num of process = 10, your program should create 10 distinct processes (in addition
to the watchdog)..
• The number of processes in the program should be preserved, and be equal to the
num of process all the time.
• Watchdog sleeps until a process terminates.
• If a process terminates, watchdog should detect and restart it. Do not forget to send
”P# <PID>” tuple for the restarted process since the PID value has changed.
• If the head process receives SIGTERM signal, all of the other processes should also
be killed and restarted by the watchdog. The processes should be terminated in
increasing order of the process numbers and this order should be visible in the output.
• Watchdog kills processes via the SIGTERM signal.
• The whole project should be terminated in a safe state. That is, executor kills all the
child processes and the watchdog when it reaches the end of instructions.txt ﬁle.

 Input & Output
Your code must read the number of processes, the path of the input and output ﬁles from the
command line. We will run your code as follows (Make sure that your ﬁnal submission
compiles and runs with these commands):
// Run the executor.cpp in background
./executor 5 instruction_path &
// For C++:
g++ process.cpp -std=c++14 -o process
g++ watchdog.cpp -std=c++14 -o watchdog
./watchdog 5 process_output watchdog_output

• Main method should be in the watchdog ﬁle and and should expect 3 command line argu-
ments:
1. num of process: an integer value to specify the number of processes in the system
2. process output path: absolute path of the process program’s output ﬁle
3. watchdog output path: absolute path of the watchdog program’s output ﬁle
• The executor program takes 2 command line arguments:
1. num of process: an integer value to specify the number of processes in the system
2. instruction path: absolute path of the instructions ﬁle

 Output
• All operations performed by the process ﬁle should be logged to the ﬁle speciﬁed by the
command line argument process output path.
– When a process is ready to execute, it prints: P<ID> is waiting for a signal
– When a process receives a signal, it prints: P<ID> received signal <VALUE>
– When a process receives SIGTERM signal, it prints: P<ID> is received signal 15,
terminating gracefully
– Table 3 shows the expected outputs of process ﬁle for the sample instructions given in
Table 2 with num of process = 3.
• All operations performed by the watchdog should be logged to the ﬁle speciﬁed by the
command line argument watchdog output path.
– When watchdog creates a process, it prints: P<ID> is started and it has a pid of
<PID VALUE>.
– When the head process is killed, watchdog prints: P1 is killed, all processes must be
killed.
– When restarting all processes, watchdog prints: Restarting all processes.
– When a process (except P1) is killed, watchdog prints: P<ID> is killed. And when a
process is being restarted/recreated, watchdog prints: Restarting P<ID>.
– When the watchdog terminates normally (i.e., all the instructions are completed), it prints:
Watchdog is terminating gracefully
– Table 4 shows the expected outputs of the watchdog ﬁle for the sample instructions given
in Table 2 with num of process = 3.
