#include "driver/timer/interface.h"
#include "driver/timer/stub.h"
#include "yrgo/test/test.h"


/* En funktion som tar in timer klassen som ingående parameter samt en 32 bitars heltal
   vi kör en for loop lika många gånger som vi skickade in som parameter (count) varje
   vid varje varv så anropar vi tick() som ligger i klassen timer som vi når via punktoperatorn 
   tick ökar en variabel med 1 så vi simulerar en timer här.

   Jag skriver ++i istället för i++ för i c++ skapas en trillfälig kopia av det gamal värdet
   Men med i++ ökar värdet direkut utan koperia något vilker sparar kraft
*/

void ticktimer(driver::timer::Interface& timer, const std::uint32_t count) noexcept 
{
    for (std::uint32_t i = 0U; i < count; ++i)
    {
        timer.tick();
    }
}

TEST(TimerStub, InitialState)
{
    // Sätter timeout_ms till 100 men bara här i test funktionen
    constexpr std::uint16_t timeout_ms{100U};

    /*
        Skapar ett objekt från klassen Stub som vi döper till timer och skicakr med timeout_ms
        som parameter som skickas till konstuktorn som vill ha en timeout_ms som ingående parameter
    */ 
    driver::timer::Stub timer{timeout_ms};

    // Här testar jag om timeout_ms() retunerar mitt värde jag skickar in i timeout_ms
    EXPECT_EQ(timer.timeout_ms(), timeout_ms);
    
    // Här testar jag om default argumentet const bool start = false i konstruktorn funkar.
    EXPECT_FALSE(timer.isRunning());

    // HasTimedOut bör retunera false eftersom timern inte är igång
    EXPECT_FALSE(timer.hasTimedOut());

}

TEST (TimerStub, StartAndStop)
{   
    // Skapar ett objekt så vi kan köra tester med skriver 100U för något måste den ha för funka.
    driver::timer::Stub timer {100U};
    EXPECT_FALSE(timer.isRunning());

    timer.start();
    EXPECT_TRUE(timer.isRunning());

    timer.stop();
    EXPECT_FALSE(timer.isRunning());

     // Extra kontroll starta flera gånger så den inte råkar funka bnara en gång.
    timer.start();
    EXPECT_TRUE(timer.isRunning());
}

TEST(TimerStub, Toggle)
{
    // Skapa en stoppad timer
    driver::timer::Stub timer{100U};
    EXPECT_FALSE(timer.isRunning());

    // Första togglingen ska starta timern
    timer.toggle();
    EXPECT_TRUE(timer.isRunning());

    // Andra togglingen ska stoppa den igen
    timer.toggle();
    EXPECT_FALSE(timer.isRunning());

    // Tredje togglingen ska starta den igen
    timer.toggle();
    EXPECT_TRUE(timer.isRunning());
}

// En annan lösning för att testa toggle funktionen.
TEST(TimerStub, ToggleLoop)
{
    constexpr unsigned iterationCount{5U};

    // Create a stopped timer.
    driver::timer::Stub timer{100U};

    // Create a variable holding the expected running state.
    bool expectedState{timer.isRunning()};

    // Toggle the timer five times, expect the running state to be flipped each time.
    for (unsigned i{}; i < iterationCount; ++i)
    {
        timer.toggle();
        expectedState = !expectedState;
        EXPECT_EQ(timer.isRunning(), expectedState);
    }
}

/* Om timern är stoppad av stop() så ska det inte spela någon roll hur nmänga gånger vi 
anropar tick(), räknaren ska inte öka om inte timer är igång
*/

TEST(TimerStub, TickWhileStopped)
{
    driver::timer::Stub timer{5U}; // 5 ms timeout

    // Använder ticktimer funktionen  med ett fast antal tick (10 st)
    ticktimer(timer, 10U);

    // Timern ska fortfarande vara stoppad eftersom vi inte kört start().
    EXPECT_FALSE(timer.isRunning());
    EXPECT_FALSE(timer.hasTimedOut());
}

TEST(TimerStub, NoTimeoutBeforeConfiguredTicks)
{
    constexpr std::uint16_t timeout_ms{10U};

    // Skapa en startad timer
    driver::timer::Stub timer{timeout_ms, true};

    // Ticka 2 steg mindre än timeouten (8 gånger)
    ticktimer(timer, timeout_ms - 2U);
    
    // bör retunera false eftersom vi inte nåt 10.
    EXPECT_FALSE(timer.hasTimedOut());
}

TEST(TimerStub, TimeoutAndReset)
{
    constexpr std::uint16_t timeout_ms{5U};

    // Skapa en timer
    driver::timer::Stub timer{timeout_ms, true};

    // Ticka tills vi når timeouten
    ticktimer(timer, timeout_ms);

    // Första anropet ska ge true och nollställa räknaren
    EXPECT_TRUE(timer.hasTimedOut());

    // Andra omedelbara anropet ska ge false eftersom räknaren är nollställd
    EXPECT_FALSE(timer.hasTimedOut());

    // Ticka upp till timeouten en gång till
    ticktimer(timer, timeout_ms);

    // Nu ska den ha timat ut igen
    EXPECT_TRUE(timer.hasTimedOut());
}

TEST(TimerStub, StopFreezesCounterWithoutResetting)
{
    constexpr std::uint16_t timeout_ms{10U};

    // Skapa en timer
    driver::timer::Stub timer{timeout_ms, true};

    // Ticka 4 steg
    ticktimer(timer, 4U);

    // Stoppa timern räknaren ska frysas (inte nollställas)
    timer.stop();
    EXPECT_FALSE(timer.isRunning());

    // Om vi tickar nu när den är stoppad ska räknaren INTE öka
    ticktimer(timer, 3U);

    // Starta timern igen detta ska nollställa räknaren
    timer.start();
    EXPECT_TRUE(timer.isRunning());

    /* Ticka upp till timeouten igen för att säkerställa att den startade om från noll
    kräver 10 tick till, inte bara 6) */
    ticktimer(timer, timeout_ms);
    EXPECT_TRUE(timer.hasTimedOut());
}