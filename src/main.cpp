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

Tuple parse_args(std::string str_values, LindaSpace ls) {
  std::vector<std::string> values;

  for(auto token = str_values.substr(0, str_values.find(",")); token != ""; token = str_values.substr(0, str_values.find(","))) {
    
    values.push_back(token);

    if(str_values.find(",") != std::string::npos)
      str_values.erase(0, str_values.find(",") + 1);
    else
      str_values = "";
  }

  Tuple tuple;

  for(auto value : values) {

    std::string str_type = value.substr(0, value.find(":"));
    std::string str_value = value.substr(value.find(":") + 1, value.length());

    if(str_type == "int") {
      
      try{

        int v = stoi(str_value);
        tuple.push_back(v);

      } catch (const std::exception &e) {

        std::cout << e.what() << std::endl;
        throw e;

      }

    } else if(str_type == "float") {

      try{

        float v = stof(str_value);
        tuple.push_back(v);

      } catch (const std::exception &e) {

        std::cout << e.what() << std::endl;
        throw e;

      }

    } else if(str_type == "string") {
      
      str_value.erase(remove( str_value.begin(), str_value.end(), '\'' ),str_value.end());
      tuple.push_back(str_value);

    } else {

      std::cout << "Syntax: ./executable -o {(int | float | string):(value | \'string_value\'),} | (-r | -i) \"regex_string\" \n";
      throw exception();
    }
  }

  ls.write(tuple);
  return tuple;
}

int main(int argc, char *argv[]) {

  const char *mem_name = "/shm31";
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
  int o_flag = 0;
  int i_flag = 0;
  int r_flag = 0;
  int t_flag = 0;
  Tuple temp;
  RegexTuple r_temp;
  int timeout = 10;


  for(int c = getopt(argc, argv, "i:r:o:t:"); c != -1; c = getopt(argc, argv, "i:r:o:t:")) {

    std::string str_values(optarg);
    

    switch(c) {

      case 'o':
      {
        if(o_flag || i_flag || r_flag) {

          std::cout << "Można podać tylko jedną akcję na raz\n";
          return 1;
        }
          
        try {
          temp = parse_args(str_values, ls);
        } catch (const char *e) {
          return 1;
        } 
        
        o_flag = 1;
        break;
      }

      case 'i':
      {
        if(o_flag || i_flag || r_flag) {

          std::cout << "Można podać tylko jedną akcję na raz\n";
          return 1;
        }

        LindaRegex regex(optarg);
        RegexTuple tuple{regex};

        r_temp = tuple;
        
        i_flag = 1;
        break;
      }

      case 'r':
      {
        if(o_flag || i_flag || r_flag) {

          std::cout << "Można podać tylko jedną akcję na raz\n";
          return 1;
        }

        LindaRegex regex(optarg);
        RegexTuple tuple{regex};
        
        r_temp = tuple;
        
        r_flag = 1;
        break;
      }

      case 't':
      {
        if(t_flag) {

          std::cout << "Można podać timeout tylko raz\n"; 
          return 1;
        }
        
        timeout = stoi(optarg);
        break;
      }

      case '?':
      {
        if (optopt == 'c') {
          
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
          std::cout << "Syntax: ./executable -o {(int | float | string):(value | \'string_value\'),} | (-r | -i) \"regex_string\" \n";
        }

        else if (isprint(optopt)) {
          
          fprintf(stderr, "Unknown option `-%c'.\n", optopt);
          std::cout << "Syntax: ./executable -o {(int | float | string):(value | \'string_value\'),} | (-r | -i) \"regex_string\" \n";
        }

        else {
          
          fprintf(stderr,"Unknown option character `\\x%x'.\n",optopt);
          std::cout << "Syntax: ./executable -o {(int | float | string):(value | \'string_value\'),} | (-r | -i) \"regex_string\" \n";
        }

        return 1;
      }

      default:
      {
        return 1;
      }
    }
  }

  if(o_flag) {

    try {

      ls.write(temp);

    } catch (const std::exception &e) {

        std::cout << e.what() << std::endl;
        return 1;

    } 

  } else if(i_flag) {

    try {

      ls.remove(r_temp, timeout);

    } catch (const std::exception &e) {

      std::cout << e.what() << std::endl;
      return 1;

    }

  } else if(r_flag) {

    try {

      ls.read(r_temp, timeout);

    } catch (const std::exception &e) {

      std::cout << e.what() << std::endl;
      return 1;

    } 
  }
  return 0;
}