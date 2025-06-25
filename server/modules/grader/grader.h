#pragma once

#include <string>
#include "../connection/connection.h"

class Grader{
    int fileId;
    Connection server;
public:
    Grader();
    std::string compileCommand(std::string&, std::string&);
    char *make_program_filename();
    char *make_exec_filename();
    char *make_compile_output_filename();
    char *make_runtime_output_filename();
    char *make_output_filename();
    char *make_output_diff_filename();
    char *compile_command(char *, char *);
    char *run_command(char *);
    char *output_check_command(char *);
    int delete_file(char *);
    void clean_file(char *);
    int execute(int); // main worker function
};