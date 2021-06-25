#include "Invoker.hpp"
#include <sstream>
#include <iostream> 

using namespace std;


namespace
{
    map <string, int> mapTokenArg = 
    {   //    token         | amount of args //         
        {   "register"      ,       1       },
        {   "name"          ,       1       },
        {   "call"          ,       1       },
        {   "answer"        ,       0       },
        {   "reject"        ,       0       },
        {   "exit"          ,       0       }
    };

    shared_ptr<MobileClient_ns::MobileClient> ptr (new MobileClient_ns::MobileClient); /////TODO: MOVE TO CONSTRUCTOR

    map <string,  Command*> mapTokenCommand =
    {   //    token         |       actual command              //  
        {   "register"      ,       new registerCommand(ptr)    },
        {   "name"          ,       new nameCommand(ptr)        },
        {   "call"          ,       new callCommand(ptr)        },
        {   "answer"        ,       new answerCommand(ptr)      },
        {   "reject"        ,       new rejectCommand(ptr)      }
    };

    void parseInput(vector<string> &input)
    {
        string inputString;
        getline(cin, inputString);

        stringstream inputSstream(inputString);
        string temp;
        vector <string> instructions;
        while(getline(inputSstream, temp, ' '))
        {
            if (!temp.empty())
            {
                instructions.push_back(temp);
            }
        }

        input = instructions;
    };

    bool validateInput(vector<string> &input, map <string, int> mapTokens)
    {
        if (mapTokens.find(input[0]) == mapTokens.end()) ///check if first phrase is a token
        {
            cout <<"commands validate ERR : "<<input[0] <<" is not a command" << endl;
            return false;
        }else 
        {
            if ( input.size() != mapTokens[input[0]] + 1 ) /// check if there's the right amont of args
            {
                cout << " commands validate ERR : bad arguments amount" << endl;
                return false; 
            }

            for (size_t i = 1; (i <= mapTokens[input[0]] + 1)&&(i <= input.size() - 1); i++)
            {
                if (mapTokens.find(input[i]) != mapTokens.end()) ///check if arg is not a command name
                    {
                        cout <<"commands validate ERR : "<<input[i] <<" is a command name passed as an argument" << endl;
                        return false;
                    }
            }        

        }
        return true;
        
    };
};      //namespace

void Invoker::interpetCmdInput(){
      
    while(true)
    {
        vector <string> input;
        parseInput(input);
        if (validateInput(input, mapTokenArg))
        {   
            if (input[0] == "exit")
            {
                break;
            };

            vector <string> argsVec = {};

            if (input.size()>1)
            {
                argsVec.push_back(input[1]); /////////////////remove hardcodre for only one arg
            }  
            
            mapTokenCommand[input[0]]->setArgs(argsVec);

            mapTokenCommand[input[0]]->execute();

        }

    }
};

