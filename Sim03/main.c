#include "structures.h"
#include "SimpleTimer.h"

//loops through LinkedList and prints out the values
void printList(struct meta_operation *LinkedList)
{
  printf("Start Program Meta-Data Code:\n");
  while (LinkedList->next_meta != NULL)
  {
    print_meta_values(LinkedList);
    printf("%d\n", strcmp(LinkedList->operation, "start"));
    LinkedList = LinkedList->next_meta;
  }
  printf("\nEnd Program Meta-Data Code.\n");

}

int main(int argc, char *argv[])
{
  //reads config file based on user input
  char *file = argv[1];

  //assigns struct to config_file
  struct config_values configFile = get_config(file);

  //reads in meta_data file
  char *input = configFile.file;

  //calls meta_parser.c
  struct meta_operation *metaVals = get_meta(input);

  //Creates PCB linkedList
  struct PCB *firstProccess =  createPCB(metaVals);


  //Loops trhough LinkedListwith function 'loopThroughProcessList()'
  if(parseThroughProcesses(metaVals, firstProccess, configFile) != 0)
  {
    printf("Error ~~ function: ProcessParser\n");
  }

}
