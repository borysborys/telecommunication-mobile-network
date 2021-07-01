#include "gtest/gtest.h"
#include "NetConfAgent_test.hpp"


class MobileClient_test : public testing::Test
{
    protected:
    void SetUp() override
    {
        _mobileClient = std::make_unique<MobileClient_ns::MobileClient>(nullptr);
    }
    protected:
    std::unique_ptr<MobileClient_ns::MobileClient> _mobileClient;
    //testing :: StrictMock<mock::MyMock> _mock;
};

TEST_F(MobileClient_test, shouldSuccedToDoSomething)
{
    string newname = "zuzu";
    _mobileClient->setName(newname);

    EXPECT_EQ(_mobileClient->getName(), newname);

}

TEST_F(MobileClient_test, shouldSuccedToDoSomethingElse)
{
    string newname = "dodo";
    _mobileClient->setName(newname);

    EXPECT_EQ(_mobileClient->getName(), newname);

}

