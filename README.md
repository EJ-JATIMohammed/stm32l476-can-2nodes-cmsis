# Two-Node CAN Communication using STM32L476RG and CMSIS

## Project Overview

This project implements a two-node CAN (Controller Area Network) communication system using two **STM32L476RG microcontroller units (MCUs)** with CMSIS (Cortex Microcontroller Software Interface Standard). The system demonstrates interrupt-driven button input on Node 1 that triggers CAN message transmission to Node 2, which responds by updating its LED status based on received messages.

### Application Behavior

The system operates as follows:

1. **Node 1 (Transmitter):**
   - Detects button press via external interrupt (GPIO PC13)
   - Generates and transmits a CAN message to Node 2
   - Cycles through a predefined LED pattern with each button press
   - Uses a counter (`led_index`) to select which LED data to transmit

2. **Node 2 (Receiver):**
   - Receives CAN messages from Node 1
   - Extracts LED pattern data from the received message
   - Updates its LED output (GPIO PB11-PB15) to display the pattern
   - Four LEDs are controlled sequentially: one LED turns on while the other three turn off
   - LED pattern cycles continuously with each received message

---

## Hardware Information

### Microcontroller Units

- **MCU:** STM32L476RG (ARM Cortex-M4)
- **Quantity:** 2 units (Node 1 and Node 2)
- **Features:**
  - CAN 2.0B controller support
  - Interrupt-capable GPIO pins
  - Multiple port options for CAN communication

### CAN Communication

- **Protocol:** CAN 2.0B
- **Message Format:** Extended CAN ID
- **CAN ID:** 0x123 (Extended identifier)
- **Message Length:** 1 byte (DLC = 1)
- **Baud Rate Configuration:**
  - Synchronization Jump Width (SJW): 1 time quantum (tq)
  - Bit Segment 1 (BS1): 13 tq
  - Bit Segment 2 (BS2): 2 tq
  - Prescaler: 1

### Connectivity

- **CAN Transceiver Interface:** GPIO PB8 (RX) and PB9 (TX)
  - Alternative function mode configured
  - High-speed GPIO configuration
  - Pull-up enabled on RX pin

---

## Node Descriptions

### Node 1: Button-Triggered Transmitter

**GPIO Configuration:**
- **Button Input:** GPIO PC13 (falling edge trigger)
- **CAN Interface:** PB8 (RX) / PB9 (TX)

**Functionality:**
- Initializes external interrupt on button pin (PC13)
- Waits for button press (falling edge detection)
- Upon interrupt, transmits CAN message with LED pattern byte
- Cycles through 5 LED patterns: `0x01`, `0x02`, `0x04`, `0x08`, `0x10`
- Increments pattern index after each transmission

**Key Features:**
- Interrupt-driven design (EXTI15_10_IRQHandler)
- CAN message transmission on button events
- LED pattern cycling mechanism

### Node 2: CAN Receiver with LED Output

**GPIO Configuration:**
- **LED Output:** GPIO PB11, PB12, PB13, PB14, PB15 (output mode)
- **CAN Interface:** PB8 (RX) / PB9 (TX)

**Functionality:**
- Initializes LEDs on GPIO Port B (pins 11-15)
- Receives CAN messages via interrupt handlers (CAN1_RX0 and CAN1_RX1)
- Extracts LED pattern data from received message
- Updates GPIO output to display the received pattern
- One LED remains illuminated per received message

**Key Features:**
- CAN reception via FIFO0 and FIFO1
- Direct LED control based on received data
- Automatic pattern update on message reception

---

## CMSIS and Driver Architecture

### CAN Driver Implementation

The project includes a reusable CAN driver that provides abstraction over CMSIS register operations.

**Driver Files:**
- `node_1/can_driver/Inc/can_driver.h` — CAN driver interface
- `node_1/can_driver/Src/can_driver.c` — CAN driver implementation
- `node_2/can_driver/` — Identical driver for Node 2

**Driver Functions:**

| Function | Purpose |
|----------|---------|
| `can_gpio_init()` | Configure GPIO pins for CAN communication (supports PA11/PA12, PB8/PB9, PD0/PD1) |
| `can_init()` | Initialize CAN controller with bit timing and mode parameters |
| `can_transmit()` | Send CAN message with ID, extended format flag, RTR flag, and data |
| `can_filter_init()` | Configure CAN message filter (32-bit, mask mode, FIFO routing) |
| `can_receive()` | Receive CAN message from specified FIFO with full message parsing |

**CMSIS Features Used:**
- Direct hardware register access via CMSIS-compliant structures
- Interrupt-driven reception (CAN1_RX0_IRQn, CAN1_RX1_IRQn)
- GPIO alternate function configuration
- Clock and NVIC management

### CAN Configuration Parameters (CMSIS Bit Fields)

Both nodes use identical CAN configuration:

```c
TTCM:  Time-Triggered Communication Mode      → Disabled
ABOM:  Automatic Bus-Off Management           → Disabled
AWUM:  Automatic Wake-Up Mode                 → Disabled
NART:  No Automatic Retransmission            → Disabled
RFLM:  Receive FIFO Locked Mode               → Enabled
TXFP:  Transmit FIFO Priority                 → Disabled
LBKM:  Loopback Mode                          → Disabled
SILM:  Silent Mode                            → Disabled
```

