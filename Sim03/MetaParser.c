#include "structures.h"

const int MAX_LINESPACE = 1024;
const int VALID = 1;
const int NOT_VALID = 0;
const int MAX_OPERATION_SPACE = 255;

//prints out each meta values in correct format
int print_meta_values(struct meta_operation *current)
{
  printf("\nData Item Component Letter: %c", current->letter);
  printf("\nData Item Operation       : %s", current->operation);
  printf("\nData Item Cycle Time      : %d\n", current->num);
  return VALID;
}

//function to check if code is valid
int letter_valid(struct meta_operation *current)

{
  if (current->letter == 'S' ||
      current->letter == 'A' ||
      current->letter == 'P' ||
      current->letter == 'M' ||
      current->letter == 'I' ||
      current->letter == 'O')
  {
    return VALID;
  }
  else
  {
    fprintf(stderr, "\nError ~~ Letter Code: %c\n", current->letter);
    exit(1);
  }
}
  int operation_valid(struct meta_operation *current)
  {
  //checks if each operation is valid
  if (strcmp(current->operation, "access") == 0     ||
      strcmp(current->operation, "allocate") == 0   ||
      strcmp(current->operation, "end") == 0        ||
      strcmp(current->operation, "hard drive") == 0 ||
      strcmp(current->operation, "keyboard") == 0   ||
      strcmp(current->operation, "printer") == 0    ||
      strcmp(current->operation, "monitor") == 0    ||
      strcmp(current->operation, "run") == 0        ||
      strcmp(current->operation, "start") == 0)
  {
    return VALID;
  }

  else
  {
    fprintf(stderr, "\nError ~~ Operation Code: %s\n", current->operation);
    exit(1);
  }
}

void validFile(char* configFile)
{
  FILE* config = fopen(configFile, "r");
  if(config == NULL)
  {
    fprintf(stderr, "Meta File: '%s' in config file nvalid\n", configFile);
    exit(1);
  }
}

//function to store each 'operation'
struct meta_operation *get_meta(char* metaFile)
{

  char line[MAX_LINESPACE];
  FILE* filepath = fopen(metaFile, "r");

  //saves space for my struct pointer and each struct
  struct meta_operation *meta_vals = malloc(sizeof(struct meta_operation));
  struct meta_operation *current_node = malloc(sizeof(struct meta_operation));
  struct meta_operation *first = malloc(sizeof(struct meta_operation));


  validFile(metaFile);
  int index = 0;
  fgets(line, MAX_LINESPACE, filepath);

      while (VALID)

      {
      //Writes first line if it is correct
        if(strcmp(line, "\nStart Program Meta-Data Code:") == 0)
        {
          printf("Start Program Meta-Data Code:");
          continue;
        }
        //Checks if first line is last line
        else if(strcmp(line, "End Program Meta-Data Code.") == 0)
        {
          printf("End Program Meta-Data Code.");

          fprintf(stderr, "First Line Expected: 'End Program Meta-Data Code.'\n");
          fprintf(stderr, "Got: %s\n", line);
          exit(1);
       }

      //allocates memory for pointer in structure
      current_node->operation = malloc(MAX_OPERATION_SPACE);

      //Iterates line by line
      char next_char = fgetc(filepath);

      //If a char is a ';' or ' ' or '\n'

      if ((next_char == ';') || (next_char == ' ') || next_char == '\n')
      {
        //Loops through all white space and extra characters
        while (next_char == ';' || next_char == ' ' || next_char == '\n')
        {
          next_char = fgetc(filepath);
        }
        //Returns character to place needed
        ungetc(next_char, filepath);
        continue;
      }

      //Checks if next char is a '.' then ends while loop
      if (next_char == '.')
      {

        //Loops after last command, then prints out last line
          current_node->next_meta = NULL;
          current_node->operation = "\nEnd Program Meta-Data Code.\n";
          break;
      }

      else
      {
        //If it isnt't anything specified put character back
        ungetc(next_char, filepath);
      }

      //Uses a regular expression that gets:
      //(char) (string) (int) ; and stores each value
      //In a struct defined in structures.h
      fscanf(filepath, "%c(%[^()])%i", &current_node->letter, current_node->operation, &current_node->cycle_time);

      //if
      if(letter_valid(current_node) == 1 && operation_valid(current_node) == 1)
      {
        //Prints out each value in correct format

        if(index == 0)
        {
          first = current_node;
        }

        current_node->next_meta = (struct meta_operation*) malloc(sizeof(struct meta_operation));
        current_node = current_node->next_meta;
        index++;
      }

    }

    meta_vals = first;

    fclose(filepath);
    //returns struct of linkedlist
    return meta_vals;
}
