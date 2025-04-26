// -----------------------------------------------------------------------
//
//                            cline_utils.h V 0.01
//
//                        (c) Brian Lynch February, 2015
//
// -----------------------------------------------------------------------

#ifndef cline_utils_h
#define cline_utils_h

# define optional_option 0
# define required_option 1
# define no_argument -1

#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>

#include <stdlib.h>    /* for exit */
#include <getopt.h>    /* for getopt_long; POSIX standard getopt is in unistd.h */
#include <limits.h>

#include "table_printer.h"

namespace cline_utils
{
   // Define a new exception class that inherits from
   // std::exception
   class cline_exception : public std::exception {
      private:
         std::string message;
      
      public:
         // Constructor accepts a const char* that is used to set
         // the exception message
         cline_exception(const std::string msg) : message(msg)
         {
         }
      
         // Override the what() method to return our message
         const char* what() const throw()
         {
            return message.c_str();
         }
      };

   /************************************************************************/
   /*
   * \brief Store additional relevation data beyond the POSIX option struct
   *
   */
   struct option_longer
   {
      // These members mimmick the option struct. Deiving from it didn't work
      const char *name;      /**< Long name of the option (no spaces) */
      int         has_arg;   /**< 1,0 flag dictating whether the option has mandatory argument */
      int        *flag;      /**< If NULL, get_opt_long returns val (the short option character itself) */
      int         val;       /**< Value of option to return or load into flag */

      int is_mandatory_opt;    /**< 1,0 flag for required_argument or optional_argument */
      std::string type_string; /**< typeid(...).name() to match stored data with void dataVal */
      void *dataVal;           /**< Arbitrary data array or otherwise stored in the struct */

      std::string desc_string;

      /************************************************************************/
      /*
      * \brief An option struct with a bit of additional information derived from POSIX option
      *           
      *   @param[in] const char *name_: long name of the option
      *   @param[in] int has_arg: required_argument or optional_argument
      *   @param[in] int *flag: Only NULL is supported by this wrapper at the moment
      *   @param[in] int val: short character (casted to int) name of the option such as 'f' or 'c'
      *   @param[in] int is_mandatory_opt: required_option or optional_option
      *   @param[in] std::string type_string_: typeid(...).name() to match stored data with void dataVal
      *   @param[in] void *dataVal: Arbitrary data array or otherwise stored in the struct
      *   
      *   @return None
      * 
      */
      /* Here is an example initialization of a set of command line options or see source/example_main.cpp
         std::vector<struct option_longer> longer_options = 
            {
               {"filename"              , required_argument, NULL, 'f', required_option, typeid(input_filename.data()).name(), &input_filename, "Descriptive statement 1"},
               {"ion_saturation_current", required_argument, NULL, 'i', required_option, typeid(Is_guess).name()             , &Is_guess, "Descriptive statement 2"}
            };
      */
      option_longer(
         const char *name_,
         int has_arg_,
         int *flag_,
         int val_,
         int is_mandatory_opt_,
         std::string type_string_,
         void *dataVal_,
         std::string description_
                  ) 
         : name(name_), has_arg(has_arg_), flag(flag_), val(val_), 
           is_mandatory_opt(is_mandatory_opt_), type_string(type_string_), dataVal(dataVal_),
           desc_string(description_)
      {}

      static std::vector<option> pack_into_option_array(std::vector<option_longer> input_)
      {
         std::vector<option> result;
         for(size_t i = 0; i < input_.size(); ++i)
         {
            result.push_back({input_[i].name, input_[i].has_arg, input_[i].flag, input_[i].val});
         }
         return(result);
      }
   };

   /************************************************************************/
   /*
   * \brief Class for parsing command line options. Not fully generic at all 
   *        but gets the job done with no memory leaks.
   *
   */
   class CommandLineParser
   {
      private:

         int argc_;
         char **argv_;

         std::map<int, std::string> optarg_map; /**< Stores option argument string characters when parsing */
         //std::vector<int> optvec_; /**< Stores option characters as integer */
         //std::vector<int> optind_; /**< Long name of the option (no spaces) */
         //std::vector<int> opterr_; /**< Long name of the option (no spaces) */
         
         //std::vector<int> opt_strings;
         //std::vector<std::string> arg_strings;

         std::vector<cline_utils::option_longer> opt_cfg;

         std::string fmt_string;

        // std::vector<std::tuple<char, int, int>> optChar2 

      public:

      /************************************************************************/
      /*
      * \brief No empty constructor.
      *
      */
      CommandLineParser() = delete;

