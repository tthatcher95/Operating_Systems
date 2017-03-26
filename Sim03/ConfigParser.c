#include "structures.h"
#define QUANTUM_TIME_MAX 1000
#define MEM_AVAIL_MAX 1048576
#define IO_CYCLE_MAX 10000
#define PROCESS_CYCLE_MAX 1000
#define MAXSPACE 1024
#define IS_VALID 0

//AScii value for colon ':'
const int COLON_OFFSET = -58;


//Creates Constants each value needed
enum {VERSION, FILE_NAME, CPU_CODE, QUANTUM_TIME, MEM_AVAIL,
      PROCESS_CYCLE, IO_cycle, LOG_TO, LOG_FILE_PATH};

int if_valid(char leftside[MAXSPACE], int iter)
  {
      switch (iter)
      {
        case VERSION:
          if (strcmp(leftside, "Version/Phase:") == COLON_OFFSET)
          {
            return IS_VALID;
          }
          else
          {
            printf("%i\n", strcmp(leftside, "Version/Phase:") == COLON_OFFSET);
            fprintf(stderr, "Expected 'Version/Phase:' ");
            fprintf(stderr, "Got: %s\n", leftside);
            exit(1);
          }
        case FILE_NAME:
          if (strcmp(leftside, "File Path:") == COLON_OFFSET)
          {
            return IS_VALID;
          }
          else
          {
            fprintf(stderr, "Expected 'File Path:' ");
            fprintf(stderr, "Got: %s\n", leftside);
            exit(1);
          }

        case CPU_CODE:
          if (strcmp(leftside, "CPU Scheduling Code:") == COLON_OFFSET)
          {
             return IS_VALID;
          }
          else
          {
            fprintf(stderr, "Expected 'CPU Scheduling Code:' ");
            fprintf(stderr, "Got: %s\n", leftside);
            exit(1);
          }
        case QUANTUM_TIME:
          if (strcmp(leftside, "Quantum Time (cycles):") == COLON_OFFSET)
          {
             return IS_VALID;
          }
          else
          {
            fprintf(stderr, "Expected 'Quantum Time (cycles):' ");
            fprintf(stderr, "Got: %s\n", leftside);
            exit(1);
          }
        case MEM_AVAIL:
          if (strcmp(leftside, "Memory Available (KB):") == COLON_OFFSET)
          {
             return IS_VALID;
          }
          else
          {
            fprintf(stderr, "Expected 'Memory Available (KB):' ");
            fprintf(stderr, "Got: %s\n", leftside);
            exit(1);
          }
        case PROCESS_CYCLE:
          if (strcmp(leftside, "Processor Cycle Time (msec):") == COLON_OFFSET)
          {
            return IS_VALID;
          }
          else
          {
            fprintf(stderr, "Expected 'Processor Cycle Time (msec):' ");
            fprintf(stderr, "Got: %s\n", leftside);
            exit(1);
          }
        case IO_cycle:
          if (strcmp(leftside, "I/O Cycle Time (msec):") == COLON_OFFSET)
          {
             return IS_VALID;
          }
        else
          {
            fprintf(stderr, "Expected 'I/O Cycle Time (msec):' ");
            fprintf(stderr, "Got: %s\n", leftside);
            exit(1);
          }
        case LOG_TO:
          if (strcmp(leftside, "Log To:") == COLON_OFFSET)
          {
            return IS_VALID;
          }
          else
          {
            fprintf(stderr, "Expected 'Log To:' ");
            fprintf(stderr, "Got: %s\n", leftside);
            exit(1);
          }
        case LOG_FILE_PATH:
          if (strcmp(leftside, "Log File Path:") == COLON_OFFSET)
          {
            return IS_VALID;
          }
          else
          {
            fprintf(stderr, "Expected 'Log File Path:' ");
            fprintf(stderr, "Got: %s\n", leftside);
            exit(1);
          }

         }
         return IS_VALID;
  }

  //Implmentaion of strdup in a standard library
  char *strdupp(char *src)
  {
    char *str;
    size_t len = strlen(src);

    str = malloc(len + 1);
    if (str)
      {
        memcpy(str, src, len + 1);
      }
    return str;
  }


int print_config_values(struct config_values values)
  {
    printf("Version/Phase: %d\n", values.version);
    printf("File Path: %s\n", values.file);
    printf("CPU Scheduling Code: %s\n", values.cpu_code);
    printf("Quantum Time (cycles): %d\n", values.quantum_time);
    printf("Memory Available (KB): %d\n", values.mem_avail);
    printf("Processor Cycle Time (msec): %d\n", values.process_cycle);
    printf("I/O Cycle Time: %d\n", values.IO_cycle);
    printf("Log To: %s\n", values.log_to);
    printf("Log File Path: %s\n\n", values.log_file_path);
    return IS_VALID;
  }

  //Check if file is valid

  void valid_file(char* configFile)
  {
    FILE* config = fopen(configFile, "r");
    if(config == NULL)
    {
      fprintf(stderr, "Config File: '%s' entered is not valid\n", configFile);
      exit(1);
    }
  }

