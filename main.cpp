// $Id: main.cpp,v 1.9 2016/07/20 20:07:26 akhatri Exp $
//Marcos Chabolla
//mchaboll@ucsc.edu (1437530)
//Partner: Marcos Chabolla

#include <cstdlib>
#include <exception>
#include <iostream>
#include <iomanip>
#include <regex>
#include <cassert>
#include <fstream>
#include <string>
#include <unistd.h>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

void optionalize(string line, str_str_map& test){
    regex comment_regex {R"(^\s*(#.*)?$)"};
    regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
    regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
    smatch result;
    if(regex_search (line, result, comment_regex)){
        cout << "Comment or empty line." << endl;
        return;
    }
    if(regex_search (line, result, key_value_regex)){
        
        if(result[1]!="" and result[2]==""){
            //Search through list and delete
            //key/value pair
            str_str_pair newPair(result[1],result[2]);
            auto it2 = test.find(newPair.first);
            if(it2 == test.end()){
                cout<<"Item to be deleted does not exist!"<<endl;
                return;
            }
            test.erase(it2);
            return;
        }
        if(result[1]=="" and result[2]==""){
            //Iterate through list and print.
            if(test.begin()==test.end()){
                return;
            }
            for (str_str_map::iterator itor = 
            test.begin();itor != test.end(); ++itor){
            cout << itor->first<<" = "<<itor->second 
            << endl;
            }
            return;
        }
        if(result[1]=="" and result[2]!=""){
            //Print all key/value pairs w/given value
            for(str_str_map::iterator itor = test.begin();
                itor!=test.end();++itor){
                    if(itor->second == result[2]){
                        cout<<itor->first<<" = "<<itor->
                        second<<endl;
                }
            }
            return;
        }
        if(result[1]!="" and result[2]!=""){
            //Insert key/value into map
            cout << result[1] << " = "<<result[2]<<endl;
            str_str_pair newPair(result[1],result[2]);
            test.insert(newPair);
            return;
        }
    }
    else if(regex_search (line, result, trimmed_regex)){
        //Search for key  and corresponding value
        str_str_map::iterator itor = test.find(result[1]);
        if(itor==test.end()){
            cout<<result[1]<<": not found!"<<endl;
            return;
        }
        if(itor->first == result[1]){
            cout<<itor->first<<" = "<<itor->second<<endl;
            return;
        }
        else if(itor->first!=result[1]){
            cout<<result[1]<<": not found!"<<endl;
            return;
        }
    }
    else{
        assert (false and "This can not happen.");
        return;
    }
}
void readInput(int argc,char**argv){
    
    string line="";
    string cinOpt = "-";
    int lineNum=0;
    str_str_map test;
    
    //If there are no arguments, read from Cin
    if(argc==1){
        while(1){
            line="";
            lineNum++;
            getline(cin,line);
            cout<<"-: "<<lineNum<<": "<<line<<endl;
            optionalize(line,test);
        }
    }
    //Take care of arguments case
    for(int i=1;i<argc;i++){
        lineNum=0;
        //Read from cin
        if(argv[i]==cinOpt){
            while(1){
                line="";
                lineNum++;
                getline(cin,line);
                cout<<"-: "<<lineNum<<": "<<line<<endl;
                optionalize(line,test);
            }
        }
        else{
            //Open current file
            ifstream inputFile(argv[i]);
            if(inputFile.is_open()){
                
                while(getline(inputFile,line)){
                    lineNum++;
                    cout<<argv[i]<<":"<<" "<<lineNum<<":"<<" "<<line
                    <<endl;
                    optionalize(line,test);
                }
                inputFile.close();
                cout<<endl<<endl;
            }
            else{
                cout<<"Could not open "<<argv[i]<<endl;
                cout<<endl<<endl;
            }
        }
    }
}

int main (int argc, char** argv) 
{
   sys_info::set_execname (argv[0]);
   scan_options (argc, argv);
   readInput(argc,argv);
   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}

