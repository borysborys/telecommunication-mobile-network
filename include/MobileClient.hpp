#ifndef MOBILE_CLIENT
#define MOBILE_CLIENT

#include <NetConfAgent.hpp>

using namespace std;

namespace MobileClient_ns
{
    class MobileClient
    {
        public: 
        void setName(string &name);       
        void registerClient(string &number);
        void handleOperData(string &name);
        void handleModuleChange(map<string,string> mapOfData);
        void call(string & number);
        void answer();
        void reject();

        private:
        string _name;
        string _number;
        string _incomingNumber;
        string _state;

        string _friendNumber;
   
        unique_ptr<NetConfAgent_ns::NetConfAgent> _agent;

    };
};

#endif