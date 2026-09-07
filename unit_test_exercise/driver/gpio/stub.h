#pragma once

#include "interface.h"

namespace driver::gpio
{

class Stub final : public Interface 
{
public: 

~Stub() noexcept override = default;

explicit Stub(bool initial_state = false) noexcept;

bool write (bool high) noexcept override;

[[nodiscard]] bool read() const noexcept override;

void toggle() noexcept override;
 
    // Stäng av kopiering och flytt
    Stub(const Stub&) = delete;
    Stub& operator=(const Stub&) = delete;
    Stub(Stub&&) = delete;
    Stub& operator=(Stub&&) = delete;

private: 

bool my_state = false;


};
} // namespace driver::gpio
