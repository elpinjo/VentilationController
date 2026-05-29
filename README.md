# VentilationController

An IoT firmware project for ESP8266/ESP32 microcontrollers that provides remote monitoring and control of ventilation systems with integrated environmental sensing.

## Features

- 🌡️ **Environmental Monitoring** - Temperature, humidity, and pressure sensing via BME280
- 🔌 **Relay Control** - Remote ON/OFF control of ventilation equipment
- 📡 **WiFi Connectivity** - Dual-mode operation (AP and Station)
- 🌐 **HTTP REST API** - Simple web interface for control and monitoring
- 📊 **MQTT Integration** - Automatic publishing of sensor data to MQTT broker
- 🔄 **OTA Updates** - Over-the-air firmware updates
- 💾 **Persistent Configuration** - SPIFFS-based settings storage

## Hardware Requirements

### Supported Boards
- ESP8266 (ESP-12E)
- ESP32

### Components
- **BME280 Sensor** - I2C environmental sensor (address 0x76)
- **Relay Module** - Connected to GPIO26 (ESP32) or D5 (ESP8266)
- **Power Supply** - Appropriate for your ESP board

### Wiring

#### ESP8266 (ESP-12E)
- BME280 SDA → D2 (GPIO4)
- BME280 SCL → D1 (GPIO5)
- Relay Control → D5 (GPIO14)

#### ESP32
- BME280 SDA → GPIO21
- BME280 SCL → GPIO22
- Relay Control → GPIO26

## Software Requirements

- [PlatformIO](https://platformio.org/) (recommended) or Arduino IDE
- USB drivers for your ESP board

## Installation

### Using PlatformIO (Recommended)

1. Clone the repository:
```bash
git clone <repository-url>
cd VentilationController
```

2. Install dependencies (automatic with PlatformIO):
```bash
pio lib install
```

3. Build the project:
```bash
pio run
```

4. Upload to your board:
```bash
# For ESP8266
pio run -e esp8266dev -t upload

# For ESP32
pio run -e esp32dev -t upload
```

### Using Arduino IDE

1. Install required libraries via Library Manager:
   - ArduinoJson
   - Adafruit BME280 Library
   - Adafruit Unified Sensor

2. Open `src/main.cpp` in Arduino IDE
3. Select your board and port
4. Upload

## Configuration

### Initial Setup

On first boot, the device creates a WiFi access point:

- **SSID**: `VENTILATOR_AP`
- **Password**: `ud8ejr9304`

Connect to this network and configure your WiFi credentials.

### WiFi Configuration

Send a POST request to configure WiFi:

```bash
curl -X POST http://192.168.4.1/configNetwork \
  -H "Content-Type: application/json" \
  -d '{"ssid":"YourWiFiSSID","networkSecret":"YourPassword"}'
```

Or use GET parameters:
```
http://192.168.4.1/configNetwork?ssid=YourWiFiSSID&networkSecret=YourPassword
```

After configuration, the device will restart and connect to your network.

### MQTT Configuration

Configure MQTT broker connection to enable automatic sensor data publishing:

**POST Request:**
```bash
curl -X POST http://<device-ip>/configMQTT \
  -H "Content-Type: application/json" \
  -d '{
    "broker":"mqtt.example.com",
    "port":1883,
    "user":"username",
    "password":"password",
    "clientId":"ventilator_01"
  }'
```

**GET Request:**
```
http://<device-ip>/configMQTT?broker=mqtt.example.com&port=1883&user=username&password=password&clientId=ventilator_01
```

**MQTT Topics:**
- `ventilator/sensor/temperature` - Temperature readings (°C)
- `ventilator/sensor/humidity` - Humidity readings (%)
- `ventilator/sensor/pressure` - Pressure readings (hPa)
- `ventilator/status` - Device status (online/offline)

Data is published every 60 seconds automatically once MQTT is configured.

## API Endpoints

Once connected to your network, access the device via its IP address:

### Control Endpoints

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/on` | GET | Turn ventilator ON |
| `/off` | GET | Turn ventilator OFF |
| `/temperature` | GET | Get current temperature |
| `/humidity` | GET | Get current humidity |
| `/reset` | GET | Restart the device |
| `/configNetwork` | POST/GET | Update WiFi configuration |
| `/configMQTT` | POST/GET | Configure MQTT broker connection |

### Examples

```bash
# Turn ventilator on
curl http://<device-ip>/on

# Get temperature
curl http://<device-ip>/temperature

# Get humidity
curl http://<device-ip>/humidity

# Turn ventilator off
curl http://<device-ip>/off
```

## OTA Updates

The device supports Over-The-Air updates via ArduinoOTA:

1. Ensure the device is connected to your network
2. Use PlatformIO or Arduino IDE OTA upload feature
3. Select the device from the network ports list

```bash
# PlatformIO OTA upload
pio run -e esp8266dev -t upload --upload-port <device-ip>
```

## Project Structure

```
VentilationController/
├── src/
│   ├── main.cpp              # Entry point
│   ├── application/
│   │   ├── core.cpp          # Main application logic
│   │   └── core.hpp
│   ├── config/
│   │   ├── config.cpp        # Configuration management
│   │   └── config.hpp
│   ├── network/
│   │   ├── wlan.cpp          # WiFi and HTTP server
│   │   ├── wlan.hpp
│   │   ├── HTTPRequest.cpp   # HTTP request parser
│   │   └── HTTPRequest.hpp
│   ├── ota/
│   │   ├── ota.cpp           # OTA update handler
│   │   └── ota.hpp
│   └── webserver/
│       └── webserver.hpp     # Web server interface
├── platformio.ini            # PlatformIO configuration
└── README.md
```

## Development

### Building for Different Platforms

```bash
# ESP8266
pio run -e esp8266dev

# ESP32
pio run -e esp32dev
```

### Serial Monitor

```bash
pio device monitor -b 115200
```

### Customization

- **Relay Pin**: Modify `RELAY0_PIN` in [`core.cpp`](src/application/core.cpp:11)
- **BME280 Address**: Change in [`core.cpp`](src/application/core.cpp:36) if using alternate address
- **AP Credentials**: Update in [`wlan.hpp`](src/network/wlan.hpp:42)
- **Server Port**: Modify in [`wlan.cpp`](src/network/wlan.cpp:15)

## Troubleshooting

### BME280 Not Found
- Check I2C wiring (SDA/SCL)
- Verify sensor address (default 0x76, some use 0x77)
- Check power supply to sensor

### Cannot Connect to WiFi
- Verify credentials are correct
- Check signal strength
- Reset configuration via `/reset` endpoint
- Reflash firmware if needed

### OTA Upload Fails
- Ensure device is on same network
- Check firewall settings
- Verify device IP address
- Try serial upload instead

## Security Considerations

⚠️ **Important**: This firmware has no authentication on HTTP endpoints. Recommendations:

- Use on trusted networks only
- Implement authentication before production use
- Consider HTTPS for sensitive deployments
- Change default AP password in code

## License

[Specify your license here]

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues.

## Acknowledgments

- Built with [PlatformIO](https://platformio.org/)
- Uses [ArduinoJson](https://arduinojson.org/)
- Sensor support via [Adafruit BME280 Library](https://github.com/adafruit/Adafruit_BME280_Library)