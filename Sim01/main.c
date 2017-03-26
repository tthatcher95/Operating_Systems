#include "structures.h"


int main(int argc, char const *argv[])
{
  FILE* file = fopen(argv[1], "r");

  //Check if file is valid
  if(file == NULL)
  {
    fprintf(stderr, "Config File: '%s' entered is not valid\n", argv[1]);
    exit(1);
  }

  //assigns struct to config_file
  struct config_values config_file = get_config(file);
  fclose(file);


  char *input = config_file.file;
  FILE* meta_file = fopen(input, "r");

  //checks if meta data file is correct
  if(meta_file == NULL) {
    printf("Meta-File: '%s' is not valid\n", input);
    exit(1);
  }

  //calls meta_parser.c
  get_meta(meta_file);
  fclose(meta_file);


}
