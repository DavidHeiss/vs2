#include "gtest/gtest.h"
#include "../Fuellstand.h"

class FuellstandTest : public testing::Test
{
protected:
    void SetUp() override
    {
        fuellstand = Sensor::Fuellstand();
    }
    Sensor::Fuellstand fuellstand;
};

TEST_F(FuellstandTest, has_data)
{
    auto message = fuellstand.generate_message(100);

    ASSERT_TRUE(message.has_data());
}

TEST_F(FuellstandTest, has_from_address)
{
    auto message = fuellstand.generate_message(100);

    ASSERT_FALSE(message.has_from_address());
}

TEST_F(FuellstandTest, has_from_port)
{
    auto message = fuellstand.generate_message(100);

    ASSERT_FALSE(message.has_from_port());
}

TEST_F(FuellstandTest, has_latency)
{
    auto message = fuellstand.generate_message(100);

    ASSERT_FALSE(message.has_latency());
}

TEST_F(FuellstandTest, has_received)
{
    auto message = fuellstand.generate_message(100);

    ASSERT_FALSE(message.has_received());
}

TEST_F(FuellstandTest, has_round_trip)
{
    auto message = fuellstand.generate_message(100);

    ASSERT_FALSE(message.has_round_trip());
}

TEST_F(FuellstandTest, has_send)
{
    auto message = fuellstand.generate_message(100);

    ASSERT_TRUE(message.has_send());
}

TEST_F(FuellstandTest, has_to_address)
{
    auto message = fuellstand.generate_message(100);

    ASSERT_FALSE(message.has_to_address());
}

TEST_F(FuellstandTest, has_to_port)
{
    auto message = fuellstand.generate_message(100);

    ASSERT_FALSE(message.has_to_port());
}

TEST_F(FuellstandTest, has_capacity)
{
    auto data = fuellstand.generate_message(100).data();

    ASSERT_TRUE(data.has_capacity());
}

TEST_F(FuellstandTest, has_consumption)
{
    auto data = fuellstand.generate_message(100).data();

    ASSERT_TRUE(data.has_consumption());
}

TEST_F(FuellstandTest, has_current_capacity)
{
    auto data = fuellstand.generate_message(100).data();

    ASSERT_TRUE(data.has_current_capacity());
}

TEST_F(FuellstandTest, has_last_refueld)
{
    auto data = fuellstand.generate_message(100).data();

    ASSERT_TRUE(data.has_last_refueld());
}

TEST_F(FuellstandTest, has_level)
{
    auto data = fuellstand.generate_message(100).data();

    ASSERT_TRUE(data.has_level());
}

TEST_F(FuellstandTest, has_type)
{
    auto data = fuellstand.generate_message(100).data();

    ASSERT_TRUE(data.has_type());
}

TEST_F(FuellstandTest, has_ussage)
{
    auto data = fuellstand.generate_message(100).data();

    ASSERT_TRUE(data.has_ussage());
}

TEST_F(FuellstandTest, has_will_refill)
{
    auto data = fuellstand.generate_message(100).data();

    ASSERT_TRUE(data.has_will_refill());
}


int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}