struct config_values get_config(char* configFile)
  {

  int line_num = 0;
  int str_to_int = 0;
  int iteration = 0;
  char line[MAXSPACE];
  char *token;
  char left[MAXSPACE];
  char temp[MAXSPACE];
  struct config_values config_vals;
  FILE* config = fopen(configFile, "r");

// Loops through every line and stores values in
// 'config_values' struct
valid_file(configFile);

  while(1)
    {
      fgets(line, MAXSPACE, config);
      // If first, skip --- blank, skip --- last, end.
      if (line_num == 0)
      {
        if((strcmp(line, "Start Simulator Configuration File\n") != 0))
        {
          fprintf(stderr, "Error ~~ line %d must be in format: ", line_num);
          fprintf(stderr, "'Start Simulator Configuration File'\n");
          exit(1);
        }
      }
      if((strcmp(line, "Start Simulator Configuration File\n") == 0))
        {
          line_num++;
          continue;
        }
      else if((strcmp(line, "\n") == 0))
        {
          line_num++;
          continue;
        }
      else if((strcmp(line, "End Simulator Configuration File.\n") == 0))
        {
          line_num++;
          break;
        }

        line_num++;
        // Breaks string up until ':'
        // Then skips space if any
      token = strtok(line, ":");
      sprintf(left, "%s" , token);

      token = strtok(NULL, " ");
      token = strtok(token, "\n");
      sprintf(temp, "%s" , token);



      // Stores values and prints errors if
      // an issue occurs
      // Version --- Converts to int and stores
      // File    --- Stores string values
      // CPUCode --- Checks if code is right, then stores
      // Quantum --- Converts to int and stores
      // Memory  --- Converts to int and stores
      // Process --- Converts to int and stores
      // IO      --- Converts to int and stores
      // Log     --- Checks if value is right,
      //             stores string in struct
      // LogFile --- Stores string in struct
      switch (iteration)
      {
        case VERSION:
          if(if_valid(left, iteration) == IS_VALID)
          {
            str_to_int = atoi((temp));
            config_vals.version = str_to_int;
            break;
          }

        case FILE_NAME:
          if(if_valid(left, iteration) == IS_VALID)
          {
            config_vals.file = strdupp(temp);
            break;
          }

        case CPU_CODE:
        //Chekcs if codes are valid
        if(if_valid(left, iteration) == IS_VALID)
          {
           if (strcmp(temp, "NONE") == 0 ||
               strcmp(temp, "FCFS-N") == 0 ||
               strcmp(temp, "SJF-N") == 0 ||
               strcmp(temp, "SRTF-P") == 0 ||
               strcmp(temp, "FCFS-P") == 0 ||
               strcmp(temp, "RR-P") == 0)
               {
                if (strcmp(temp, "NONE") == 0)
                {
                 config_vals.cpu_code = "FCFS-N";
                 break;
                }
                else
                {
                  config_vals.cpu_code = strdupp(temp);
                  break;
                }
               }
               else
               {
                 printf("Error ~~ CPU CODE: %s", temp);
                 exit(1);
               }
          }
        case QUANTUM_TIME:
           if(if_valid(left, iteration) == IS_VALID)
          {

           str_to_int = atoi((temp));

           //checks if value is within bounds 0 <= value <= 100
           if(0 <= str_to_int && str_to_int <= QUANTUM_TIME_MAX)
           {
               config_vals.quantum_time = str_to_int;
           }
           else
           {
             fprintf(stderr, "Error ~~ Quantum Time Out of Bounds (0 - 100) %d \n", str_to_int);
             exit(1);
           }

           break;
          }
        case MEM_AVAIL:
           if(if_valid(left, iteration) == IS_VALID)
          {
           str_to_int = atoi((temp));

           //checks if value is within bounds 0 <= value <= 1048576 (1 MB)
           if(0 <= str_to_int && str_to_int <= MEM_AVAIL_MAX)
           {
               config_vals.mem_avail = str_to_int;
           }
           else
           {
             fprintf(stderr, "Error ~~ Memory Available Out of Bounds (0 - 1,048,576) %d \n", str_to_int);
             exit(1);
           }

           break;
          }
        case PROCESS_CYCLE:
           if(if_valid(left, iteration) == IS_VALID)
          {
           str_to_int = atoi((temp));

           //checks if value is within bounds 0 <= value <= 1000
           if(0 <= str_to_int && str_to_int <= PROCESS_CYCLE_MAX)
           {
             config_vals.process_cycle = str_to_int;

           }
           else
           {
             fprintf(stderr, "Error ~~ Processor Cycle Time Out of Bounds (0 - 1000) %d \n", str_to_int);
             exit(1);
           }

           break;
          }
        case IO_cycle:
           if(if_valid(left, iteration) == IS_VALID)
          {
           str_to_int = atoi((temp));

           //checks if value is within bounds 0 <= value <= 10,000
           if(0 <= str_to_int && str_to_int <= IO_CYCLE_MAX)
           {
             config_vals.IO_cycle = str_to_int;
           }
           else
           {
             fprintf(stderr, "Error ~~ IO Cycle Time Out of Bounds (0 - 10,000) %d \n", str_to_int);
             exit(1);
           }
           break;
          }
        case LOG_TO:
           if(if_valid(left, iteration) == IS_VALID)
          {
            //Checks if log to is valid
             if(strcmp(temp, "Monitor") == 0 ||
                strcmp(temp, "File") == 0 ||
                strcmp(temp, "Both") == 0)
             {
               config_vals.log_to = strdupp(temp);
               break;
             }
             else
             {
               fprintf(stderr, "Error ~~ Log To: %s", temp);
               exit(1);
             }
          }
        case LOG_FILE_PATH:
           if(if_valid(left, iteration) == IS_VALID)
          {
             if(strcmp(config_vals.log_to, "Monitor") == 0)
             {
               config_vals.log_file_path = "NONE";
               break;
             }
             else
             {
               config_vals.log_file_path = strdupp(temp);

               break;
             }
          }
        }
       iteration++;

     }
    // Returns a struct containing all neccesary values
    fclose(config);

    return config_vals;

  }
