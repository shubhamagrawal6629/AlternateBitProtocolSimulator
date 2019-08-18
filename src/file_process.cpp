/** \brief This source file defines functions for Output File Processing.
*
* The output file generated by the simulator is processed
* to produce a more readable format. The data is stored in 
* output file *_proc.txt in a table format as shown below:
*
*Time           value  port                component
*00:00:10:000   20     out                 generator_con  
*00:00:20:000   1      packet_sent_out     sender1        
*00:00:20:000   11     data_out            sender1        
*00:00:23:000   11     out                 subnet1        
*00:00:33:000   1      out                 receiver1      
*00:00:36:000   1      out                 subnet2  
*
* Peter Bliznyuk-Kvitko
* Carleton University
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <errno.h>
#include <regex>
#include <bits/stdc++.h> 
#include <boost/algorithm/string.hpp>

#include "../include/file_process.hpp"

#define BUF_SIZE 2048

using namespace std;

void output_file_process(char *fin, char *fout) {
    /** check if the read access to the input file is OK */
    if (access(fin, R_OK)) {
        std::cout << "The file " << fin << " can not be read from, errno = "
            << errno << "\n";
    }
    else {
        /** read the input file into a string */
        char *file = read_file(fin);
        /** write the data from the input file string into output file */
        write_file(fout, file);
    }
}

char *read_file(char *fin) {
    char read_buf[BUF_SIZE];
    char *file;
    int size;
    int i = 1;
    
    int fdi = open(fin, O_RDONLY);
    
    if (fdi > 0) {
        /** read a block of data */
        while ((size = read(fdi, read_buf, BUF_SIZE)) != 0) {
            if (i == 1) {
                /** allocate memory and store the first block of data */
                file = (char *) malloc(sizeof(char) * BUF_SIZE);
                file = strncpy(file, read_buf, size);
            }
            else {
                /** reallocate memory and append the next block of data */
                file = (char *) realloc(file, sizeof(char) * BUF_SIZE * i);
                strncpy(file + BUF_SIZE * (i - 1), read_buf, size);
            }
            i++;
        }
    }
    else {
        cout << "The file " << fin 
            << " can not be opened for reading, errno = " << errno << "\n";
    }
    
    int fc = close(fdi);
    if (fc < 0) {
        cout << "The file " << fin << " can not be closed\n";
    }
    
    return(file);
}

void write_file(char *fout, char *file) {
    ofstream out_file (fout);
    char* pch = NULL;
    char* time;
    
    if (out_file.is_open()) {
        out_file << "Time\t\t\tValue\tPort\t\t\t\t\tComponent\n";

        /** split the file string on new line */
        pch = strtok(file, "\r\n");

        while (pch != NULL) {
            regex b("\\d+:\\d+:\\d+:\\d+");
            if ( regex_match(pch, b) ) { /** is the line a Time line ? */
                time = pch;
                time[strlen(time)] = '\0';
            }
            else {  /** the line is not a Time line */
                string port_str;
                string comp;
                vector<string> result;
        
                /** get the string in [] brackets and the name of component */        
                regex r("\\[(.*)\\] generated by model (.*)");
                port_str = regex_replace(pch, r, "$1"); 
                comp = regex_replace(pch, r, "$2"); 

                /** split the string in [] brackets by comma
                * and store in result vector
                */
                boost::split(result, port_str, boost::is_any_of(","));

                /** iterate through all results */        
                for (int i = 0; i < result.size(); i++) {
                    string port;
                    string value;

                    /** get the port and value from the result item */
                    regex r("(.*)\\:\\:(.*)\\: \\{(.*)\\}");
                    port = regex_replace(result[i], r, "$2"); 
                    value = regex_replace(result[i], r, "$3"); 
            
                    /** if value is not empty, output to file
                    * in specified format            
                    */
                    if (strcmp(&value[0], "") != 0) {
                        out_file << setw(15) << left << time << setw(7)
                            << value << setw(20) << port << setw(15)
                                << comp << "\n";
                    }
                }
            }
            pch = strtok(NULL, "\r\n");
        }        
    }
    else {
        cout << "The file " << fout
            << " can not be opened for writing, errno = " << errno << "\n";
    }
}
