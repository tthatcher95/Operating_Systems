#include "structures.h"
#include "SimpleTimer.h"
#include <pthread.h>


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

void *getThreads(void *arguments)
  {
    struct threadARGS *args = (struct threadARGS *) arguments;
    double totalTime = args->totalTime;
    double currentOpTime;
    char *tempBuffer = args->tempBuffer;
    char *fileBuffer = args->fileBuffer;
    struct PCB *firstProccess = args->firstProccess;
    struct config_values configFile = args->configFile;

    //Decides between different letters
    switch(firstProccess->Process->letter)
    {
      case ('I'):
        //If current operation is 'P' executes it, then prints the time it took to execute
        sprintf(tempBuffer,"Time: %f, Process %d %s input start\n", totalTime, firstProccess->numOfProcess, CURRENT_PROCESS_OPERATION);
        fileBuffer = buildBuffer(configFile, tempBuffer, fileBuffer);

        //Waits for # of I/O Cycles * Cycle Time then addes to overall time
        currentOpTime = waitFor(NUMOF_IO_CYCLES_TIMES_CYCLE_TIME, totalTime);
        firstProccess->processTime += currentOpTime*1000;

        //Updates total time in struct value and actual time
        totalTime = currentOpTime;
        args->totalTime = totalTime;

        //Prints string into tempBuffer char array
        sprintf(tempBuffer,"Time: %f, Process %d %s input end\n", totalTime, firstProccess->numOfProcess, CURRENT_PROCESS_OPERATION);

        //Calls a function to continuously build buffer array
        buildBuffer(configFile, tempBuffer, fileBuffer);
        break;
      case ('O'):
        //If current operation is 'P' executes it, then prints the time it took to execute
        sprintf(tempBuffer,"Time: %f, Process %d %s output start\n", totalTime, firstProccess->numOfProcess, CURRENT_PROCESS_OPERATION);
        buildBuffer(configFile, tempBuffer, fileBuffer);

        //Waits for # of I/O Cycles * Cycle Time then addes to overall time
        currentOpTime = waitFor(NUMOF_IO_CYCLES_TIMES_CYCLE_TIME, totalTime);
        firstProccess->processTime += currentOpTime*1000;

        //Updates total time in struct value and actual time
        totalTime = currentOpTime;
        args->totalTime = totalTime;

        //Prints string into tempBuffer char array
        sprintf(tempBuffer,"Time: %f, Process %d %s output end\n", totalTime, firstProccess->numOfProcess, CURRENT_PROCESS_OPERATION);

        //Calls a function to continuously build buffer array
        buildBuffer(configFile, tempBuffer, fileBuffer);
        break;

    }
    return arguments;
  }

  //Functions to swap two processes values
  void swap(struct PCB *lessThan, struct PCB *greaterThan)
  {

      //Assigns values to temporary values
      int numProcess = lessThan->numOfProcess;
      double tempTime = lessThan->processTime;
      struct meta_operation *tempProcess = lessThan->Process;

      //Assigns value from 'lessThan' to 'greaterThan' values
      lessThan->numOfProcess = greaterThan->numOfProcess;
      lessThan->processTime = greaterThan->processTime;
      lessThan->Process = greaterThan->Process;

      //Assigns 'greaterThan' value to temporary values
      greaterThan->numOfProcess = numProcess;
      greaterThan->processTime = tempTime;
      greaterThan->Process = tempProcess;
  }

  //Sorts the linked list of Processes based on processTime
  //Using bubbleSort
  struct PCB *SJF(struct PCB *firstProcess)
  {
    struct PCB *currentProcess = NULL;
    int swapped;
    do {
        swapped = 0;
        currentProcess = firstProcess;

        //While next process is valid check if next is less than currentProcess
        //Swap if next is, otherwise move to the next process
        while (currentProcess->next_Process != NULL)
        {
          if(currentProcess->processTime > currentProcess->next_Process->processTime)
          {
            //If less than swap values and assign swapped to '1'
            swap(currentProcess->next_Process, currentProcess);
            swapped = 1;
          }
          currentProcess = currentProcess->next_Process;
        }
       }

    //Will run all code above until swapped == 0 or false
    while (swapped);

    //Returns the next iteration as the first will be garbage
    return firstProcess->next_Process;
  }


  int getList(struct PCB *firstProccess, struct config_values configFile, int amountOfProcess)
  {

    //Assigns a new pointer to start of LinkedList
    struct PCB *start = firstProccess;
    struct meta_operation *first = firstProccess->Process;

    //Iterates through until next Node is NULL
    while (firstProccess->next_Process != NULL)
    {
      amountOfProcess++;
      first = firstProccess->Process;
      firstProccess->Process = firstProccess->Process->next_meta;

      //If letter does not equal 'A' and operation does not equal "start" OR
      //If letter does not equal 'S' and operation does not equal "end" keep looping
      while((firstProccess->Process->letter != 'A' && strcmp(firstProccess->Process->operation, "start") != 0) ||
            (firstProccess->Process->letter != 'S' && strcmp(firstProccess->Process->operation, "end") != 0))
      {
        //Uses variable NUMOF_PROCESS_CYCLES_TIMES_CYCLE_TIME if letter equals 'P'
        //To determine the processTime
        if(firstProccess->Process->letter == 'P')
        {
          firstProccess->processTime += NUMOF_PROCESS_CYCLES_TIMES_CYCLE_TIME*1000;
        }

        //Uses variable NUMOF_IO_CYCLES_TIMES_CYCLE_TIME if letter equals 'I' or 'O'
        //To determine the processTime
        else if(firstProccess->Process->letter == 'I' || firstProccess->Process->letter == 'O')
        {
          firstProccess->processTime += NUMOF_IO_CYCLES_TIMES_CYCLE_TIME*1000;
        }
        firstProccess->Process = firstProccess->Process->next_meta;
      }

      //Reassign Process to the start
      firstProccess->Process = first;

      //Iterate to next node in linkedList
      firstProccess = firstProccess->next_Process;
    }

    //Sets pointer back to start
    firstProccess = start;

    //Returns total amount of Processes
    return amountOfProcess;
  }

  //Function to constatntly add and build buffer used to write to a file if configFile specifies
  char *buildBuffer(struct config_values configFile, char tempBuffer[10000], char fileBuffer[100000])
  {

    //If configFile says log to file, print the contents to the buffer
    if(strcmp(configFile.log_to, "File") == 0)
    {
      sprintf(fileBuffer + strlen(fileBuffer), "%s", tempBuffer);
    }

    //If value equals Both or Monitor output to file and add contents to fileBuffer
    else
    {
      sprintf(fileBuffer + strlen(fileBuffer), "%s", tempBuffer);
      printf("%s", tempBuffer);
    }

    return fileBuffer;
  }

  void writeToFile(char fileBuffer[], struct config_values configFile)
  {
    int index = 0;
    FILE* metaFile = fopen(configFile.log_file_path, "w");

    //Loops through each char in the char array fileBuffer and write to the
    //File specified in the configFile

    while (index < strlen(fileBuffer))
    {
      fprintf(metaFile, "%c", fileBuffer[index]);
      index++;
    }

    //Close file after work
    fclose(metaFile);

  }

