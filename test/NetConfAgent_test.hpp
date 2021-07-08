#include <gmock/gmock.h>
#include <NetConfAgent.hpp>

class NetConfAgentMock : public NetConfAgent_ns::NetConfAgent
{
    public:
    MOCK_METHOD(bool, initSysrepo, (), (override));
    MOCK_METHOD(bool, subscribeForModelChandes, (const string & strXpath, MobileClient_ns::MobileClient &mobClient), (override));
    MOCK_METHOD(bool, fetchData, (const string &str_xpath, string &str_value, sr_datastore_t ds), (override));
    MOCK_METHOD(bool, changeData, (const string &str_xpath, const string &str_entry, const string &param), (override));
    MOCK_METHOD(bool, registerOperData, (const string & strXpath, MobileClient_ns::MobileClient &mobClient), (override));
    MOCK_METHOD(bool, closeSysrepo, (), (override));

    
};