### Filter Configuration

- **Filter Scale:** 32-bit
- **Filter Mode:** Identifier mask mode
- **FIFO Assignment:** FIFO1
- **Filter Registers:**
  - R1 (ID Filter): `(0x00000120 << 3) | (1 << 2)`
  - R2 (Mask Filter): `(0x1FFFFFF0 << 3) | (1 << 2)`
  - Accepts messages matching ID 0x12x pattern

---

## Project Structure

```
stm32l476-can-2nodes-cmsis/
├── node_1/                          # Transmitter node
│   ├── application/
│   │   └── main_node1.c             # Node 1 main application (button, interrupt, transmission)
│   └── can_driver/
│       ├── Inc/
│       │   └── can_driver.h         # CAN driver interface
│       └── Src/
│           └── can_driver.c         # CAN driver implementation
│
├── node_2/                          # Receiver node
│   ├── application/
│   │   └── main_node2.c             # Node 2 main application (LED control, reception)
│   └── can_driver/
│       ├── Inc/
│       │   └── can_driver.h         # CAN driver interface (identical to node_1)
│       └── Src/
│           └── can_driver.c         # CAN driver implementation (identical to node_1)
│
└── README.md                        # This file
```

---

## Development Environment

### Required Tools

- **ARM Compiler or GCC:** For STM32L476 C compilation
- **STM32L4 CMSIS Headers:** `stm32l4xx.h` and related
- **Programmer/Debugger:** ST-Link or compatible tool
- **IDE (Optional):** STM32CubeIDE, Keil µVision, or IAR Embedded Workbench

### Dependencies

- **CMSIS Core:** ARM Cortex-M4 CMSIS library
- **STM32L4xx Peripheral Headers:** Official ST device headers
- **C Standard Library:** `stdint.h`, `stdbool.h`

---

## Build and Usage Instructions

### Building the Project

#### Using STM32CubeIDE

1. Import the project into STM32CubeIDE
2. Configure project settings:
   - Target MCU: STM32L476RG
   - Build output folder for each node (Node 1 and Node 2)
3. For **Node 1**: Build `node_1/application/main_node1.c` with `node_1/can_driver/`
4. For **Node 2**: Build `node_2/application/main_node2.c` with `node_2/can_driver/`

#### Using GNU Arm Toolchain (Command Line)

```bash
# For Node 1
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -I./node_1/can_driver/Inc \
  node_1/application/main_node1.c node_1/can_driver/Src/can_driver.c \
  -o node1.elf -lm

# For Node 2
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -I./node_2/can_driver/Inc \
  node_2/application/main_node2.c node_2/can_driver/Src/can_driver.c \
  -o node2.elf -lm
```

### Programming the Boards

1. **Node 1 Firmware:** Program `node1.elf` to the first STM32L476RG board
2. **Node 2 Firmware:** Program `node2.elf` to the second STM32L476RG board
3. Establish CAN bus connection between the two nodes (CAN_L and CAN_H lines)

### Operation

1. Power on both boards
2. Ensure CAN transceiver module is properly connected
3. Press the button on Node 1:
   - Node 1 transmits a CAN message with LED pattern data
   - Node 2 receives the message and updates its LED output
4. Repeat button presses to cycle through LED patterns
5. Observe LED changes on Node 2 in response to each button press

---

## Features

### Core Features

✓ **Two-Node CAN Communication**
  - Reliable message transmission between STM32L476RG boards
  - Extended CAN ID (0x123) with message filtering

✓ **Interrupt-Driven Design**
  - Button press detection via EXTI (External Interrupt) on PC13
  - CAN message reception via CAN peripheral interrupts

✓ **CMSIS-Based Driver**
  - Direct CMSIS register access for efficiency
  - Portable and reusable CAN driver
  - Support for multiple GPIO configurations (PA, PB, PD ports)

✓ **LED Pattern Control**
  - Cyclic LED pattern selection on Node 1
  - Real-time LED update on Node 2 based on received messages
  - 5-state pattern: one LED on, others off (patterns: 0x01, 0x02, 0x04, 0x08, 0x10)

✓ **Configurable CAN Parameters**
  - Flexible bit timing configuration (SJW, BS1, BS2, Prescaler)
  - Programmable message filtering
  - FIFO-based reception with automatic release

### Design Highlights

- **Low-Level Control:** Direct CMSIS register manipulation ensures minimal overhead
- **Modular Architecture:** Reusable CAN driver for easy adaptation to other projects
- **Efficient Interrupts:** EXTI and CAN interrupts eliminate polling overhead
- **Standard Protocol Compliance:** Full CAN 2.0B compliance with proper error handling

---

## Notes

- CAN communication requires proper termination resistors (typically 120Ω at each end of the bus)
- Ensure both boards use the same CAN baud rate settings for reliable communication
- The button on Node 1 must be configured with appropriate debouncing in a production environment
- GPIO pins and CAN peripheral clocks are managed at the driver level using CMSIS register writes

---

## License

This project is provided as-is for educational and embedded systems portfolio purposes.

## Author

**EJ-JATIMohammed**

Repository: [stm32l476-can-2nodes-cmsis](https://github.com/EJ-JATIMohammed/stm32l476-can-2nodes-cmsis)
