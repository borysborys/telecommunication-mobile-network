#include <MobileClient.hpp>
#include <iostream>

using namespace std;

namespace
{
    const string concatXpathToState(string number)
    {
        return ("/mobile-network:core/subscribers[number=\"" + number + "\"]/state");
    };

    const string concatXpathToUserName(string number)
    {
        return ("/mobile-network:core/subscribers[number=\"" + number + "\"]/userName");
    };
    
    const string concatXpathToNumberKey(string number)
    {
        return ("/mobile-network:core/subscribers[number=\"" + number + "\"]");
    };

    const string concatXpathToIncomingNumber(string number)
    {
        return ("/mobile-network:core/subscribers[number=\"" + number + "\"]/incomingNumber");
    };
  
    vector<vector<string>> stateDiagram =
    {   //      state       |       operation       |       is allowed?                     //

        {       "idle"      ,       "call"          ,       "YES"                           },
        {       "idle"      ,       "setName"       ,       "YES"                           },
        {       "idle"      ,       "register"      ,       "client alredy registered"      },
        {       "idle"      ,       "unregister"    ,       "YES"                           },

        {       ""          ,       "call"          ,       "client not registered"         },
        {       ""          ,       "setName"       ,       "YES"                           },
        {       ""          ,       "register"      ,       "YES"                           },

        {       "busy"      ,       "call"          ,       "client being busy"             },
        {       "busy"      ,       "setName"       ,       "can't change name while busy"  },
        {       "busy"      ,       "register"      ,       "not allowed"                   },
        {       "busy"      ,       "reject"        ,       "YES"                           },

        {       "active"    ,       "call"          ,       "YES"                           },
        {       "active"    ,       "setName"       ,       "YES"                           },
        {       "active"    ,       "register"      ,       "client alredy registered"      },
        {       "active"    ,       "answer"        ,       "YES"                           },
        {       "active"    ,       "reject"        ,       "YES"                           }

    };

    bool stateCheck(string &state, const string &operationName, vector<vector<string>> stateDiagram)
    {
        for (auto key : stateDiagram)
        {
            if (key[0] == state && key[1] == operationName)
            {
                if (key[2] == "YES")
                {
                    return true;
                }
                else
                {
                    cout <<" operation not allowed : "<< key[2] << endl;
                    return false;
                }
            }
        };
        cout << " operation not allowed : " << "this operation has not been foreseen for current state" << endl;
        return false; 
    };    
};


namespace MobileClient_ns
{
    void MobileClient::setName(string &name)
    {
        if (stateCheck(_state, "setName", stateDiagram))
        {
            _name = name;
        }
    };

     void MobileClient::registerClient( string &number)
    {   
        if (stateCheck(_state, "register", stateDiagram))
        {
            _number = number;

            _state = "idle";

            _agent = make_unique<NetConfAgent_ns::NetConfAgent>();      ///TODO : put to contructor

            _agent->initSysrepo();

            _agent->changeData(concatXpathToState(_number), _state);

            _agent->registerOperData(concatXpathToUserName(_number),*this);
            
            _agent->subscribeForModelChandes(concatXpathToNumberKey(_number), *this);
        }
    };

    void MobileClient::handleOperData(string &name)
    {
        name = _name;        
    };

    void MobileClient::handleModuleChange(map<string,string> mapOfNewData)
    {              
        if (mapOfNewData.find("incomingNumber") != mapOfNewData.end())
        {
            _incomingNumber = mapOfNewData["incomingNumber"];

            if (_incomingNumber != "")
            {
                cout << _incomingNumber << " is calling you: answer/reject? "<< endl;
            }

        }
        if (mapOfNewData.find("state") != mapOfNewData.end())
        {
            _state = mapOfNewData["state"];

            if (_state == "idle")
            {
                cout << "call ended" << endl;
            }
            if (_state == "busy")
            {
                cout << "call started" << endl;
            }
        }
        if (mapOfNewData.find("number") != mapOfNewData.end())
        {
            cout << "something went wrong- your client's number has changed" << endl;
           /// _number = mapOfData["number"];
        }
    };

    void MobileClient::call(string & friendNumber)
    {
        if (stateCheck(_state, "call", stateDiagram))
        {         
            _friendNumber = friendNumber;

            string friendState;

            if ( _friendNumber == _number)
            {
                cout << "you can't call to yourself" << endl;
            }
            else if (_agent->fetchData(concatXpathToState(_friendNumber), friendState))
            {
                 cout << "calling " << _friendNumber << " ..." << endl;
                if (friendState == "idle" )
                {                   
                    _agent->changeData(concatXpathToState(_number), "active");
                    _agent->changeData(concatXpathToState(_friendNumber), "active");
                    _agent->changeData(concatXpathToIncomingNumber(_friendNumber), _number);
                }
                else
                {
                    cout << "abonent " << _friendNumber << " can't answer you at the moment" << endl;
                }                
            }
            else //can't fetch friend's data
            {
                cout << "there's no such abonent as " <<_friendNumber << endl;
            }
        }
    };

    void MobileClient::answer()
    {
        if (stateCheck(_state, "answer", stateDiagram))
        {
            _agent->changeData(concatXpathToState(_incomingNumber), "busy");
            _agent->changeData(concatXpathToState(_number), "busy");
        }

    };

    void MobileClient::reject()
    {
        if (stateCheck(_state, "reject", stateDiagram))
        {   
            //case : you are rejecting incoming call
            _agent->changeData(concatXpathToState(_incomingNumber), "idle");

            //case : you are the caller and rejecting
            _agent->changeData(concatXpathToIncomingNumber(_friendNumber), "");   /// TODO: CHANGE DATA- DELETE ITEM
            _agent->changeData(concatXpathToState(_friendNumber), "idle");  
            _agent->changeData(concatXpathToIncomingNumber(_number), "");        /// TODO: CHANGE DATA- DELETE ITEM
            
            ///in any way:
            _agent->changeData(concatXpathToState(_number), "idle");                       

        }        
    };

    void MobileClient::unregister()
    {
        if (stateCheck(_state, "unregister", stateDiagram))
        {   
            _agent->changeData(concatXpathToIncomingNumber(_number), "", "delete");
            
            _agent->changeData(concatXpathToNumberKey(_number), "", "delete");

            _agent->closeSysrepo();
            _agent.reset();
            
            _name.erase();
            _number.erase();
            _incomingNumber.erase();
            _state.erase();
            _friendNumber.erase();           

        }
    }
};