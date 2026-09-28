# STM32L476 Two-Node CAN Communication System

## Project Overview

This project demonstrates an **event-driven inter-MCU communication system** using two STM32L476RG microcontrollers connected via CAN (Controller Area Network) bus. The system showcases real-time event propagation where a physical button press on one MCU triggers an LED response on the other, illustrating practical embedded systems design patterns including interrupt handling, CMSIS-based peripheral control, and robust communication protocols.

The architecture emphasizes **separation of concerns** with one MCU acting as a dedicated transmitter node (handling user input through external interrupts) and another as a receiver node (controlling output devices based on received messages). This modular design pattern is commonly used in automotive and industrial embedded applications.

## System Architecture

```
┌──────────────────────────────┐
│     STM32L476RG #1           │
│   Transmitter Node           │
│                              │
│ Button (PC13)                │
│   ↓                          │
│ EXTI Interrupt (EXTI15_10)   │
│   ↓                          │
│ CAN Message TX               │
└──────────────┬───────────────┘
               │
               │ CAN Bus (PB8/PB9)
               │
┌──────────────▼───────────────┐
│     STM32L476RG #2           │
│    Receiver Node             │
│                              │
│ CAN Message RX (FIFO)        │
│   ↓                          │
│ Interrupt Handler            │
│   ↓                          │
│ LED Update (PB11-PB15)       │
└──────────────────────────────┘
```

## Project Architecture

### Directory Structure

```
stm32l476-can-2nodes-cmsis/
├── node_1/                          # Transmitter MCU Project
│   ├── application/
│   │   └── main_node1.c             # Main application: button handling & CAN TX
│   └── can_driver/
│       ├── Inc/
│       │   └── can_driver.h         # CAN driver public interface
│       └── Src/
│           └── can_driver.c         # CAN driver implementation (GPIO, init, TX/RX, filters)
│
└── node_2/                          # Receiver MCU Project
    ├── application/
    │   └── main_node2.c             # Main application: LED control & CAN RX
    └── can_driver/
        ├── Inc/
        │   └── can_driver.h         # CAN driver public interface
        └── Src/
            └── can_driver.c         # CAN driver implementation
```

### Component Description

- **main_node1.c**: Initializes GPIO for button input (PC13), configures EXTI interrupt on falling edge, sets up CAN controller, and transmits LED state values (0x1, 0x2, 0x4, 0x8, 0x10) sequentially with each button press.

- **main_node2.c**: Initializes GPIO for LED output (PB11-PB15 in push-pull mode), configures CAN receiver with filters, and updates LED state based on received CAN messages via FIFO interrupt handlers.

- **can_driver.h**: Declares all CAN driver functions with comprehensive parameter documentation, supporting GPIO configuration, CAN initialization, message transmission/reception, and filter setup.

- **can_driver.c**: Implements low-level CAN peripheral control using CMSIS register access, including GPIO pin configuration for three CAN transceiver options (PA11/PA12, PB8/PB9, PD0/PD1), bit timing calculations, mailbox management, and FIFO-based reception.

## Main Features

- **External Interrupt-Based Button Detection**: GPIO pin PC13 configured with falling-edge trigger using EXTI15_10_IRQHandler for responsive button input.
- **CAN Bus Communication**: Both nodes use CAN1 configured for extended ID messaging at 1 Mbps with CMSIS register-level control.
- **Event-Driven Architecture**: Button press triggers EXTI interrupt → CAN transmission → receiver FIFO interrupt → LED update, implementing a complete event propagation chain.
- **LED State Cycling**: Node 1 cycles through five LED patterns (0x1, 0x2, 0x4, 0x8, 0x10) on successive button presses; Node 2 displays the received pattern on its LED bank.
- **Flexible CAN GPIO Configuration**: Driver supports three different pin configurations (PA11/PA12, PB8/PB9, or PD0/PD1) for CAN transceiver connection.
- **Message Filtering**: Receive FIFO filters configured in mask mode to accept messages with IDs matching 0x12x pattern.
- **Dual FIFO Reception**: Handles CAN reception through both FIFO0 and FIFO1 with independent interrupt handlers.