      /************************************************************************/
      /*
      * \brief Create a CommandLineParser object.
      *
      *     @param[in] int argc: Integer variable storing number of command line arguments (including executable name)    
      *     @param[in] char **argv: Array of character pointers listing all the arguments    
      *     @param[in] std::vector<struct option_longer> &option_config: Vector of option_longer structs
      *     @return None.
      * 
      */
      CommandLineParser(
         int argc,
         char **argv,
         const std::vector<cline_utils::option_longer> &option_config
                       )
            : argc_(argc), argv_(argv), opt_cfg(option_config)
      {
         this->opt_cfg.push_back({NULL, 0, NULL, 0, required_option, "", NULL, ""});
      }

      /************************************************************************/
      /*
      * \brief Ensure that all required_options are present
      *
      *     @return None
      * 
      */
      void check_required_options()
      {
         // Exclude the last element from for loop iteration since getopt_long requires
         // last member of option structs array be all zeroes
         for(size_t option_index = 0; option_index < this->opt_cfg.size() - 1; ++option_index)
         {
            if(required_option == this->opt_cfg[option_index].is_mandatory_opt)
            {
               auto itr = this->optarg_map.find(this->opt_cfg[option_index].val);

               if (itr == this->optarg_map.end())
               {
                  std::stringstream ss("");
                  ss << "******************************************************************************************" << std::endl;
                  ss << "check_required_options(...) - Missing required option in command line args: -" << char(this->opt_cfg[option_index].val) << std::endl;
                  ss << "******************************************************************************************" << std::endl;
                  throw cline_utils::cline_exception(std::string(ss.str()));
                  break;
               }
            }
         }
      }

      /************************************************************************/
      /*
      * \brief Ensure no options long or short names are duplicates in input
      *        configuration struct.
      *
      *     @return None
      * 
      */
      void check_duplicate_option_config_names()
      {
         std::vector<std::string> long_names;
         std::vector<int> short_names;

         // Exclude the last element from for loop iteration since getopt_long requires
         // last member of option structs array be all zeroes
         for(size_t option_index = 0; option_index < this->opt_cfg.size() - 1; ++option_index)
         {
            long_names.push_back(this->opt_cfg[option_index].name);
            short_names.push_back(this->opt_cfg[option_index].val);
         }

         // Check option long name configuration for duplicates
         std::unordered_set<std::string> usln;
         for (auto i : long_names) 
         {
            if (usln.find(i) != usln.end())
            {
               std::stringstream ss("");
               ss << "****************************************************************************************************************" << std::endl;
               ss << "check_duplicate_options(...) - Struct option.name field const char* configured with duplicate entries: " << i << std::endl;
               ss << "****************************************************************************************************************" << std::endl;
               throw cline_utils::cline_exception(std::string(ss.str()));
            }
            else
            {
               usln.insert(i);
            }
         }

         // Check option short name configuration for duplicates
         std::unordered_set<int> ussn;
         for (auto i : short_names) 
         {
            if (ussn.find(i) != ussn.end())
            {
               std::stringstream ss("");
               ss << "****************************************************************************************************************" << std::endl;
               ss << "check_duplicate_options(...) - Struct option->flag field int* configured with duplicate entries: " << char(i) << std::endl;
               ss << "****************************************************************************************************************" << std::endl;
               throw cline_utils::cline_exception(std::string(ss.str()));
            }
            else
            {
               ussn.insert(i);
            }
         }
      }


      /************************************************************************/
      /*
      * \brief Create a format string to be fed into get_opt
      *           
      *     @return None
      * 
      */
      void create_option_format_string()
      {
         // Loop through the option_longer struct and construct the
         std::string result(":");

         // Exclude the last element from for loop iteration since getopt_long requires
         // last member of option structs array be all zeroes
         for(size_t option_index = 0; option_index < this->opt_cfg.size() - 1; ++option_index)
         {
            result += char(this->opt_cfg[option_index].val);

            switch(this->opt_cfg[option_index].has_arg)
            {
               case (required_argument):
                  result.append(":");
                  break;

               case (optional_argument):
                  result.append("::");
                  break;
            }
         }

         this->fmt_string = result;
      }

