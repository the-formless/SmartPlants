# SmartPlants Firmware Agent Guidelines

## Build Commands
- Build: `make` (SDCC compiler for STM8)
- Clean: `make clean`
- Flash: `make flash` (ST-Link)
- Lint: `cppcheck --enable=all src/ inc/` (if cppcheck installed)

## Test Commands
- No automated tests yet
- Manual testing: Build and flash to hardware
- Run single test: N/A (implement Unity framework for unit tests)

## Code Style Guidelines
- **C Standards**: SDCC compiler, MISRA C compliance
- **Naming**: snake_case functions/variables, UPPER_SNAKE_CASE constants, module prefixes (e.g., `sensor_temp`)
- **Types**: stdint.h types (uint8_t), typedef structs
- **File Organization**: Headers in `inc/`, sources in `src/`, HAL abstraction layer
- **Error Handling**: Return error codes, debug assertions, UART logging
- **Documentation**: Doxygen comments for APIs, parameter/return docs</content>
<parameter name="filePath">/home/nirakaar/Developer/SmartPlants/AGENTS.md