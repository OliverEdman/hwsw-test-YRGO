#include "driver/gpio/interface.h"
#include "driver/gpio/stub.h"
#include "yrgo/test/test.h"

// Veriferiera att en nyskapad stubb har rätt starttillstånd.
TEST(GpioStub, InitialState)
{
    // Create GPIO instance with initial state unspecified.
    // Expect the GPIO to disabled.
    driver::gpio::Stub gpio1{};
    EXPECT_FALSE(gpio1.read());

    // Create another GPIO instance with initial state set to true.
    // Expect the GPIO to enabled.
    driver::gpio::Stub gpio2{true};
    EXPECT_TRUE(gpio2.read());

    // Create another GPIO instance with initial state set to false.
    // Expect the GPIO to disabled.
    driver::gpio::Stub gpio3{false};
    EXPECT_FALSE(gpio3.read());
}

TEST(GpioStub, write)
{
    // Skapa instans (börjar som false by default av konstruktorn)
    driver::gpio::Stub led1{};
    EXPECT_FALSE(led1.read());

    // Skriv false när den redan är false (0 -> 0)
    led1.write(false);
    EXPECT_FALSE(led1.read());

    // Skriv true (0 -> 1)
    led1.write(true);
    EXPECT_TRUE(led1.read());

    // Skriv true när den redan är true (1 -> 1)
    led1.write(true);
    EXPECT_TRUE(led1.read());

    // Skriv false (1 -> 0)
    led1.write(false);
    EXPECT_FALSE(led1.read());
}

TEST(GpioStub, toggle)
{
    // Skapar en instance bör vara false by default
    driver::gpio::Stub led1{};
    EXPECT_FALSE(led1.read());

    // Om vi togglar borde det går från (0 -> 1) då borde led1.read() vara true.
    led1.toggle();
    EXPECT_TRUE(led1.read());

    // Om vi togglar igen testar vi (1 -> 0)
    led1.toggle();
    EXPECT_FALSE(led1.read());
}
