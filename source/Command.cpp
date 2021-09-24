#include "Command.hpp"
#include <iostream>

using namespace std;

Command::Command( shared_ptr<MobileClient_ns::MobileClient> ptrMobileClient)
{
    _mobClient = ptrMobileClient;    
}

void Command::setArgs(vector <string> argsVec)
{
    _argsVec = argsVec;
}

void  registerCommand :: execute()
{        
    if ( _argsVec.size() == 1){                
    _mobClient->registerClient(_argsVec[0]);
    } 
    else {
        cout << "something went wrong =("<< endl;
    };

}

void nameCommand :: execute()
{
    if ( _argsVec.size() == 1){                
    _mobClient->setName(_argsVec[0]);
    } 
    else {
        cout << "something went wrong =("<< endl;
    };
}

void callCommand::execute()
{
    if ( _argsVec.size() == 1){                
    _mobClient->call(_argsVec[0]);
    } 
    else {
        cout << "something went wrong =("<< endl;
    };
}

void answerCommand::execute()
{
    _mobClient ->answer();
}

void rejectCommand::execute()
{
    _mobClient ->reject();
}

void unregisterCommand::execute()
{
    _mobClient ->unregister();
}