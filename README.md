## IoT-Based Server Room Monitoring and Control System	
### Overview

This project is an **Arduino-based** system designed to monitor environmental conditions such as temperature, humidity, and flame presence. It integrates multiple sensors, including a **MAX6675 thermocouple** for temperature measurement, a **DHT11 sensor** for humidity, and a **flame sensor** for fire detection. The system also features an **ESP8266 Wi-Fi** module to send data to ThingSpeak, an IoT platform, for real-time monitoring and analysis.

### Key Features

***Temperature Monitoring:***

- Uses a MAX6675 thermocouple to measure high temperatures accurately.

- Displays temperature readings on an I2C LCD.

***Humidity Monitoring:***

- Utilizes a DHT11 sensor to measure ambient humidity.

- Displays humidity readings alongside temperature on the LCD.

***Flame Detection:***

- Detects flames using a flame sensor.

- Triggers an alarm (buzzer) and visual indicators (LEDs) when a flame is detected.

***Threshold Alert System:***

- Alerts users when the temperature exceeds a predefined threshold.

- Activates a relay and visual/audio indicators (LEDs and buzzer) for warnings.

***IoT Integration:***

- Sends temperature and humidity data to ThingSpeak using the ESP8266 Wi-Fi module.

- Enables remote monitoring and data logging.
