#include <iostream>
#include <cstring>
#include <pthread.h>
#include <string>
#include "grader.h"

Grader::Grader(): server(){
    fileId = pthread_self(); // any random unique text
}

std::string Grader::compileCommand(std::string& programFile, std::string& execFile){
    char *s;
    char s1[20];

    s = (char *)malloc(200 * sizeof(char));
    memset(s, 0, sizeof(s));
    memset(s1, 0, sizeof(s1));  
    strcpy(s, "g++ -o ");
    strcat(s, execFile.c_str());
    strcat(s, "  ");
    strcat(s, programFile.c_str());
    strcat(s, " 2> logs/outputs/compiler_err");
    sprintf(s1, "%d", fileId);
    strcat(s, s1);
    return s;
}

// Function to create the run command
char *Grader::run_command(char *execFile)
{
    char *s;
    char s1[20];

    s = (char *)malloc(200 * sizeof(char));
    memset(s, 0, sizeof(s));
    memset(s1, 0, sizeof(s1));
    sprintf(s1, "%d", fileId);

    strcpy(s, "./");
    strcat(s, execFile);
    strcat(s, " > logs/outputs/out");
    strcat(s, s1);
    strcat(s, " 2> logs/outputs/runtime_err");
    strcat(s, s1);
    // printf("%s\n", s);
    return s;
}

// Function to create output check command
char *Grader::output_check_command(char *outputFile){
    char *s;
    char s1[20];

    s = (char *)malloc(200 * sizeof(char));
    memset(s, 0, sizeof(s));
    memset(s1, 0, sizeof(s1));
    sprintf(s1, "%d", fileId);

    strcpy(s, "diff -Z ");
    strcat(s, outputFile);
    strcat(s, " expected_output.txt");
    strcat(s, " > logs/outputs/output_diff");
    strcat(s, s1);
    // printf("\n%s\n",s);
    return s;
}

// Function to create program filename
char *Grader::make_program_filename()
{

    char *s;
    char s1[20];

    s = (char *)malloc(200 * sizeof(char));
    memset(s, 0, sizeof(s));
    memset(s1, 0, sizeof(s1));
    sprintf(s1, "%d", fileId);
    strcpy(s, "logs/files/file");
    strcat(s, s1);
    strcat(s, ".cpp");
    return s;
}

// Function to create the execute filename
char *Grader::make_exec_filename()
{
    char *s;
    char s1[20];

    s = (char *)malloc(200 * sizeof(char));
    memset(s, 0, sizeof(s));
    memset(s1, 0, sizeof(s1));
    sprintf(s1, "%d", fileId);
    strcpy(s, "logs/files/prog");
    strcat(s, s1);
    return s;
}

// Function to create the compile error file 
char *Grader::make_compile_output_filename()
{
    char *s;
    char s1[20];
    s = (char *)malloc(200 * sizeof(char));
    memset(s, 0, sizeof(s));
    memset(s1, 0, sizeof(s1));
    sprintf(s1, "%d", fileId);
    strcpy(s, "logs/outputs/compiler_err");
    strcat(s, s1);
    return s;
}

// Function to make the runtime error file
char *Grader::make_runtime_output_filename()
{
    char *s;
    char s1[20];
    s = (char *)malloc(200 * sizeof(char));
    memset(s, 0, sizeof(s));
    memset(s1, 0, sizeof(s1));
    sprintf(s1, "%d", fileId);
    strcpy(s, "logs/outputs/runtime_err");
    strcat(s, s1);
    return s;
}

// Function to create the output file 
char *Grader::make_output_filename()
{
    char *s;
    char s1[20];
    s = (char *)malloc(200 * sizeof(char));
    memset(s, 0, sizeof(s));
    memset(s1, 0, sizeof(s1));
    sprintf(s1, "%d", fileId);
    strcpy(s, "logs/outputs/out");
    strcat(s, s1);
    return s;
}

// Function to create the output diff file
char *Grader::make_output_diff_filename(){
    char *s;
    char s1[20];
    s = (char *)malloc(200 * sizeof(char));
    memset(s, 0, sizeof(s));
    memset(s1, 0, sizeof(s1));
    sprintf(s1, "%d", fileId);
    strcpy(s, "logs/outputs/output_diff");
    strcat(s, s1);
    return s;
}

