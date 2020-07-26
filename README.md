# C++ Web Server from scratch.

This project is a Static Web server which serves files and is written from scratch. It is written in C++ and uses CMake for managing dependencies.
### Building for source
Clone the repository on your local system and open the root project directory in your favourite terminal.
Create a new folder to output MakeFile and executable.
```sh
$ mkdir build
$ cd build
```
Make sure your system has CMake installed. Type the following command to generate necessary Makefiles.
```sh
$ cmake ..
```
To compile the source and generate executable, type the following command.
```sh
$ make
```
A File named 'web_server' will be created inside build folder. All the files to be served must be stored in a folder named 'serverroot' in the same directory as executable.

### Todos

 - Fix Segmentation Fault Issue.
 - Write Tests
  
License
----

MIT
