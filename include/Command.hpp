#ifndef COMMAND
#define COMMAND

#include "MobileClient.hpp"
#include <vector>
#include <iostream>


using namespace std;
using namespace MobileClient_ns;


class Command 
{   
    public:   

    explicit Command( shared_ptr<MobileClient_ns::MobileClient> ptrMobileClient = nullptr);
 
    virtual void execute() = 0;

    void setArgs(vector <string> argsVec);

    protected:    

    vector <string> _argsVec;

    shared_ptr< MobileClient> _mobClient;       

};

class registerCommand : public Command
{
    public:
    using Command::Command;
    void execute();   
};  


class nameCommand : public Command
{
    using Command::Command;
    void execute();
};

class callCommand : public Command
{
    using Command::Command;
    void execute();
};

class answerCommand : public Command
{
    using Command::Command;
    void execute();
};

class rejectCommand : public Command
{
    using Command::Command;
    void execute();
};

class unregisterCommand : public Command
{
    using Command::Command;
    void execute();
};

#endif  