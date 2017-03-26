#include "structures.h"
#include "SimpleTimer.h"


//Macros to enhance Readability
#define CURRENT_PROCESS_OPERATION firstProccess->Process->operation
#define NUMOF_IO_CYCLES_TIMES_CYCLE_TIME (float) configFile.IO_cycle * (float) firstProccess->Process->cycle_time/1000
#define NUMOF_PROCESS_CYCLES_TIMES_CYCLE_TIME (float)configFile.process_cycle * (float) firstProccess->Process->cycle_time/1000


//Timer using Professor Leverington's Code
//Takes two values addes 'Milliseconds' to the first of the values
//Iteratively calls accessTimer() until the first value is reached

double waitFor(double Milliseconds, double total)
{
  double current;
  double base;

  char *tmr = malloc(sizeof(char*));

  base = accessTimer(START_TIMER, tmr);
  current = accessTimer(STOP_TIMER, tmr);

  while(current <= base + Milliseconds)
  {
    current = accessTimer(START_TIMER, tmr);
  }

  //returns difference between both values
  total += current - base;
  return total;
}

//Function that walks through my linkedList of PCB's
//And sets them all to 'NEW' state and takes the time
//It takes to set all nodes

double setNew(struct PCB *pcbList)
{

  //char pointer for accessTimer()
  char *tmr = malloc(sizeof(char*));

  //Assigns a new pointer to start of LinkedList
  struct PCB *start = pcbList;

  accessTimer(START_TIMER, tmr);

  //Iterates through until next Node is NULL
  while (pcbList->next_Process != NULL)
  {
    pcbList->state = NEW;
    pcbList = pcbList->next_Process;

  }
  accessTimer(STOP_TIMER, tmr);

  //Sets pointer back to start
  pcbList = start;
  return accessTimer(GET_TIME_DIFF, tmr);

}

//Function that walks through my linkedList of PCB's
//And sets them all to 'READY' state and takes the time
//It takes to set all nodes

double setReady(struct PCB *pcbList)
{

  char *tmr = malloc(sizeof(char*));

  //Assigns a 2nd pointer to start of LinkedList
  struct PCB *start = pcbList;

  accessTimer(START_TIMER, tmr);

  //Iterates till next node is NULL
  while (pcbList->next_Process != NULL)
  {
    pcbList->state = READY;
    pcbList = pcbList->next_Process;
  }
  accessTimer(STOP_TIMER, tmr);

  //Resets pointer back to beginning
  pcbList = start;
  return accessTimer(GET_TIME_DIFF, tmr);

}

//Function that sets one node of my PCB LinkedList
//And sets one to 'RUNNING' state and takes the time
//To perform that

double setRunning(struct PCB *pcbList)
{

  char *tmr = malloc(sizeof(char*));

  accessTimer(START_TIMER, tmr);
  pcbList->state = READY;
  accessTimer(STOP_TIMER, tmr);
  return accessTimer(GET_TIME_DIFF, tmr);

}

////Function that sets one node of my PCB LinkedList
//And sets one to 'EXIT' state and takes the time
//To perform that

double setExit(struct PCB pcbList)
{

  char *tmr = malloc(sizeof(char*));

  accessTimer(START_TIMER, tmr);
  pcbList.state = EXIT;
  accessTimer(STOP_TIMER, tmr);
  return accessTimer(GET_TIME_DIFF, tmr);

}

//Creates and Returns a PCB LinkedList using Pointers

struct PCB *createPCB(struct meta_operation *metaData)
{
  struct PCB *firstProccess = malloc(sizeof(struct PCB));
  struct PCB *currentProcess = malloc(sizeof(struct PCB));

  //Keeps track of how many 'Proccesses' are in the current MetaData File
  int processTracker = FIRST;

  //Iterates through MetaData LinkedList until next node is NULL
  while (metaData->next_meta != NULL)
  {

    //Assigns the next process to be NULL (overrides down the road if there is a process there)
    currentProcess->next_Process = NULL;

    //Checks for the start of new Processes
    if (metaData->letter == 'A' && strcmp(metaData->operation, "start") == 0)
    {

      //Points '*Process' at MetaData pointer containing a starting point
      currentProcess->Process = metaData;

      //Assigns Process number
      currentProcess->numOfProcess = processTracker;

      //Records first process position
      if (processTracker == FIRST)
      {
        firstProccess = currentProcess;

      }

      //Allocates memory for next Node
      currentProcess->next_Process = malloc(sizeof(struct PCB));

      //Reassigns current to Next Node
      currentProcess = currentProcess->next_Process;
      processTracker++;

    }

    metaData= metaData->next_meta;
  }

  //Return location of first Process
  return firstProccess;
}

//Function to record how long it takes to make the PCB linkedlist
//Using Previous function 'createPCB()' and Professor Leverington's timer

double setPCB(struct PCB *pcbList, struct meta_operation *metaData)
{
  char *tmr = malloc(sizeof(char*));

  accessTimer(START_TIMER, tmr);
  pcbList = createPCB(metaData);
  accessTimer(STOP_TIMER, tmr);
  return accessTimer(GET_TIME_DIFF, tmr);
}

//Function to record how long it takes to allocate
//specified memory
double allocateMemory(int memory)
{
  char *tmr = malloc(sizeof(char*));

  accessTimer(START_TIMER, tmr);
  malloc(memory);
  accessTimer(STOP_TIMER, tmr);
  return accessTimer(GET_TIME_DIFF, tmr);
}