char *Grader::compile_command(char *programFile, char *execFile)
{

    char *s;
    char s1[20];

    s = (char *)malloc(200 * sizeof(char));
    memset(s, 0, sizeof(s));
    memset(s1, 0, sizeof(s1));
    strcpy(s, "g++ -o ");
    strcat(s, execFile);
    strcat(s, "  ");
    strcat(s, programFile);
    strcat(s, " 2> logs/outputs/compiler_err");
    sprintf(s1, "%d", fileId);
    strcat(s, s1);
    // printf("%s\n", s);
    return s;
}



// char *Grader::run_command(char *execFile)
// {
//     char *s;
//     char s1[20];

//     s = (char *)malloc(200 * sizeof(char));
//     memset(s, 0, sizeof(s));
//     memset(s1, 0, sizeof(s1));
//     sprintf(s1, "%d", fileId);

//     strcpy(s, "./");
//     strcat(s, execFile);
//     strcat(s, " > logs/outputs/out");
//     strcat(s, s1);
//     strcat(s, " 2> logs/outputs/runtime_err");
//     strcat(s, s1);
//     // printf("%s\n", s);
//     return s;
// }



// char *Grader::output_check_command(char *outputFile)
// {
//     char *s;
//     char s1[20];

//     s = (char *)malloc(200 * sizeof(char));
//     memset(s, 0, sizeof(s));
//     memset(s1, 0, sizeof(s1));
//     sprintf(s1, "%d", fileId);

//     strcpy(s, "diff -Z ");
//     strcat(s, outputFile);
//     strcat(s, " expected_output.txt");
//     strcat(s, " > logs/outputs/output_diff");
//     strcat(s, s1);
//     // printf("\n%s\n",s);
//     return s;
// }



int Grader::delete_file(char *file_path){
    char command[256];
    snprintf(command, sizeof(command), "rm %s", file_path);
    return system(command);
}

void Grader::clean_file(char *file_path){
    delete_file(file_path);
    free(file_path);
}

// main function to execute the code and send the result
int Grader::execute(int clientSockFD){
    char *programFileName = make_program_filename();    // creating the file name with unique id
    try{
        server.recvFile(clientSockFD, programFileName);  // recieving the file from the client
    } catch (std::exception e) {
        free(programFileName);
        throw std::runtime_error("ERROR :: FILE RECV ERROR");
    }
    int n = server.sendData(clientSockFD, "I got your code file for grading\n", 33); //send the response back to the client
    if (n < 0)
    {
        free(programFileName);
        throw std::runtime_error("ERROR :: FILE SEND ERROR");
    }

    // creating all the necessary commands and file names
    char *execFileName = make_exec_filename();
    char *compileOutputFileName = make_compile_output_filename();
    char *runtimeOutputFileName = make_runtime_output_filename();
    char *outputFileName = make_output_filename();
    char *outputDiffFileName = make_output_diff_filename();

    char *compileCommand = compile_command(programFileName, execFileName);
    char *runCommand = run_command(execFileName);
    char *outputCheckCommand = output_check_command(outputFileName);

    // check if compile command runs successfully
    if (system(compileCommand) != 0)
    {
        n = server.sendData(clientSockFD, "COMPILER ERROR", 15); // send back response to the client
        // sleep(1);
        if (n >= 0)
        {
            n = server.sendFile(clientSockFD, compileOutputFileName); // send the compiler error to the client
        }
    }

    // chceck if execute command is running properly
    else if (system(runCommand) != 0)
    {
        n = server.sendData(clientSockFD, "RUNTIME ERROR", 14);  // send back the response to the client
        if (n >= 0)
            n = server.sendFile(clientSockFD, runtimeOutputFileName); // send the runtime error to the client
    }
    else
    {
        //check the output matches with the expected output or not
        if (system(outputCheckCommand) != 0)
        {
            n = server.sendData(clientSockFD, "OUTPUT ERROR", 14);   // send respond to the client MSG_NOSIGNAL flag fixes the broken pipe error
            if (n >= 0)
                n = server.sendFile(clientSockFD, outputDiffFileName);    // send the difference to the client
        }
        else
        {
            n = server.sendData(clientSockFD, "PROGRAM RAN", 12);    // send the client about successful executioon
        }
    }

    // free all the variables used
    free(programFileName);
    free(execFileName);
    free(compileOutputFileName);
    free(runtimeOutputFileName);
    free(outputFileName);
    free(outputDiffFileName);
    free(compileCommand);
    free(runCommand);
    free(outputCheckCommand);

    if (n < 0)
        throw std::runtime_error("ERROR :: FILE SEND ERROR");
    return 0;
}