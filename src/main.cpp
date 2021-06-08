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

int parse_input_data(char* const *data, int length, LindaSpace ls) {
  int c;

  while((c = getopt(length, data, "s:i:f:")) != -1) {
    switch(c) {
      case 's':
      {
        char* value = optarg;
        Tuple string_tuple{value};
        ls.write(string_tuple);
        break;
      }
      case 'i':
      {
        char* value = optarg;
        int i;
        try {
          i = stoi(value);
        } catch(const std::exception &e) {
          std::cout << e.what() << std::endl;
          return 1;
        }
        Tuple int_tuple{i};
        ls.write(int_tuple);
        break;
      }
      case 'f':
      {
        char* value = optarg;
        float f;
        try {
          f = stof(value);
        } catch(const std::exception &e) {
          std::cout << e.what() << std::endl;
          return 1;
        }
        Tuple float_tuple{f};
        ls.write(float_tuple);
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
  return 0;
}

std::vector<char*> split_arguments(char* first_item, char str[]) {
  
  std::vector<char*> words {};
  words.push_back(first_item);
  char *token = strtok(str, " ");
  words.push_back(token);

  while (token != NULL) {
    
    token = strtok(NULL, " ");
    words.push_back(token);
  }
  words.pop_back();
  return words;
}

int main(int argc, char *argv[]) {

  if(argc != 3) {
    std::cout << "Exactly one flag and argument are requiered";
    return 1;
  }

  const char *mem_name = "/shm2";
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
    } catch (const char *e) {
      std::cout << e << std::endl;
    } catch (...) {
      std::cout << "ERROR";
    }
 }

  int c;
  LindaSpace ls(mem->tupleSpace, &mem->sem_is_resource_reserved, &mem->sem_counting_readers, &mem->cond_waiting_for_changes, &mem->mutex_waiting_for_changes);

  while((c = getopt(argc, argv, "i:r:o:")) != -1) {
    char* value = optarg;
    switch(c) {
      case 'i':
      {
        std::vector<char*> new_value = split_arguments(argv[0], value);
        int size = new_value.size();
        char** new_value2 = &new_value[0];
        parse_input_data(new_value2, size, ls);
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
//   try {
//     const char *mem_name = "/shm";
//     SharedMemoryHandler::getInstance()->create(mem_name);
//     char data[10 * 128];
//     memset(data, 0, sizeof(data));
//     LindaTuples l;
//     l.output({0, 1.f, "kupa", -2});
//     l.output({"dupa", -2, "papuga"});
//     // l.output({});
//     l.output({""});
//     l.serialize(data, 0);
//     l.deserialize(data);
//     std::cout << std::endl << (int)l.getTuplesAmount() << std::endl;
//   } catch (const std::exception &e) {
//     std::cout << e.what() << std::endl;
//   } catch (const char *e) {
//     std::cout << e << std::endl;
//   } catch (...) {
//     std::cout << "ERROR";
//   }

  // LindaSpace ls(true);
  // Tuple testTuple{1};
  // LindaRegex regex1("int:==1");
  // RegexTuple testRegexTuple{regex1};
  // std::thread t3(&LindaSpace::remove, &ls, testRegexTuple);
  // std::thread t2(&LindaSpace::write, &ls, testTuple);
  // sleep(2);
  // std::thread t1(&LindaSpace::read, &ls, testRegexTuple);
  // std::thread t4(&LindaSpace::write, &ls, testTuple);

  // t1.join();
  // t2.join();
  // t3.join();
  // t4.join();
