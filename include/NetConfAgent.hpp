#ifndef NET_CONF_AGENT
#define NET_CONF_AGENT

#include "libsysrepocpp/headers/Sysrepo.hpp"
#include "libsysrepocpp/headers/Session.hpp"
#include "libsysrepocpp/headers/Connection.hpp"

using namespace std;

namespace MobileClient_ns
{
    class MobileClient;
};

namespace NetConfAgent_ns
{   
    class NetConfAgent
    {
    public:
        /*
        *@brief starts sysrepo session
        *
        *@return true if succedes, otherwise- false
        */
        bool initSysrepo();
        /*
        *@brief prints all changed in the selected YANG module to console
        *
        *@param str_module_name name of the module of interest
        * 
        *@return true if succedes, otherwise- false
        */
        bool subscribeForModelChandes(const string & strXpath, MobileClient_ns::MobileClient &mobClient);
        /*
        *@brief puts data from provided xpath into string
        *
        *@param str_xpath xpath to fetch data from
        * 
        *@param ds datastore type to fetch data from, SR_DS_RUNNING by default
        * 
        *@return true if succedes, otherwise- false
        */
        bool fetchData(const string &str_xpath, string &str_value, sr_datastore_t ds = SR_DS_RUNNING);
        /*
        *@brief changes data value in the selected xpath (config-true data only)
        *
        *@param str_xpath xpath to data to be changed
        * 
        *@param str_entry entry to be pushed for the selected xpath
        * 
        *@return true if succedes, otherwise- false
        */        
        bool changeData(const string &str_xpath, const string &str_entry);
        /*
        *@brief registers on operational data provision
        *
        *@param str_xpath path to oper data
        *
        *@param str_value value to be provided
        * 
        *@return true if succedes, otherwise- false
        */
        bool registerOperData(const string & strXpath, MobileClient_ns::MobileClient &mobClient);
        /*
        *@brief sends notification to Sysrepo. DEPENDED on mobile-network.yang
        *
        */
        bool notifySysrepo(const string &str_value1, const string &str_value2);
        /*
        *@brief subscribes and handles RPC. HARDCODED and DEPENDED mobile-network.yang
        *
        *@param str_rpc_xpath xpath to rpc of interest
        * 
        *@return true if succedes, otherwise- false
        */
        bool subscribeForRpc(const string &str_rpc_xpath, const string &str_value);

    private:

        unique_ptr<sysrepo::Connection> _conn;
        shared_ptr<sysrepo::Session> _sess;
        shared_ptr<sysrepo::Subscribe> _subscribe;

    };
}
#endif