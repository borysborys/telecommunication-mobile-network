#include "gtest/gtest.h"
#include "NetConfAgent_test.hpp"
#include "MobileClient.hpp"

using ::testing::StrictMock;
using ::testing::Return;
using ::testing :: _;
using ::testing :: DoAll;
using ::testing :: SetArgReferee;

namespace 
{
    const string MYNUMBER = "101";
}

class MobileClient_test : public testing::Test
{
    protected:
    void SetUp() override
    {
        auto tempMock = std::make_unique<testing::StrictMock<NetConfAgentMock>>();
        _mock = tempMock.get();
        _mobileClient = std::make_unique<MobileClient_ns::MobileClient>(std::move(tempMock));
    }

    void registerClient()
    {
        EXPECT_CALL(*_mock, initSysrepo())
        .WillOnce(Return(true));

        EXPECT_CALL(*_mock, changeData(_,_,_))
        .WillOnce(Return(true));

        EXPECT_CALL(*_mock, registerOperData(_,_))
        .WillOnce(Return(true));

        EXPECT_CALL(*_mock, subscribeForModelChandes(_,_))
        .WillOnce(Return(true));

        _mobileClient->registerClient(MYNUMBER);
    }

    protected:
    std::unique_ptr<MobileClient_ns::MobileClient> _mobileClient;
    testing :: StrictMock<NetConfAgentMock> *_mock;

};

TEST_F(MobileClient_test, shouldSuccedToChangeName)
{
    string newname = "zuzu";
    _mobileClient->setName(newname);

    EXPECT_EQ(_mobileClient->getName(), newname);

}

TEST_F(MobileClient_test, shouldSuccedToRegister)
{
    EXPECT_CALL(*_mock, initSysrepo())
        .WillOnce(Return(true));

    EXPECT_CALL(*_mock, changeData(_,_,_))
        .WillOnce(Return(true));
    
    EXPECT_CALL(*_mock, registerOperData(_,_))
        .WillOnce(Return(true));

    EXPECT_CALL(*_mock, subscribeForModelChandes(_,_))
        .WillOnce(Return(true));

    _mobileClient->registerClient(MYNUMBER);

}


TEST_F(MobileClient_test, shouldSuccedToCall)
{

    registerClient();

    const string freindxpath = "/mobile-network:core/subscribers[number=\"102\"]/state";
    string freindstate = "idle";    

    EXPECT_CALL(*_mock, fetchData(freindxpath,_,_))
        .WillOnce(DoAll(SetArgReferee<1>(freindstate), Return(true)));
    

    EXPECT_CALL(*_mock, changeData(_,_,_))
        .WillRepeatedly(Return(true));
            
    const string friendnumber = "102"; 
    const string state = "idle";
    _mobileClient->setState(state);
    _mobileClient->call(friendnumber);

}

TEST_F(MobileClient_test, shouldSuccedToHandleModuleChange)
{

    registerClient();

    map<string,string> myMap =
    {
        {"incomingNumber", "101"},
        {"state", "active"},
    };

    _mobileClient->handleModuleChange(myMap);

    EXPECT_EQ(_mobileClient->getState(), "active");

}

TEST_F(MobileClient_test, RegisterUnregisterRegister)
{
    registerClient();

    EXPECT_CALL(*_mock, changeData("/mobile-network:core/subscribers[number=\"101\"]/incomingNumber","","delete"))
        .WillOnce(Return(true));

    EXPECT_CALL(*_mock, changeData("/mobile-network:core/subscribers[number=\"101\"]","","delete"))
        .WillOnce(Return(true));

    EXPECT_CALL(*_mock, closeSysrepo())
        .WillOnce(Return(true));

    _mobileClient->unregister();
    
    registerClient();

}

TEST_F(MobileClient_test, shouldSuccedToAnswer)     
{
    registerClient();
    _mobileClient->setState("active");
    _mobileClient->setIncomingNumber("102");

    const string myxpathIncomingNumber = "/mobile-network:core/subscribers[number=\"101\"]/incomingNumber";
    EXPECT_CALL(*_mock, fetchData(myxpathIncomingNumber,_,_))
        .WillOnce(Return(true));

    const string freindxpathState = "/mobile-network:core/subscribers[number=\"102\"]/state";
    EXPECT_CALL(*_mock, changeData(freindxpathState,"busy",_))
        .WillOnce(Return(true));

    const string myxpathState = "/mobile-network:core/subscribers[number=\"101\"]/state";
    EXPECT_CALL(*_mock, changeData(myxpathState,"busy",_))
        .WillOnce(Return(true));
    
    _mobileClient->answer();
}

TEST_F(MobileClient_test, shouldSuccedToReject)    
{
    registerClient();

    _mobileClient->setState("active");
    _mobileClient->setIncomingNumber("102");

    const string freindxpathState = "/mobile-network:core/subscribers[number=\"102\"]/state";
    EXPECT_CALL(*_mock, changeData(freindxpathState,"idle",_))
        .WillOnce(Return(true));

    const string freindxpathIncomingNumber = "/mobile-network:core/subscribers[number=\"\"]/incomingNumber";
    EXPECT_CALL(*_mock, changeData(freindxpathIncomingNumber,"","delete"))
        .WillOnce(Return(true));

    const string myxpathIncomingNumber = "/mobile-network:core/subscribers[number=\"101\"]/incomingNumber";
    EXPECT_CALL(*_mock, changeData(myxpathIncomingNumber,"","delete"))
        .WillOnce(Return(true));

    const string myxpathState = "/mobile-network:core/subscribers[number=\"101\"]/state";
    EXPECT_CALL(*_mock, changeData(myxpathState,"idle",_));
    
    _mobileClient->reject();
}





