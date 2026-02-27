# Embedded UART Communication
## Overview
A bare-metal C application developed for the Blackboard (Zynq-7000) hardware platform. This project implements a serial driver by directly interfacing with memory-mapped I/O registers to enable communication with a PC via PuTTY.

## Features
- **Register-Level Configuration:** Manual setup of Control, Mode, and Baud Rate Generator registers.
- **Baud Rate:** Configured for 115200 (8N1).
- **Status Polling:** Robust TX/RX FIFO handling using status register monitoring.

## Hardware Used
- Blackboard (ARM Cortex-A9 based Zynq SoC)
- Micro-USB cable for UART-to-USB bridge
