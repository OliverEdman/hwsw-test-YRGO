#pragma once

#include "interface.h"

namespace driver::gpio 
{
    class Stub final : Interface
    {
        private:
        bool my_state;
        
        public:
            explicit Stub (bool initial_state = false) noexcept

            : my_state(initial_state)
            {}

            ~Stub() noexcept override = default;
            
            // No copy move operators.
            Stub(const Stub&)               = delete;
            Stub& operator=(const Stub&)    = delete;
            Stub(Stub&&)                    = delete;
            Stub& operator=(Stub&&)         = delete;

            void write(bool high) noexcept override
            {
                my_state = high;
            }

            [[nodiscard]] bool read() const noexcept override 
            {
                return my_state;
            }
            void toggle() noexcept override
            {
                my_state = !my_state;
            }
    }; 

} // namespace driver::gpio