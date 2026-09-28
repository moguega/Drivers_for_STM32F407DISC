# STM32F4xx Custom Peripheral Drivers

Custom bare-metal peripheral drivers developed from scratch in C for the **STM32F407G-DISC1** board (ARM Cortex-M4).

## 🚀 Peripherals Implemented

- **GPIO Driver**:
  - Pin mode configuration (Input, Output, Alternate Function, Analog).
  - Output types (Push-Pull, Open-Drain) and speed controls.
  - Pull-up / Pull-down resistor controls.
  - EXTI configuration with falling/rising edge detection.
  - Bit manipulation and atomic operations.

- **SPI Driver**:
  - Master and Slave mode support.
  - Full-Duplex, Half-Duplex, and Simplex configurations.
  - Software Slave Management (SSM / SSI).
  - Clock phase (CPHA) and polarity (CPOL) modes.
  - Blocking APIs (`SPI_SendData`, `SPI_ReceiveData`).
  - **Interrupt-driven non-blocking APIs** (`SPI_SendDataIT`, `SPI_ReceiveDataIT`).
  - Event callback architecture (`SPI_ApplicationEventCallback`).

## 📁 Project Structure

```
STM32F4xx_drivers/
├── drivers/
│   ├── Inc/               # Driver header files (stm32f407.h, SPI_Driver.h, stm32f407_GPIO_driver.h)
│   └── Src/               # Driver source code (STM32F407_SPI_Driver.c, stm32f407xx_gpio.c)
├── Src/                   # Test applications and examples
│   ├── 009test_SPI.c      # On-chip SPI1 (Master) to SPI2 (Slave) interrupt loopback test
│   ├── syscalls.c         # Newlib syscalls with ITM / SWV printf redirection
│   └── ...
├── Startup/               # Vector table and startup assembly
├── STM32F407VGTX_FLASH.ld # Linker script
└── STM32F4xx_drivers.launch # Debug configuration for STM32CubeIDE (SWV enabled)
```

## 🛠️ Toolchain

- **IDE**: STM32CubeIDE
- **Compiler**: `arm-none-eabi-gcc`
- **Architecture**: ARM Cortex-M4
- **Hardware**: STM32F407G-DISC1
