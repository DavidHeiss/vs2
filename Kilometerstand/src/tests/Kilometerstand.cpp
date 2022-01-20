#include "gtest/gtest.h"
#include "../Kilometerstand.h"

class KilometerstandTest : public testing::Test
{
protected:
    void SetUp() override
    {
        kilometerstand = Sensor::Kilometerstand();
    }
    Sensor::Kilometerstand kilometerstand;
};

TEST_F(KilometerstandTest, has_data)
{
    auto message = kilometerstand.generate_message(100);

    ASSERT_TRUE(message.has_data());
}

TEST_F(KilometerstandTest, has_from_address)
{
    auto message = kilometerstand.generate_message(100);

    ASSERT_FALSE(message.has_from_address());
}

TEST_F(KilometerstandTest, has_from_port)
{
    auto message = kilometerstand.generate_message(100);

    ASSERT_FALSE(message.has_from_port());
}

TEST_F(KilometerstandTest, has_latency)
{
    auto message = kilometerstand.generate_message(100);

    ASSERT_FALSE(message.has_latency());
}

TEST_F(KilometerstandTest, has_received)
{
    auto message = kilometerstand.generate_message(100);

    ASSERT_FALSE(message.has_received());
}

TEST_F(KilometerstandTest, has_round_trip)
{
    auto message = kilometerstand.generate_message(100);

    ASSERT_FALSE(message.has_round_trip());
}

TEST_F(KilometerstandTest, has_send)
{
    auto message = kilometerstand.generate_message(100);

    ASSERT_TRUE(message.has_send());
}

TEST_F(KilometerstandTest, has_to_address)
{
    auto message = kilometerstand.generate_message(100);

    ASSERT_FALSE(message.has_to_address());
}

TEST_F(KilometerstandTest, has_to_port)
{
    auto message = kilometerstand.generate_message(100);

    ASSERT_FALSE(message.has_to_port());
}

TEST_F(KilometerstandTest, has_mileage)
{
    auto data = kilometerstand.generate_message(100).data();

    ASSERT_TRUE(data.has_mileage());
}

TEST_F(KilometerstandTest, has_type)
{
    auto data = kilometerstand.generate_message(100).data();

    ASSERT_TRUE(data.has_type());
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