//Function that makes the Simulation look nice
//And does the core work as far as: Assigning Wait Time, Accounting for each letter/operation.
//Basically combines everything to form the simulator02

int parseThroughProcesses(struct meta_operation *metaData, struct PCB *firstProccess, struct config_values configFile)
{

  pthread_t ioThread;
  struct threadARGS args;
  char tempBuffer[255];
  char fileBuffer[100000];
  double totalTime = 0.0;
  double currentOpTime = 0.0;
  int amountOfProcess = 0;

  amountOfProcess = getList(firstProccess, configFile, amountOfProcess);
  args.firstProccess = firstProccess;
  args.configFile = configFile;
  args.tempBuffer = tempBuffer;
  args.fileBuffer = fileBuffer;


  //If SJF-N is CPU_CODE sort by run time, then loop through
  if (strcmp(configFile.cpu_code, "SJF-N") == 0)
  {
      firstProccess = SJF(firstProccess);
  }

  //Prints out initialization & adds to buffer using function 'buildBuffer'
  sprintf(tempBuffer, "Operating System Simulator\n");
  buildBuffer(configFile, tempBuffer, fileBuffer);

  //Adds ===== to buffer
  sprintf(tempBuffer, "==========================\n\n");
  buildBuffer(configFile, tempBuffer, fileBuffer);

  //Addes string to buffer
  sprintf(tempBuffer, "Loading ConfigData\n");
  buildBuffer(configFile, tempBuffer, fileBuffer);

  //Adds string to buffer
  sprintf(tempBuffer, "Loading MetaData\n");
  buildBuffer(configFile, tempBuffer, fileBuffer);

  //Adds strings to buffer
  sprintf(tempBuffer, "==========================\n\n");
  buildBuffer(configFile, tempBuffer, fileBuffer);

  //Adds strings to buffer
  sprintf(tempBuffer, "Begin Simulation \n");
  buildBuffer(configFile, tempBuffer, fileBuffer);

  //Starts time at 0.0
  sprintf(tempBuffer, "Time: %f, OS: System start\n", totalTime);
  buildBuffer(configFile, tempBuffer, fileBuffer);

  //Calls function 'setPCB()' which sets main process to state 'RUNNING'
  //Then returns a value and calls 'waitFor()' which will wait for the time specified by 'setPCB()'
  totalTime = waitFor(setPCB(firstProccess, metaData), totalTime);

  //Updates thread struct with totalTime value
  args.totalTime = totalTime;

  //Adds string with time into the buffer
  sprintf(tempBuffer,"Time: %f, OS: Begin PCB Creation\n", totalTime);
  buildBuffer(configFile, tempBuffer, fileBuffer);

  //Calls function 'setNew()' which sets main process to state 'RUNNING'
  //Then returns a value and calls 'waitFor()' which will wait for the time specified by 'setnew()'
  totalTime = waitFor(setNew(firstProccess), totalTime);

  //Updates thread struct with totalTime value
  args.totalTime = totalTime;

  //Adds string with time into the buffer
  sprintf(tempBuffer,"Time: %f, OS: Initialized all processes states to NEW\n", totalTime);
  buildBuffer(configFile, tempBuffer, fileBuffer);

  //Calls function 'setReady()' which sets main process to state 'RUNNING'
  //Then returns a value and calls 'waitFor()' which will wait for the time specified by 'setReady()'
  totalTime = waitFor(setReady(firstProccess), totalTime);

  //Updates thread struct with totalTime value
  args.totalTime = totalTime;

  //Adds string with time into the buffer
  sprintf(tempBuffer,"Time: %f, OS: Initialized all processes states to READY\n", totalTime);
  buildBuffer(configFile, tempBuffer, fileBuffer);

  //Iterates through all Process's in list defined in 'createPCB()' function
  while(firstProccess != NULL)
  {

         //Calls function 'setRunning()' which sets main process to state 'RUNNING'
         //Then returns a value and calls 'waitFor()' which will wait for the time specified by 'setRunning()'
         totalTime = waitFor(setRunning(firstProccess), totalTime);

         //Updates thread struct with totalTime value
         args.totalTime = totalTime;

         //Adds string to bufferArray
         sprintf(tempBuffer,"Time: %f, OS: Process: %d set state set to RUNNING\n", totalTime, firstProccess->numOfProcess);
         buildBuffer(configFile, tempBuffer, fileBuffer);

         //While the letter of the current PCB does not equal 'S' and the operation does not equal "end"
         //Iterates through my PCB LinkedList and prints out values needed
         if (strcmp(configFile.cpu_code, "SJF-N") == 0)
         {
           //If SJF-N is in effect print the string with info into the buffer
           sprintf(tempBuffer,"SJF-N: selects Process %d with time: %d(ms)\n", firstProccess->numOfProcess, (int) firstProccess->processTime);
           buildBuffer(configFile, tempBuffer, fileBuffer);
         }

         while ((strcmp(firstProccess->Process->operation, "end") != 0) && amountOfProcess > 0)
         {


           //Checks which letter is in the currentProcess
           switch (firstProccess->Process->letter)
           {
             case ('P'):
                  //If current operation is 'P' executes it, then prints the time it took to execute and adds to fileBuffer
                  sprintf(tempBuffer,"Time: %f, Process %d %s operation start\n", totalTime, firstProccess->numOfProcess, CURRENT_PROCESS_OPERATION);
                  buildBuffer(configFile, tempBuffer, fileBuffer);

                  //Waits for # of Process Cycles * Cycle Time then addes to overall time
                  currentOpTime = waitFor(NUMOF_PROCESS_CYCLES_TIMES_CYCLE_TIME, totalTime);

                  //Updates totalTime with time just waited
                  totalTime = currentOpTime;

                  //Adds time into processTime struct for this process
                  firstProccess->processTime += currentOpTime*1000;

                  //Adds striing and info into the buffer
                  sprintf(tempBuffer,"Time: %f, Process %d %s operation end\n", totalTime, firstProccess->numOfProcess, CURRENT_PROCESS_OPERATION);
                  buildBuffer(configFile, tempBuffer, fileBuffer);

                  //Updates thread struct with totalTime
                  args.totalTime = totalTime;
                  break;

             case ('I'):
                  //Create thread and run 'waitFor()' functions
                  pthread_create(&ioThread, NULL, getThreads, (void *)&args);

                  //Waits for thread to execute and return
                  pthread_join(ioThread, NULL);

                  //Assigns totalTime to value from thread
                  totalTime = args.totalTime;
                  break;

             case ('O'):
                  //Create thread and run 'waitFor()' functions
                  pthread_create(&ioThread, NULL, getThreads, (void *)&args);

                  //Waits for thread to execute and return
                  pthread_join(ioThread, NULL);

                  //Assigns totalTime to value from thread
                  totalTime = args.totalTime;
                  break;

             case ('M'):
                  currentOpTime = waitFor(.5, totalTime);
                  totalTime = currentOpTime;

                  //Updates thread struct with totalTime
                  args.totalTime = totalTime;

                  //Adds time into processTime struct for this process
                  firstProccess->processTime += currentOpTime*1000;

                  //Stores string and information into the fileBuffer
                  sprintf(tempBuffer,"Time: %f, Process %d Memory management %s action\n", totalTime, firstProccess->numOfProcess, CURRENT_PROCESS_OPERATION);
                  buildBuffer(configFile, tempBuffer, fileBuffer);
                  break;

           }

           //Goes to the next node and processes it's information
           firstProccess->Process = firstProccess->Process->next_meta;

           //Increments process by 1 in struct
           args.firstProccess = firstProccess;

         }

         //Sets current Process to EXIT state then returns the time it took to execute
         totalTime = waitFor(setExit(*firstProccess), totalTime);

         //Updates thread struct with totalTime
         args.totalTime = totalTime;

         //Adds string into bufferArray
         sprintf(tempBuffer,"Time: %f, OS: Process %d set in EXIT state\n", totalTime, firstProccess->numOfProcess);
         buildBuffer(configFile, tempBuffer, fileBuffer);

         //Moves current Process to next Process
         firstProccess = firstProccess->next_Process;
         amountOfProcess--;

         //If letter of operation equals 'S' and operation equals 'end'
         //Program Ends and prints out final time
         if(amountOfProcess == 0)
         {
           //Adds string into bufferArray
           sprintf(tempBuffer,"Time: %f, System stop\n", totalTime);
           buildBuffer(configFile, tempBuffer, fileBuffer);

           //Adds string into bufferArray
           sprintf(tempBuffer,"End Simulation\n");
           buildBuffer(configFile, tempBuffer, fileBuffer);

           //Adds string into bufferArray
           sprintf(tempBuffer,"==========================\n");
           buildBuffer(configFile, tempBuffer, fileBuffer);

           //If config value calls for it call writeToFile to write fileBuffer
           //To File specified by the configuration file
           if(strcmp(configFile.log_to, "Both") == 0 || strcmp(configFile.log_to, "File") == 0)
           {
             FILE *logFile = fopen(configFile.log_file_path, "rw");
             writeToFile(fileBuffer, configFile);
             fclose(logFile);
           }
           //Return ) if all Process have executed
           return 0;
         }
       }
       //Returns 1 if an error pops up
       return 1;
     }
