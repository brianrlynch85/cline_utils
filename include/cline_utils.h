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

#include <cmath>
#include <vector>
#include <iostream>

#include <stdlib.h>    /* for exit */
#include <getopt.h>    /* for getopt_long; POSIX standard getopt is in unistd.h */
#include <limits.h>

#include "table_printer.h"

namespace cline_utils
{
   /************************************************************************/
   /*
   * \brief Store additional relevation data beyond the POSIX option struct
   *
   */
   struct option_longer : option
   {
      // These members are inherited from the option struct
      //const char *name;      /**< Long name of the option (no spaces) */
      //int         has_arg;   /**< 1,0 flag dictating whether the option has mandatory argument */
      //int        *flag;      /**< If NULL, get_opt_long returns val (the short option character itself) */
      //int         val;       /**< Value of option to return or load into flag */

      int is_mandatory_opt;    /**< 1,0 flag for required_argument or optional_argument */
      std::string type_string; /**< typeid(...).name() to match stored data with void dataVal */
      void *dataVal;           /**< Arbitrary data array or otherwise stored in the struct */

      std::string desc_string;

      /************************************************************************/
      /*
      * \brief Calculates the inverse of matrix A. A is a N x N matrix stored 
      *        stored as a linear data array. Users responsibility to allocate.
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
         : is_mandatory_opt(is_mandatory_opt_), type_string(type_string_), dataVal(dataVal_), desc_string(description_)
      {
         // Could probably also be initialized via initializer list, but since they
         // are from the base class(struct), I chose to leave them here as reminder.
         // Base class(struct) has no user-defined constructor that I can tell and the
         // compiler generated one is used.
         name = name_;
         has_arg = has_arg_;
         flag = flag_;
         val = val_;
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

         std::vector<std::string> optargvec_; /**< Stores option argument string characters when parsing */
         std::vector<int> optvec_; /**< Stores option characters as integer */
         //std::vector<int> optind_; /**< Long name of the option (no spaces) */
         //std::vector<int> opterr_; /**< Long name of the option (no spaces) */
         
         //std::vector<int> opt_strings;
         //std::vector<std::string> arg_strings;

         std::vector<option_longer> opt_cfg;

         std::string fmt_string;

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
         const std::vector<struct option_longer> &option_config
                       )
         : argc_(argc), argv_(argv), opt_cfg(option_config)
      {}

      /************************************************************************/
      /*
      * \brief Unpack command line data into internal class members
      *
      *     @return None
      * 
      */
      void create_option_strings()
      {
         int opt = 0;

         // Create the format string from the option_longer struct
         while((opt = getopt_long(this->argc_, this->argv_, this->fmt_string.c_str(), this->opt_cfg.data(), NULL)) != -1)
         {
            switch (opt) //Recall that character constants (which have a type int) comparisons to opt (which is also an int)
            {  
               case '?': //Unrecognized command line option
                  std::cerr << "create_option_strings(...) - Unrecognized option: " << optopt << std::endl;
                  this->print_usage();
                  exit(EXIT_FAILURE);
                  
               case ':': //Missing argument for option
                  std::cerr << "create_option_strings(...) - Missing argument for option: " << optopt << std::endl;
                  this->print_usage();
                  exit(EXIT_FAILURE);

               case 1:
                  std::cerr << "create_option_strings(...) - Arguments with no corresponding option: " << optarg << std::endl;
                  this->print_usage();
                  exit(EXIT_FAILURE);

               default:
                  //std::cout << optarg << " " << optopt << " " << char(opt) << std::endl;
                  this->optargvec_.push_back(optarg);
                  this->optvec_.push_back(opt);
            }
         }

         this->validate_required_options();
      }

      /************************************************************************/
      /*
      * \brief Ensure that all required_options are present
      *
      *     @return None
      * 
      */
      void validate_required_options()
      {
         bool found = false;

         for(size_t option_index = 0; option_index < this->opt_cfg.size(); ++option_index)
         {
            found = false;
            if(required_option == this->opt_cfg[option_index].is_mandatory_opt)
            {
               for(auto itr : this->optvec_)
               {
                  // Loop through current option type strings to find the option currently being parsed by getopt
                  if(this->opt_cfg[option_index].val == itr)
                  {
                     found = true;
                     break;
                  }
               }

               if(false == found)
               {
                  std::cerr << "validate_required_options(...) - Missing required option in command line args: -" << char(this->opt_cfg[option_index].val) << std::endl;
                  this->print_usage();
                  exit(EXIT_FAILURE);
               }
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
         std::string result;

         for(size_t option_index = 0; option_index < this->opt_cfg.size(); ++option_index)
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
      * \brief Parse the command line and store the data
      *
      *     @return None.
      * 
      */
      void parse_command_line()
      {
         bool found_option_type = 0;

         // Create the format string from the option_longer struct
         this->create_option_format_string();      
         this->create_option_strings();

         for(auto itr : this->optvec_)
         {
            // Loop through current option type strings to find the option currently being parsed by getopt
            found_option_type = false;
            for(size_t option_index = 0; option_index < this->opt_cfg.size(); ++option_index)
            {
               if(this->opt_cfg[option_index].val == itr)
               {
                  std::string optTypeString = this->opt_cfg[option_index].type_string;
                  std::string optArgString = this->optargvec_[option_index];

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
               std::cerr << "parse_command_line(...) - Unable to match option type string: " << char(itr) << std::endl;
               this->print_usage();
               exit(EXIT_FAILURE);
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
         std::string result("Unknown??? ");
         
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
         std::cerr << "------------------------------------------------------" << std::endl; 
         std::cerr << "General Usage: ";
         std::cerr << argv_[0] << " -option_character <option_argument>" << std::endl;
         std::cerr << "------------------------------------------------------" << std::endl;
         std::cerr << "------------------------------------------------------" << std::endl; 
         std::cerr << "Option & Argument Table" << std::endl;
         std::cerr << "------------------------------------------------------" << std::endl;

         bprinter::TablePrinter tp(&std::cerr);
         tp.AddColumn("Option Long Name ", 25);
         tp.AddColumn("Option Character ", 20);
         tp.AddColumn("Argument Type ", 20);
         tp.AddColumn("Option Required ", 20);
         tp.AddColumn("Argument Required ", 20);
         tp.AddColumn("Description [Physical Units] ", 75);
      
         tp.PrintHeader();
         
         for(auto itr: this->opt_cfg)
         {
            tp << itr.name; 
            tp << char(itr.val);
            tp << this->get_type_string(itr.type_string);
            tp << itr.is_mandatory_opt;
            tp << itr.has_arg;
            tp << itr.desc_string;
         }
         tp.PrintFooter();
      }

   }; // End class CommandLineParser
}

#endif
