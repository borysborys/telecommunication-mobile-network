#ifndef MOBILE_CLIENT
#define MOBILE_CLIENT

#include <NetConfAgent.hpp>

using namespace std;

namespace MobileClient_ns
{
    class MobileClient
    {

        public:

        MobileClient();

        MobileClient(unique_ptr<NetConfAgent_ns::NetConfAgent>);
              
        void registerClient(const string &number);
        void handleOperData(string &name);
        void handleModuleChange(map<string,string> mapOfData);
        void call(const string & number);
        void answer();
        void reject();
        void unregister();

        void setName(string &name);  
        void setState(const string &state);
        void setIncomingNumber(const string &incomingNumber);

        string getName();
        string getState();
        string getIncomingNumber();

        private:
        string _name;
        string _number;
        string _incomingNumber;
        string _state;
        string _friendNumber;
   
        unique_ptr<NetConfAgent_ns::NetConfAgent> _agent;

    };
}

#endif