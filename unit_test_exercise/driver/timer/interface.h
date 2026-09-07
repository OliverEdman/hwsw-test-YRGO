#pragma once

#include <cstdint>

namespace driver::timer 
{
class Interface
{
public:

    // Destruktor
    virtual ~Interface() noexcept = default;

    // retunera timerns timeout i ms
    [[nodiscard]] virtual std::uint16_t timeout_ms() const noexcept = 0;

    // retunera true om timern är igång
    [[nodiscard]] virtual bool isRunning() const noexcept = 0;

    // startar timern
    virtual void start() noexcept = 0;

    //stoppar timern.
    virtual void stop() noexcept = 0;

    // Växlar mellan igång och stoppad.
    virtual void toggle() noexcept = 0;

    //räknar upp timern ett steg, motsvarande 1 ms.
    virtual void tick() noexcept = 0;

    //returnerar true om timern har timat ut.
    [[nodiscard]] virtual bool hasTimedOut() const noexcept = 0;

    // 3 av dessa ska bara läsa tillstånd därav const samt nodiscard

};
}// namespace driver::timer