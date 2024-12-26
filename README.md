# **Firmware Over-the-Air (FOTA) System**

This project demonstrates a reliable Firmware Over-the-Air (FOTA) update system using the **ESP82xx Wi-Fi module** and **STM32F4 microcontroller**. The solution is designed for seamless firmware retrieval, parsing, and updates, ensuring operational stability and efficiency. The project focuses on simplicity and robustness, providing a stable platform for further development in embedded systems.

---

## **Features**
- **Firmware Updates**: 
  - Wirelessly retrieves and updates firmware over Wi-Fi.
  - Ensures secure, validated firmware installation.
- **Memory Management**: 
  - Implements bootloader, primary, secondary, and rollback partitions.
  - Efficient flash memory handling for update integrity.
- **Communication Protocols**: 
  - HTTP requests for firmware retrieval.
  - UART for ESP8266 communication.
- **Error Handling**:
  - Recovery mechanisms to address failed updates.
  - Validation of firmware integrity before installation.

---

## **Hardware Setup**
- **Microcontroller**: STM32F4 series
- **Wi-Fi Module**: ESP82xx
- **Optional**: SD card for backup storage (future work)

### **Pin Configuration**
| **Component**       | **Pin**   | **Function**              |
|----------------------|-----------|---------------------------|
| ESP TX              | PA9       | UART TX for ESP communication |
| ESP RX              | PA10      | UART RX for ESP communication |
| GPIO for Reset      | PA8       | ESP Reset pin             |
| GPIO for Enable     | 3.3V      | ESP Enable signal         |

---

## **Firmware Workflow**
1. **Update Retrieval**:
   - Firmware is fetched wirelessly using HTTP GET requests.
   - The ESP module communicates with the STM32 via UART.
2. **Validation and Parsing**:
   - Retrieved firmware is validated for integrity using checksum or markers.
   - Parsed firmware is stored in temporary buffers.
3. **Memory Write**:
   - Valid firmware is written to the secondary partition.
   - Rollback mechanism is in place to revert to the previous version in case of failure.
4. **Installation**:
   - Upon successful validation, the system reboots into the new firmware.

---

## **Tools and Technologies**
- **Programming Language**: C
- **Hardware Development**: Bare-metal programming
- **Communication**: UART, HTTP, I2C
- **Flash Management**: STM32 Flash Memory API
- **Debugging Tools**: Logic analyzer, ST-Link

---

## **Error Handling and Challenges**
- **UART Communication**:
  - Resolved buffer overflow issues during data transfer between ESP and STM32.
- **Flash Memory Writing**:
  - Implemented proper memory alignment for firmware sectors.
- **SD Card Integration**:
  - Encountered file system issues; SD card backup remains a future improvement.

---

## **Future Improvements**
1. **Enhanced Security**:
   - Add cryptographic signature validation for firmware authenticity.
2. **MQTT Support**:
   - Replace HTTP with MQTT for more efficient communication.
3. **SD Card Integration**:
   - Resolve file system compatibility to enable local firmware backup.
