#include "gtest/gtest.h"
#include "../Verkehrssituation.h"

class VerkehrssituationTest : public testing::Test {
protected:
    void SetUp() override {
        verkehrssituation = Sensor::Verkehrssitation();
    }
    Sensor::Verkehrssitation verkehrssituation;
};

TEST_F(VerkehrssituationTest , has_data)
{
    auto message = verkehrssituation.generateMessage();

    ASSERT_TRUE(message.has_data());
}

TEST_F(VerkehrssituationTest, has_from_address)
{
    auto message = verkehrssituation.generateMessage();

    ASSERT_FALSE(message.has_from_address());
}

TEST_F(VerkehrssituationTest, has_from_port)
{
    auto message = verkehrssituation.generateMessage();

    ASSERT_FALSE(message.has_from_port());
}

TEST_F(VerkehrssituationTest, has_latency)
{
    auto message = verkehrssituation.generateMessage();

    ASSERT_FALSE(message.has_latency());
}

TEST_F(VerkehrssituationTest, has_received)
{
    auto message = verkehrssituation.generateMessage();

    ASSERT_FALSE(message.has_received());
}

TEST_F(VerkehrssituationTest, has_round_trip)
{
    auto message = verkehrssituation.generateMessage();

    ASSERT_FALSE(message.has_round_trip());
}

TEST_F(VerkehrssituationTest, has_send)
{
    auto message = verkehrssituation.generateMessage();

    ASSERT_TRUE(message.has_send());
}

TEST_F(VerkehrssituationTest, has_to_address)
{
    auto message = verkehrssituation.generateMessage();

    ASSERT_FALSE(message.has_to_address());
}

TEST_F(VerkehrssituationTest, has_to_port)
{
    auto message = verkehrssituation.generateMessage();

    ASSERT_FALSE(message.has_to_port());
}

TEST_F(VerkehrssituationTest, has_numeric_value)
{
    auto data = verkehrssituation.generateMessage().data();

    ASSERT_TRUE(data.has_numeric_value());
}

TEST_F(VerkehrssituationTest, has_rising)
{
    auto data = verkehrssituation.generateMessage().data();

    ASSERT_TRUE(data.has_rising());
}

TEST_F(VerkehrssituationTest, has_type)
{
    auto data = verkehrssituation.generateMessage().data();

    ASSERT_TRUE(data.has_type());
}

TEST_F(VerkehrssituationTest, has_value)
{
    auto data = verkehrssituation.generateMessage().data();

    ASSERT_TRUE(data.has_value());
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}