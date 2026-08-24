#pragma once

namespace driver::gpio
{
	class Interface
	{

	 public: 
		virtual ~Interface() = default;
			
		virtual void write(bool high) noexcept = 0;

		[[nodiscard]] virtual bool read() const noexcept = 0;

		virtual void toggle() noexcept = 0;

	};


} // namespace driver::gpio:Interface
