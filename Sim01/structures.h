#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct config_values get_config(FILE* config);
struct meta_operation *get_meta(FILE* filepath);

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
