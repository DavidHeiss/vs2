#include "gtest/gtest.h"
#include "../Durchschnittsgeschwindigkeit.h"

class DurchschnittsgeschwindigkeitTest : public testing::Test
{
protected:
    void SetUp() override
    {
        durchschnittsgeschwindigkeit = Sensor::Durchschnittsgeschwindigkeit();
    }
    Sensor::Durchschnittsgeschwindigkeit durchschnittsgeschwindigkeit;
};

TEST_F(DurchschnittsgeschwindigkeitTest, has_data)
{
    auto message = durchschnittsgeschwindigkeit.generate_message();

    ASSERT_TRUE(message.has_data());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_from_address)
{
    auto message = durchschnittsgeschwindigkeit.generate_message();

    ASSERT_FALSE(message.has_from_address());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_from_port)
{
    auto message = durchschnittsgeschwindigkeit.generate_message();

    ASSERT_FALSE(message.has_from_port());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_latency)
{
    auto message = durchschnittsgeschwindigkeit.generate_message();

    ASSERT_FALSE(message.has_latency());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_received)
{
    auto message = durchschnittsgeschwindigkeit.generate_message();

    ASSERT_FALSE(message.has_received());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_round_trip)
{
    auto message = durchschnittsgeschwindigkeit.generate_message();

    ASSERT_FALSE(message.has_round_trip());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_send)
{
    auto message = durchschnittsgeschwindigkeit.generate_message();

    ASSERT_TRUE(message.has_send());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_to_address)
{
    auto message = durchschnittsgeschwindigkeit.generate_message();

    ASSERT_FALSE(message.has_to_address());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_to_port)
{
    auto message = durchschnittsgeschwindigkeit.generate_message();

    ASSERT_FALSE(message.has_to_port());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_accelerate)
{
    auto data = durchschnittsgeschwindigkeit.generate_message().data();

    ASSERT_TRUE(data.has_accelerate());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_acceleration)
{
    auto data = durchschnittsgeschwindigkeit.generate_message().data();

    ASSERT_TRUE(data.has_acceleration());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_average_speed)
{
    auto data = durchschnittsgeschwindigkeit.generate_message().data();

    ASSERT_TRUE(data.has_average_speed());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_breaking_force)
{
    auto data = durchschnittsgeschwindigkeit.generate_message().data();

    ASSERT_TRUE(data.has_breaking_force());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_distance)
{
    auto data = durchschnittsgeschwindigkeit.generate_message().data();

    ASSERT_TRUE(data.has_distance());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_max_speed)
{
    auto data = durchschnittsgeschwindigkeit.generate_message().data();

    ASSERT_TRUE(data.has_max_speed());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_min_speed)
{
    auto data = durchschnittsgeschwindigkeit.generate_message().data();

    ASSERT_TRUE(data.has_min_speed());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_speed)
{
    auto data = durchschnittsgeschwindigkeit.generate_message().data();

    ASSERT_TRUE(data.has_speed());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_start_time)
{
    auto data = durchschnittsgeschwindigkeit.generate_message().data();

    ASSERT_TRUE(data.has_start_time());
}

TEST_F(DurchschnittsgeschwindigkeitTest, has_type)
{
    auto data = durchschnittsgeschwindigkeit.generate_message().data();

    ASSERT_TRUE(data.has_type());
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}