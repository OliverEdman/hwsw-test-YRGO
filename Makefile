format:
	find libs/atmega -name "*.cpp" -o -name "*.h" | xargs clang-format -i