      /************************************************************************/
      /*
      * \brief Unpack command line data into internal class members
      *
      *     @return None
      * 
      */
      void parse_options_arguments()
      {
         int opt = 0;

         std::stringstream ss("");

         std::vector<option> longer_options = option_longer::pack_into_option_array(this->opt_cfg);

         while(true)
         {
            int option_index = 0;
            opt = getopt_long(this->argc_, this->argv_, this->fmt_string.c_str(), longer_options.data(), &option_index);
            if(-1 == opt)
            {
               break;
            }

            switch (opt) //Recall that character constants (which have a type int) comparisons to opt (which is also an int)
            {  
               case 'h': //Help option
                  ss << "*************************************************************************" << std::endl;
                  ss << "parse_options_arguments(...) - Help Option Selected: " << char(optopt) << std::endl;
                  ss << "*************************************************************************" << std::endl;
                  throw cline_utils::cline_exception(std::string(ss.str()));
                  break;

               case '?': //Unrecognized command line option
                  ss << "*************************************************************************" << std::endl;
                  ss << "parse_options_arguments(...) - Unrecognized option: " << optopt << " " << char(opt) << std::endl;
                  ss << "*************************************************************************" << std::endl;
                  throw cline_utils::cline_exception(std::string(ss.str()));
                  break;
                 
               case ':': //Missing argument for option
                  ss << "*************************************************************************" << std::endl;
                  ss << "parse_options_arguments(...) - Missing argument for option: " << char(optopt) << std::endl;
                  ss << "*************************************************************************" << std::endl;
                  throw cline_utils::cline_exception(std::string(ss.str()));
                  break;

               case 1:
                  ss << "*************************************************************************" << std::endl;
                  ss << "parse_options_arguments(...) - Arguments with no corresponding option: " << optarg << std::endl;
                  ss << "*************************************************************************" << std::endl;
                  throw cline_utils::cline_exception(std::string(ss.str()));
                  break;

               default:
                  // If the size stays the same, then insert failed and we know its likely a duplicate key input on
                  // the command line.
                  size_t sizeBefore = this->optarg_map.size();
                  this->optarg_map.insert({opt,optarg});
                  if(this->optarg_map.size() == sizeBefore)
                  {
                     ss << "*************************************************************************" << std::endl;
                     ss << "parse_options_arguments(...) - Duplicate option struct.val input found: " << char(opt) << std::endl;
                     ss << "*************************************************************************" << std::endl;
                     throw cline_utils::cline_exception(std::string(ss.str()));
                  }
                  break;
           }
         }

         this->check_required_options();
      }

      /************************************************************************/
      /*
      * \brief Parse the command line and store the data
      *
      *     @return None.
      * 
      */
      void parse_command_line()
      {
         bool found_option_type = 0;
         
         // Create the format string from the option_longer struct
         this->check_duplicate_option_config_names();

         // Create the required/optional options and required/not argument strings
         this->create_option_format_string();

         this->parse_options_arguments();

         for (auto const & [key, val] : this->optarg_map)
         {
            // Loop through current option type strings to find the option currently being parsed by getopt
            found_option_type = false;
            // Exclude the last element from for loop iteration since getopt_long requires
            // last member of option structs array be all zeroes
            for(size_t option_index = 0; option_index < this->opt_cfg.size() - 1; ++option_index)
            {
               if(this->opt_cfg[option_index].val == key)
               {
                  std::string optTypeString = this->opt_cfg[option_index].type_string;
                  std::string optArgString = val;

                  if(std::string(typeid(char *).name()) == optTypeString) // It's a string
                  {
                     *(std::string *)this->opt_cfg[option_index].dataVal = optArgString;
                     found_option_type = true;
                     //std::cout << "char * " << this->opt_cfg[option_index].val << " " << optArgString << " " << *(std::string *)this->opt_cfg[option_index].dataVal << " " << optTypeString << " " << option_index << std::endl;
                  }
                  else if(std::string(typeid(double).name()) == optTypeString) // It's a double
                  {
                     char *endPtr;
                     double test = strtod(optArgString.c_str(), &endPtr);
                     *(double *)this->opt_cfg[option_index].dataVal = (endPtr != optArgString) ? test : std::nan("1");
                     found_option_type = true;
                     //std::cout << "double " << this->opt_cfg[option_index].val << " " << optArgString << " " << *(double *)this->opt_cfg[option_index].dataVal << " " << optTypeString << " " << option_index << std::endl;
                  }
                  else if(std::string(typeid(int).name()) == optTypeString) // It's an int
                  {
                     char *endPtr;
                     int test = std::strtol(optArgString.c_str(), &endPtr, 0);
                     *(int *)this->opt_cfg[option_index].dataVal = (endPtr != optArgString) ? test : INT_MIN;
                     found_option_type = true;
                     //std::cout << "int " << this->opt_cfg[option_index].val << " " << optArgString << " " << *(int *)this->opt_cfg[option_index].dataVal << " " << optTypeString << " " << option_index << std::endl;
                  }
               }
               // If option has been found and handled, move on.
               if(true == found_option_type) break;
            }
               
            if(false == found_option_type)
            {
               std::stringstream ss("");
               ss << "*************************************************************************" << std::endl;
               ss << "parse_command_line(...) - Unable to match option type string: " << char(key) << std::endl;
               ss << "*************************************************************************" << std::endl;
               throw cline_utils::cline_exception(std::string(ss.str()));
               break;
            }
         }
      }

