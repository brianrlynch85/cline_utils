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
validate_required_options(...) - Missing required option in command line args: -a

------------------------------------------------------
General Usage: bin/example_main -option_character <option_argument>
------------------------------------------------------
------------------------------------------------------
Option & Argument Table
------------------------------------------------------
+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|        Option Long Name |   Option Character |      Argument Type |    Option Required |  Argument Required |                                              Description [Physical Units] |
+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|     name_for_parameter1D|                   a|             double |                   1|                   1|             Required option with required double argument [physical units]|
|     name_for_parameter2D|                   b|             double |                   1|                   1|                           Required option with required double argument []|
|     name_for_parameter3I|                   c|                int |                   0|                   1|                   Optional option with a required integer arugment if used|
|     name_for_parameter4S|                   d|             char * |                   1|                   1|                              Required option with required string argument|
+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
```

<!-- ROADMAP -->
## Roadmap

See `TODO.md` for more information.

<!-- LICENSE -->
## License

See `LICENSE.txt` for more information.