## Hardware

### Microcontroller
- **MCU**: STM32L476RG (Arm Cortex-M4, 80 MHz)
- **Count**: 2 units (one per node)

### Communication
- **Protocol**: CAN 2.0B (Extended ID)
- **Baud Rate**: 1 Mbps (SJW=1Tq, BS1=13Tq, BS2=2Tq, Prescaler=1)
- **Pins (Default)**: CAN1 on PB8 (RX) and PB9 (TX)
- **Transceiver**: External CAN transceiver module required (not included)

### Input/Output
- **Button**: Tactile switch on GPIO PC13 (Node 1), active low
- **LEDs**: Five discrete LEDs connected to GPIO PB11, PB12, PB13, PB14, PB15 (Node 2), active high

## How It Works

```
Button Press on Node 1
        ↓
EXTI15_10_IRQHandler triggered (falling edge on PC13)
        ↓
ISR increments led_index and transmits via CAN
        ↓
CAN Message (ID: 0x123, Data: leds_data[led_index])
        ↓
CAN Bus
        ↓
Node 2 receives message in FIFO
        ↓
CAN1_RX0_IRQHandler or CAN1_RX1_IRQHandler triggered
        ↓
ISR reads message and extracts LED pattern from data[0]
        ↓
GPIO GPIOB->ODR updated (PB11-PB15 driven with new pattern)
        ↓
LEDs on Node 2 light up according to received pattern
```

**Latency Path**: Button → EXTI IRQ (~microseconds) → CAN TX → CAN propagation (~microseconds) → CAN RX → LED GPIO update (~microseconds)

## Build and Run

### Prerequisites
- STM32CubeIDE or any ARM GCC-based toolchain
- STM32L476 CMSIS headers and startup files
- JTAG/SWD debugger for programming (ST-Link v2 or compatible)
- CAN transceiver module (e.g., TJA1050)

### Building

1. **Node 1 (Transmitter)**
   ```bash
   cd node_1
   arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O2 \
     -I. -Ican_driver/Inc \
     application/main_node1.c can_driver/Src/can_driver.c \
     -o node1.elf
   ```

2. **Node 2 (Receiver)**
   ```bash
   cd node_2
   arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O2 \
     -I. -Ican_driver/Inc \
     application/main_node2.c can_driver/Src/can_driver.c \
     -o node2.elf
   ```

### Programming

Use ST-Link or your debugger to flash both MCUs:
```bash
st-flash write node1.elf 0x08000000  # Node 1
st-flash write node2.elf 0x08000000  # Node 2
```

### Operation

1. Connect CAN transceiver module between both MCUs on PB8/PB9
2. Connect 120Ω termination resistors at both ends of CAN bus
3. Power both boards
4. Press button on Node 1 to cycle LED patterns on Node 2

## Project Structure

```
Root Directory
├── node_1/
│   ├── application/
│   │   └── main_node1.c             (160 lines)
│   └── can_driver/
│       ├── Inc/
│       │   └── can_driver.h          (121 lines)
│       └── Src/
│           └── can_driver.c          (555 lines)
│
├── node_2/
│   ├── application/
│   │   └── main_node2.c             (131 lines)
│   └── can_driver/
│       ├── Inc/
│       │   └── can_driver.h          (121 lines)
│       └── Src/
│           └── can_driver.c          (555 lines)
│
├── README.md
└── LICENSE
```

## CAN Message Format

- **Message ID**: 0x123 (29-bit extended ID)
- **Message Type**: Data frame (not RTR)
- **Data Length Code (DLC)**: 1 byte
- **Data Byte 0**: LED pattern (0x1, 0x2, 0x4, 0x8, or 0x10)
- **Bit Timing**: 1 Mbps using SJW=1, BS1=13, BS2=2, Prescaler=1

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

## Author

**EJ-JATIMohammed**

---

For questions or issues, please open a GitHub issue on this repository.