      /************************************************************************/
      /*
      * \brief Terrible hack function to print option data type. Can't use more
      *        elegant methods because internally this class only sees void data.
      *
      *     @return None.
      * 
      */
      std::string get_type_string(const std::string type_name)
      {  
         std::string result("None ");
         
         if("Pc" == type_name)
         {
            result = "char * ";
         }
         else if("d" == type_name)
         {
            result = "double ";
         }
         else if("f" == type_name)
         {
            result = "float ";
         }
         else if("i" == type_name)
         {
            result = "int ";
         }

         return(result);
      }

      /************************************************************************/
      /*
      * \brief Print usage example & available data.
      *
      *     @return None.
      * 
      */
      void print_usage()
      {  
         std::cerr << std::endl;
         std::cerr << "--------------------------------------------------------------" << std::endl; 
         std::cerr << "foo@bar:~$ ";
         std::cerr << argv_[0] << " -option_character <argument_value>" << std::endl;
         std::cerr << "--------------------------------------------------------------" << std::endl;
 
         bprinter::TablePrinter tp(&std::cerr);
         tp.AddColumn("Option Long Name ", 25);
         tp.AddColumn("Option Character ", 20);
         tp.AddColumn("Argument Type ", 20);
         tp.AddColumn("Option Required ", 20);
         tp.AddColumn("Argument Required ", 20);
         tp.AddColumn("Argument Default Value ", 30);
         tp.AddColumn("Description ", 75);
      
         tp.PrintHeader();
         
         for(size_t option_index = 0; option_index < this->opt_cfg.size() - 1; ++option_index)
         {

            tp << this->opt_cfg[option_index].name;
            tp << char(this->opt_cfg[option_index].val);
            tp << this->get_type_string(this->opt_cfg[option_index].type_string);
            tp << this->opt_cfg[option_index].is_mandatory_opt;
            tp << this->opt_cfg[option_index].has_arg;

            std::string type_name = this->opt_cfg[option_index].type_string;
            if("Pc" == type_name)
            {
               tp << *(std::string *)this->opt_cfg[option_index].dataVal;
            }
            else if("d" == type_name)
            {
               tp << *(double *)this->opt_cfg[option_index].dataVal;
            }
            else if("f" == type_name)
            {
               tp << *(float *)this->opt_cfg[option_index].dataVal;
            }
            else if("i" == type_name)
            {
               tp << *(int *)this->opt_cfg[option_index].dataVal;
            }
            else
            {
               tp << "None ";
            }

            tp << this->opt_cfg[option_index].desc_string;

         }
         tp.PrintFooter();
      }

      /************************************************************************/
      /*
      * \brief Print summary of command line available data.
      *
      *     @return None.
      * 
      */
     void print_input_summary()
     {  
         bprinter::TablePrinter tp(&std::cerr);
         tp.AddColumn("Option Long Name ", 25);
         tp.AddColumn("Option Character ", 20);
         tp.AddColumn("Argument Type ", 20);
         tp.AddColumn("Option Required ", 20);
         tp.AddColumn("Argument Required ", 20);
         tp.AddColumn("Current Argument Value ", 30);
         tp.AddColumn("Description ", 75);
     
         tp.PrintHeader();

         for(size_t option_index = 0; option_index < this->opt_cfg.size() - 1; ++option_index)
         {

            tp << this->opt_cfg[option_index].name;
            tp << char(this->opt_cfg[option_index].val);
            tp << this->get_type_string(this->opt_cfg[option_index].type_string);
            tp << this->opt_cfg[option_index].is_mandatory_opt;
            tp << this->opt_cfg[option_index].has_arg;

            std::string type_name = this->opt_cfg[option_index].type_string;
            if("Pc" == type_name)
            {
               tp << *(std::string *)this->opt_cfg[option_index].dataVal;
            }
            else if("d" == type_name)
            {
               tp << *(double *)this->opt_cfg[option_index].dataVal;
            }
            else if("f" == type_name)
            {
               tp << *(float *)this->opt_cfg[option_index].dataVal;
            }
            else if("i" == type_name)
            {
               tp << *(int *)this->opt_cfg[option_index].dataVal;
            }
            else
            {
               tp << "None ";
            }

            tp << this->opt_cfg[option_index].desc_string;

         }
         tp.PrintFooter();
     }

   }; // End class CommandLineParser
}

#endif
