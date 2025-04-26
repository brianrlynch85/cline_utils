<!-- ABOUT THE PROJECT -->
## About The Project

A simple and buggy command line options parser among a sea of better alternatives.

<!-- GETTING STARTED -->
## Getting Started

```console
foo@bar:~$ mkdir build
foo@bar:~$ cd build
foo@bar:~$ cmake ../
-- The CXX compiler identification is GNU 11.4.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- The C compiler identification is GNU 11.4.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/brian/projects/plasmas/ext/cline_utils/build
foo@bar:~$ make
Consolidate compiler generated dependencies of target bprinter
[ 33%] Built target bprinter
[ 50%] Building CXX object source/CMakeFiles/example_main.dir/example_main.cpp.o ccon
[ 66%] Linking CXX executable ../bin/example_main
[ 66%] Built target example_main
Consolidate compiler generated dependencies of target bprinterTest
[100%] Built target bprinterTest
```

<!-- USAGE EXAMPLES -->
## Usage

```console
foo@bar:~$ bin/example_driver
-- BEGIN example_main --
******************************************************************************************
check_required_options(...) - Missing required option in command line args: -a
******************************************************************************************

--------------------------------------------------------------
foo@bar:~$ bin/example_main -option_character <argument_value>
--------------------------------------------------------------
+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|        Option Long Name |   Option Character |      Argument Type |    Option Required |  Argument Required |       Argument Default Value |                                                               Description |
+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                     help|                   h|                int |                   0|                   0|                             0|                                        Optional help option that must be h|
|                longName1|                   a|             double |                   1|                   1|                           nan|             Required option with required double argument [physical units]|
|                longName2|                   b|             double |                   1|                   1|3.140000000000000124344978758*|                           Required option with required double argument []|
|                longName3|                   c|                int |                   0|                   1|                           100|                   Optional option with a required integer arugment if used|
|                longName4|                   d|             char * |                   1|                   1|                              |                              Required option with required string argument|
+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
-- END example_main --
```

```console
foo@bar:~$ bin/example_main -b 4 --longName1=5 -c 3 -d 'hello'
-- BEGIN example_main --
+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|        Option Long Name |   Option Character |      Argument Type |    Option Required |  Argument Required |       Current Argument Value |                                                               Description |
+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                     help|                   h|                int |                   0|                   0|                             0|                                        Optional help option that must be h|
|                longName1|                   a|             double |                   1|                   1|5.000000000000000000000000000*|             Required option with required double argument [physical units]|
|                longName2|                   b|             double |                   1|                   1|4.000000000000000000000000000*|                           Required option with required double argument []|
|                longName3|                   c|                int |                   0|                   1|                             3|                   Optional option with a required integer arugment if used|
|                longName4|                   d|             char * |                   1|                   1|                         hello|                              Required option with required string argument|
+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
-- END example_main --
```

<!-- ROADMAP -->
## Roadmap

See `TODO.md` for more information.

<!-- LICENSE -->
## License

See `LICENSE.txt` for more information.