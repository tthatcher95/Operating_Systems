#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>


//Declaration of Functions
struct config_values get_config(char* configFile);
struct meta_operation *get_meta(char* metaFile);
struct PCB *loopThrough(struct meta_operation *metaData, struct config_values configFile);
struct PCB *createPCB(struct meta_operation *metaData);

void loopThroughProcessList(struct meta_operation *metaData, struct PCB *firstProccess, struct config_values configFile);
int print_meta_values(struct meta_operation *current);
double waitFor(double Milliseconds, double total);

enum NODES { FIRST };

//Stores all values from config file
struct config_values
{
  int version;
  char *file;
  char *cpu_code;
  int quantum_time;
  int mem_avail;
  int process_cycle;
  int IO_cycle;
  char *log_to;
  char *log_file_path;
};

//Meta file
struct meta_operation
{
  char letter;
  char *operation;
  int num;
  int num_nodes;
  int cycle_time;
  struct meta_operation *next_meta;
};

//PCB Struct which includes Meta-Data LinkedList
struct PCB
{
  struct meta_operation *Process;
  struct PCB *next_Process;
  enum {NEW, READY, BLOCKED, EXIT, RUNNING} state;
  int numOfProcess;
};

#endif
