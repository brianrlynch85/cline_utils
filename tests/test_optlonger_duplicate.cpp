// -----------------------------------------------------------------------
//
//                      test_optlonger_duplicate.cpp V 0.01
//
//                        (c) Brian Lynch February, 2015
//
// -----------------------------------------------------------------------

#include <iostream>

#define CATCH_CONFIG_RUNNER
#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_session.hpp"

#include "cline_utils.h"

// Cheesy global test variables to make things quicker.
int _G_argc;
char** _G_argv; //std::vector<std::string> Gargv;
cline_utils::CommandLineParser *_G_cline;   

/************************************************************************/
/*
* \brief Duplicate config option
* 
*/
TEST_CASE("Duplicate Config Long Option","[MUSTFAIL]")
{
   //std::cout << "-- BEGIN Duplicate Option Test --" << std::endl;

   double parameter1D = std::nan("1"),
          parameter2D = 3.14;

   int helpFlag    = 0,
       parameter3I = 100;

   std::string parameter4S("");

   std::vector<cline_utils::option_longer> longer_options = 
      {
         {"help"     , no_argument      , NULL, 'h', optional_option, typeid(helpFlag).name()          , &helpFlag   , " Optional help option that must be h"},
         {"longName1", required_argument, NULL, 'a', required_option, typeid(parameter1D).name()       , &parameter1D, " Required option with required double argument [physical units]"},
         {"longName1", required_argument, NULL, 'e', required_option, typeid(parameter1D).name()       , &parameter1D, " Required option with required double argument [physical units]"},
         {"longName2", required_argument, NULL, 'b', required_option, typeid(parameter2D).name()       , &parameter2D, " Required option with required double argument []"},
         {"longName3", required_argument, NULL, 'c', optional_option, typeid(parameter3I).name()       , &parameter3I, " Optional option with a required integer arugment if used"},
         {"longName4", required_argument, NULL, 'd', required_option, typeid(parameter4S.data()).name(), &parameter4S, " Required option with required string argument"},
      };

   _G_cline->add_options(longer_options);
   REQUIRE_THROWS(_G_cline->parse_command_line());
   _G_cline->print_input_summary();
   _G_cline->delete_all_options();

   //std::cout << "-- END Duplicate Config Option Test --" << std::endl;
}

/************************************************************************/
/*
* \brief Duplicate config option
* 
*/
TEST_CASE("Duplicate Config Short Option","[MUSTFAIL]")
{
   //std::cout << "-- BEGIN Duplicate Option Test --" << std::endl;

   double parameter1D = std::nan("1"),
          parameter2D = 3.14;

   int helpFlag    = 0,
       parameter3I = 100;

   std::string parameter4S("");

   std::vector<cline_utils::option_longer> longer_options = 
      {
         {"help"     , no_argument      , NULL, 'h', optional_option, typeid(helpFlag).name()          , &helpFlag   , " Optional help option that must be h"},
         {"longName1", required_argument, NULL, 'a', required_option, typeid(parameter1D).name()       , &parameter1D, " Required option with required double argument [physical units]"},
         {"longName11", required_argument, NULL, 'a', required_option, typeid(parameter1D).name()       , &parameter1D, " Required option with required double argument [physical units]"},
         {"longName2", required_argument, NULL, 'b', required_option, typeid(parameter2D).name()       , &parameter2D, " Required option with required double argument []"},
         {"longName3", required_argument, NULL, 'c', optional_option, typeid(parameter3I).name()       , &parameter3I, " Optional option with a required integer arugment if used"},
         {"longName4", required_argument, NULL, 'd', required_option, typeid(parameter4S.data()).name(), &parameter4S, " Required option with required string argument"},
      };

   _G_cline->add_options(longer_options);
   REQUIRE_THROWS(_G_cline->parse_command_line());
   _G_cline->print_input_summary();
   _G_cline->delete_all_options();

   //std::cout << "-- END Duplicate Config Option Test --" << std::endl;
}

/************************************************************************/
/*
* \brief Driver function for Catch2 tests. Not clear to me how to use
*        Catch::Session().run() with LONG command line arguments... 
*        so quick hack using globals
*
*     @param[in] int argc: Integer variable storing number of command line arguments (including executable name)    
*     @param[in] char **argv: Array of character pointers listing all the arguments    
*     @return int: Success status
* 
*/
int runCatchTests(int argc, char* const argv[])
{
   //std::cout << "-- BEGIN runCatchTests --" << std::endl;

   _G_argc = argc;

   // Allocate memory and copy strings
   // std::copy(argv + 1, argv + Gargc, std::back_inserter(Gargv));
   _G_argv = new char*[(_G_argc + 1) * sizeof * _G_argv];
   for(size_t i = 0; i < _G_argc; ++i)
   {
      size_t slength = strlen(argv[i]) + 1;
      //std::cout << slength << std::endl;
      _G_argv[i] = new char[slength];
      memcpy(_G_argv[i], argv[i], slength);
   }
   _G_argv[_G_argc] = NULL; // Must be NULL terminated

   // Creat the command line parser
   _G_cline = new cline_utils::CommandLineParser(_G_argc, _G_argv);

   // Call potential tests
   int result = Catch::Session().run();

   // Clean up memory
   for(size_t i = 0; i < _G_argc; ++i)
   {
      delete[] _G_argv[i];
   }
   delete[] _G_argv;

   delete _G_cline;

   //std::cout << "-- END runCatchTests --" << std::endl;

   // Return to main
   return result;
}

/************************************************************************/
/*
* \brief Main function not built into Catch2...
*
*     @return int.
* 
*/
int main(int argc, char* const argv[])
{
   return runCatchTests(argc, argv);
}