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

int parse_arguments(char str[], LindaSpace ls) {

  for(char* token = strtok(str, " "); token != NULL; token = strtok(NULL, " ")) {

    std::string str(token);

    if(str == "-i") {

      try{

        int value = stoi(strtok(NULL, " "));
        Tuple int_tuple{value};
        ls.write(int_tuple);

      } catch (const std::exception &e) {

        std::cout << e.what() << std::endl;
        return 1;

      }
    } else if(str == "-f") {

      try{

        float value = stof(strtok(NULL, " "));
        Tuple float_tuple{value};
        ls.write(float_tuple);

      } catch (const std::exception &e) {

        std::cout << e.what() << std::endl;
        return 1;

      }

    } else if(str == "-s") {

      char* value = strtok(NULL, "\'");
      Tuple string_tuple{value};
      ls.write(string_tuple);

    } else {

      std::cout << "Unknown type. Expected {[-i integer_value] or [-f float_value] or [-s string_value]}\n";
      return 1;

    }
  }
  return 0;
}

int main(int argc, char *argv[]) {

  if(argc != 3) {
    std::cout << "Exactly one flag and argument are requiered";
    return 1;
  }

  const char *mem_name = "/shm3";
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
        parse_arguments(value, ls);
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
