Fahim-BotOS v1.0
​Developed by My Gadget BD 🇧🇩
​Fahim-BotOS is a high-performance, multitasking embedded operating system specifically architected for ESP32-C3 based smart pet robots. It empowers robots with lifelike expressions, reactive sensor feedback, and seamless cloud-based firmware management.
​🚀 Key Features
​Real-time Multitasking: Powered by the FreeRTOS engine, ensuring simultaneous handling of animations, sensor polling, and network tasks without lag.
​Professional Canvas Animations: High-fidelity frame-by-frame rendering for OLED displays. Includes smooth transitions for moods like Idle, Happy, Angry, and Dizzy.
​Intelligent Interaction: Utilizing the MPU6050 gyroscope and digital touch sensors for instant physical-to-digital responsiveness.
​Self-Healing OTA Update: Built-in Over-The-Air (OTA) update system allowing the robot to fetch and install the latest firmware directly from GitHub or a remote server.
​Environmental Awareness: Integrated WiFi support to fetch real-time weather data and NTP time synchronization.
​Auditory Feedback: Native support for the ISD1820 voice module to trigger pre-recorded audio responses.
​🛠 Hardware Specifications
​To run Fahim-BotOS, the following hardware configuration is required:
​MCU: ESP32-C3 (RISC-V architecture)
​Display: 0.96" SSD1306 OLED (I2C)
​Sensors: MPU6050 Accelerometer/Gyro, 2x Digital Touch Sensors
​Audio: ISD1820 Voice Recorder/Player Module & Speaker
​Power: 3.7V Li-ion battery with integrated charging circuit
​📂 Project Architecture (OOP)
​The OS is built using a modular Object-Oriented approach for scalability:
​DisplayManager: Handles all pixel-perfect drawings and animation frames.
​SensorManager: Manages interrupt-driven or high-frequency polling for inputs.
​NetworkManager: Dedicated handler for WiFi connectivity and OTA logic.
​Fahim-BotOS.ino: The main entry point managing the FreeRTOS task scheduler.
​📥 Installation & Setup
​Open Arduino IDE or ArduinoDroid (for mobile users).
​Install the following libraries via Library Manager:
​Adafruit SSD1306 & Adafruit GFX
​Adafruit MPU6050
​ArduinoJson
​Update your WiFi credentials (SSID and Password) in the source code.
​Select ESP32-C3 Dev Module and click Upload.
​🔄 Remote Firmware Updates (OTA)
​My Gadget BD is committed to continuous improvement. To receive new features:
​Ensure your robot is connected to WiFi.
​The OS will automatically check this repository for a new .bin file version.
​If an update is found, the robot will download and reboot into the new OS version.
​📄 License & Copyright
​Copyright © 2026 My Gadget BD. All Rights Reserved.
Commercial use, modification, or distribution of this OS is strictly prohibited without explicit permission from My Gadget BD.
​📞 Support & Contact
​For inquiries, bulk orders, or technical support:
​Company: My Gadget BD
​Founder: Fahim
​Location: Jashore, Bangladesh
