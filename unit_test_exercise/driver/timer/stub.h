#pragma once

#include <cstdint>
#include <cstdio>
#include <exception>

#include "driver/timer/interface.h"

namespace driver::timer 
{

class Stub final : public Interface
{
public:
    // Konstruktor
    explicit Stub(const std::uint16_t timeout_ms, const bool start = false) noexcept 
        : myTimeout_ms{timeout_ms}
        , myCounter{0U}
        , myRunning{start}
    {
        if (0U == timeout_ms)
        {
            std::fprintf(stderr, "Cannot create timer without a timeout!\n");
            std::terminate();
        }
    }

    // Destruktor
    ~Stub() noexcept override = default;

    // Radera default, kopiering och flytt
    Stub()                       = delete;
    Stub(const Stub&)            = delete;
    Stub(Stub&&)                 = delete;
    Stub& operator=(const Stub&) = delete;
    Stub& operator=(Stub&&)      = delete;

    // Returnerar timerns timeout i ms
    [[nodiscard]] std::uint16_t timeout_ms() const noexcept override
    {
        return myTimeout_ms;
    }

    // Returnerar true om timern är igång
    [[nodiscard]] bool isRunning() const noexcept override
    {
        return myRunning;
    }

    // Startar timern och nollställer räknaren
    void start() noexcept override
    {
        myRunning = true;
        myCounter = 0U;
    }

    // Stoppar timern, men lämnar räknaren orörd
    void stop() noexcept override
    {
        myRunning = false;
    }

    // Växlar mellan igång och stoppad, och nollställer räknaren
    void toggle() noexcept override
    {
        myRunning = !myRunning;
        myCounter = 0U;
    }

    // Räknar upp räknaren med ett - men bara om timern är igång och räknaren ej nått timeout
    void tick() noexcept override
    {
        if (myRunning && myCounter < myTimeout_ms)
        {
            ++myCounter;
        }
    }

    // Returnerar true när räknaren har nått timeouten och nollställer den direkt
    [[nodiscard]] bool hasTimedOut() const noexcept override
    {
        if (myCounter >= myTimeout_ms)
        {
            const_cast<Stub*>(this)->myCounter = 0U;
            return true;
        }
        return false;
    }

private:
    const std::uint16_t myTimeout_ms;
    mutable std::uint16_t myCounter{0U}; // mutable för att hasTimedOut() ska kunna nollställa trots const-kvalificering
    bool myRunning;
};

} // namespace driver::timer