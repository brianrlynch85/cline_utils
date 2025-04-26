// -----------------------------------------------------------------------
//
//                            example_main.cpp V 0.01
//
//                        (c) Brian Lynch February, 2015
//
// -----------------------------------------------------------------------

#include <iostream>

#include "cline_utils.h"

int main(int argc, char** argv)
{
   std::cout << "-- example_main --" << std::endl;

   double parameter1D = std::nan("1"),
          parameter2D = 3.14;

   int helpFlag    = 0,
       parameter3I = 100;

   std::string parameter4S("gg");

   // Command line parser configuration options
   // h is reserved as the help option
   std::vector<cline_utils::option_longer> longer_options = 
      {
         {"help"     , no_argument      , NULL, 'h', optional_option, typeid(helpFlag).name()          , &helpFlag   , " Optional help option that must be h"},
         {"longName1", required_argument, NULL, 'a', required_option, typeid(parameter1D).name()       , &parameter1D, " Required option with required double argument [physical units]"},
         {"longName2", required_argument, NULL, 'b', required_option, typeid(parameter2D).name()       , &parameter2D, " Required option with required double argument []"},
         {"longName3", required_argument, NULL, 'c', optional_option, typeid(parameter3I).name()       , &parameter3I, " Optional option with a required integer arugment if used"},
         {"longName4", required_argument, NULL, 'd', required_option, typeid(parameter4S.data()).name(), &parameter4S, " Required option with required string argument"},
      };

   cline_utils::CommandLineParser cline(argc, argv, longer_options);//, print_usage);   

   try
   {
      cline.parse_command_line();
      cline.print_input_summary();
   }
   catch(const cline_utils::cline_exception& e)
   {
      std::cerr << e.what();
      cline.print_usage();
   }
   
   return(0);
}