//Function that makes the Simulation look nice
//And does the core work as far as: Assigning Wait Time, Accounting for each letter/operation.
//Basically combines everything to form the simulator02

void loopThroughProcessList(struct meta_operation *metaData, struct PCB *firstProccess, struct config_values configFile)
{
  double totalTime = 0.0;

  //Prints out initialization
  printf("Operating System Simulator\n");
  printf("==========================\n\n");
  printf("Loading ConfigData\n");
  printf("Loading MetaData\n");
  printf("==========================\n\n");
  printf("Begin Simulation \n");

  //Iterates through all Process's in list defined in 'createPCB()' function
  while(firstProccess->next_Process != NULL)
  {

         //Starts time at 0.0, but also executes how long it takes to start at zero
         waitFor(NUMOF_PROCESS_CYCLES_TIMES_CYCLE_TIME, totalTime);
         printf("Time: %f, OS: System start\n", totalTime);

         //Calls function 'setPCB()' which sets main process to state 'RUNNING'
         //Then returns a value and calls 'waitFor()' which will wait for the time specified by 'setPCB()'
         totalTime = waitFor(setPCB(firstProccess, metaData), totalTime);
         printf("Time: %f, OS: Begin PCB Creation\n", totalTime);

         //Calls function 'setNew()' which sets main process to state 'RUNNING'
         //Then returns a value and calls 'waitFor()' which will wait for the time specified by 'setnew()'
         totalTime = waitFor(setNew(firstProccess), totalTime);
         printf("Time: %f, OS: Initialized all processes states to NEW\n", totalTime);

         //Calls function 'setReady()' which sets main process to state 'RUNNING'
         //Then returns a value and calls 'waitFor()' which will wait for the time specified by 'setReady()'
         totalTime = waitFor(setReady(firstProccess), totalTime);
         printf("Time: %f, OS: Initialized all processes states to READY\n", totalTime);

         //Calls function 'setRunning()' which sets main process to state 'RUNNING'
         //Then returns a value and calls 'waitFor()' which will wait for the time specified by 'setRunning()'
         totalTime = waitFor(setRunning(firstProccess), totalTime);
         printf("Time: %f, OS: Process: %d set state set to RUNNING\n", totalTime, firstProccess->numOfProcess);

         //While the letter of the current PCB does not equal 'S' and the operation does not equal "end"
         //Iterates through my PCB LinkedList and prints out values needed
         while (firstProccess->Process->letter != 'S' && strcmp(firstProccess->Process->operation, "end") != 0)

         {

           //Checks which letter is in the currentProcess
           switch (firstProccess->Process->letter)
           {
             case ('P'):
                  //If current operation is 'P' executes it, then prints the time it took to execute
                  printf("Time: %f, Process %d %s operation start \n", totalTime, firstProccess->numOfProcess, CURRENT_PROCESS_OPERATION);

                  //Waits for # of Process Cycles * Cycle Time then addes to overall time
                  totalTime = waitFor(NUMOF_PROCESS_CYCLES_TIMES_CYCLE_TIME, totalTime);
                  printf("Time: %f, Process %d %s operation end \n", totalTime, firstProccess->numOfProcess, CURRENT_PROCESS_OPERATION);
                  break;

             case ('I'):
                  //If current operation is 'P' executes it, then prints the time it took to execute
                  printf("Time: %f, Process %d %s input start \n", totalTime, firstProccess->numOfProcess, CURRENT_PROCESS_OPERATION);

                  //Waits for # of I/O Cycles * Cycle Time then addes to overall time
                  totalTime = waitFor(NUMOF_IO_CYCLES_TIMES_CYCLE_TIME, totalTime);
                  printf("Time: %f, Process %d %s input end \n", totalTime, firstProccess->numOfProcess, CURRENT_PROCESS_OPERATION);
                 break;

             case ('O'):
                  //If current operation is 'P' executes it, then prints the time it took to execute
                  printf("Time: %f, Process %d %s output start \n", totalTime, firstProccess->numOfProcess, CURRENT_PROCESS_OPERATION);

                  //Waits for # of I/O Cycles * Cycle Time then addes to overall time
                  totalTime = waitFor(NUMOF_IO_CYCLES_TIMES_CYCLE_TIME, totalTime);
                  printf("Time: %f, Process %d %s output end OS: \n", totalTime, firstProccess->numOfProcess, CURRENT_PROCESS_OPERATION);
                  break;

             case ('M'):
                  totalTime = waitFor(0.5, totalTime);
                  printf("Time: %f, Process %d Memory management %s action \n", totalTime, firstProccess->numOfProcess, CURRENT_PROCESS_OPERATION);
                  break;

           }

           //Goes to the next node and processes it's information
           firstProccess->Process = firstProccess->Process->next_meta;
         }

         //Sets current Process to EXIT state then returns the time it took to execute
         totalTime = waitFor(setExit(*firstProccess), totalTime);
         printf("Time: %f, OS: Process %d set in EXIT state\n", totalTime, firstProccess->numOfProcess);

         //If letter of operation equals 'S' and operation equals 'end'
         //Program Ends and prints out final time
         if(firstProccess->Process->next_meta->letter == 'S' && strcmp(firstProccess->Process->next_meta->operation, "end") == 0)
         {
           waitFor(NUMOF_PROCESS_CYCLES_TIMES_CYCLE_TIME, totalTime);
           printf("Time: %f, System stop\n\n", totalTime);
           printf("End Simulation\n");
           printf("==========================\n\n");

         }

         //Moves current Process to next Process
         firstProccess = firstProccess->next_Process;
       }
     }
