#include "lindaTuples.h"
#include "sharedMemoryHandler.h"
#include "lindaSpace.h"
#include <iostream>
#include <thread>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>

int parse_arguments2(char str[], LindaSpace ls) {

  char* token = strtok(str, ":");
  std::string str_type(token);

  if(str_type == "int") {
    
    int value = 0;

    try{

      value = stoi(strtok(NULL, " "));

    } catch (const std::exception &e) {

      std::cout << e.what() << std::endl;
      return 1;

    }

    Tuple int_tuple{value};
    ls.write(int_tuple);
    return 0;

  } else if(str_type == "float") {

    float value = 0;

    try{

      value = stof(strtok(NULL, " "));

    } catch (const std::exception &e) {

      std::cout << e.what() << std::endl;
      return 1;

    }

    Tuple float_tuple{value};
    ls.write(float_tuple);
    return 0;

  } else if(str_type == "string") {

    char* value = strtok(NULL, "\"");
    Tuple string_tuple{value};
    ls.write(string_tuple);
    return 0;

  } else {

    std::cout << "Syntax: ./executable {-i (int | float | string):(value | \"string_value\") | (-r | -o) \"regex_string\"} \n";
    return 1;

  }
}

int main(int argc, char *argv[]) {

  const char *mem_name = "/shm21";
  SharedMemoryHandler::SharedMemory* mem;

  try {
    mem = SharedMemoryHandler::getInstance()->open(mem_name);
  } catch (const char *e) {

    try{

      SharedMemoryHandler::getInstance()->create(mem_name);
      char data[10 * 128];
      memset(data, 0, sizeof(data));

      mem = SharedMemoryHandler::getInstance()->open(mem_name);

    } catch (const std::exception &e) {

      std::cout << e.what() << std::endl;
      return 1;

    } catch (const char *e) {

      std::cout << e << std::endl;
      return 1;

    } catch (...) {

      std::cout << "ERROR";
      return 1;

    }
  }

  LindaSpace ls(mem->tupleSpace, &mem->sem_is_resource_reserved, &mem->sem_counting_readers, &mem->cond_waiting_for_changes, &mem->mutex_waiting_for_changes);
  
  for(int c = getopt(argc, argv, "i:r:o:"); c != -1; c = getopt(argc, argv, "i:r:o:")) {

    char* value = optarg;

    switch(c) {

      case 'i':
      {
        parse_arguments2(value, ls);
        break;
      }

      case 'r':
      {
        LindaRegex regex(value);
        RegexTuple tuple{regex};
        ls.remove(tuple);
        break;
      }

      case 'o':
      {
        LindaRegex regex(value);
        RegexTuple tuple{regex};
        ls.read(tuple);
        break;
      }

      case '?':
      {
        if (optopt == 'c')
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);

        else if (isprint(optopt))
          fprintf(stderr, "Unknown option `-%c'.\n", optopt);

        else
          fprintf(stderr,"Unknown option character `\\x%x'.\n",optopt);
          
        return 1;
      }

      default:
      {
        return 1;
      }
    }
  }
}