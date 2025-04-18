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
          parameter2D = std::nan("1");

   int parameter3I = 100;    //Maximum number of iterations while fitting

   std::string parameter4S; //Command line option input file

   // Command line parser configuration options
   std::vector<struct cline_utils::option_longer> longer_options = 
      {

         {"name_for_parameter1D", required_argument, NULL, 'a', required_option, typeid(parameter1D).name()       , &parameter1D, " Required option with required double argument [physical units]"},
         {"name_for_parameter2D", required_argument, NULL, 'b', required_option, typeid(parameter2D).name()       , &parameter2D, " Required option with required double argument []"},
         {"name_for_parameter3I", required_argument, NULL, 'c', optional_option, typeid(parameter3I).name()       , &parameter3I, " Optional option with a required integer arugment if used"},
         {"name_for_parameter4S", required_argument, NULL, 'd', required_option, typeid(parameter4S.data()).name(), &parameter4S, " Required option with required string argument"},
         {"help", required_argument, NULL, 'h', required_option, typeid(parameter4S.data()).name(), &parameter4S, " Required option with required string argument"}
      };

   cline_utils::CommandLineParser cline(argc, argv, longer_options);//, print_usage);
   cline.parse_command_line();
}