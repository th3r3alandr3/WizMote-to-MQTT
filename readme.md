# WizMote to MQTT

## Overview

This project is a simple bridge between WizMote and Home Assistant. It listens for WizMote messages and forwards them to Home Assistant via MQTT.

## Getting Started

### Prerequisites

- [PlatformIO IDE](https://platformio.org/install/ide?install=vscode)
- WizMote
- ESP32 or ESP8266 board
- Home Assistant with MQTT broker

### Configuration

Copy `.env.example` to `.env` and update the configuration.

### Building

Please refer to the [PlatformIO documentation](https://docs.platformio.org/en/latest/quickstart.html) for building and uploading the firmware.

### Usage

The device will connect to the configured WiFi network and MQTT broker. It will listen for WizMote messages and forward them to Home Assistant.

When a new WizMote is discovered, it will be added to the Home Assistant entity registry. You can then use the entity in automations and scripts.




