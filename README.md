# Battle Bot (RC Car)

This is a mini Project that, me and my friends had made in college 2nd Year.

## Project Overview
This is a **battle bot (RC car)** designed for high-speed maneuverability and combat-style performance. The project began with **3D modeling in Blender**, followed by real-world fabrication using workshop tools. The bot is controlled via **WiFi using ESP32 and the Blynk IoT app**. A key constraint was keeping the total weight **under 4kg** and **450mm x 450mm x 450mm**, ensuring optimal speed and efficiency.

## Demo
*()*

## Features
- **Joystick Control** for smooth movement via **Blynk IoT App**
- **Dedicated Weapon Motor Control** via a slider
- **WiFi Connectivity** for seamless communication
- **PWM-Based Speed Control**
- **Optimized Weight (Under 4kg)** for better performance

## Components Used
### **Hardware:**
- **Motors:** Pro-Range **12V 300 RPM Johnson Geared DC Motor** (Grade A, Encoder Compatible)
- **Motor Drivers:** Double **BTS7960 43A H-Bridge High-Power Stepper Motor Driver Module**
- **Microcontroller:** **ESP32**
- **Couplings:** **6mm Hex coupling for Robot Smart Car Wheel** (18mm Length)
- **Wheels:** **85MM Large Robot Smart Car Wheel, 38MM Width Surface (Blue)**
- **Power Supply:** Pro-Range **INR 21700 P42A 11.1V 4200mAh 3S1P Li-ion Drone Battery Pack (Triangle)**
- **Buck Converter:** (Chosen based on battery rating)
- **Miscellaneous:** GI Sheet (for body), Connectors, Chassis parts, etc.
- **Extra-Materials:** 26 Gauge wire for logic connections,16 Gauge wire for motor and battery connections and female header pins for ESP32, Switch for ON/OFF.

### **Software & Tools:**
- **Blender** (For 3D modeling and design validation)
- **Blynk** (For WiFi-based remote control)
- **Arduino IDE** (For ESP32 programming)

## Circuit Diagram
*()*

## Fabrication & Assembly
1. **3D Modeling:** Initial design and validation done in **Blender**.
2. **Material Cutting & Assembly:** Using **workshop tools**, components were assembled into the final RC bot.
3. **Electronics Integration:** Connecting **ESP32**, **motors**, **motor drivers**, and **battery**.
4. **Software Setup:** Configuring **Blynk app**, uploading ESP32 firmware.

## Installation & Setup
1. Clone this repository:
2. Open the code in **Arduino IDE**.
3. Install the required **ESP32 board support** and necessary libraries.
4. Upload the code to your **ESP32**.
5. Connect the bot to the **Blynk IoT App**.
6. Control your battle bot using the joystick and buttons!
7. And library we are using for smooth operation of motor is Ledcsetup(), So your esp32 board libray version will be    2.0.17 (just go into the Arduino IDE’s board manager → esp32 → pick version “2.0.17” and press update.)

## How to Use
1. Open the **Blynk App**.
2. Connect to **ESP32 via WiFi**.
3. Use the **joystick for movement**.
4. Use **buttons for motor control**.
5. Adjust the **weapon motor speed** using the slider.
6. Enjoy your battle bot in action!

## Code Breakdown
*()*

## 🤝 Contributors
- **Dheerendra-Amrute**
- **Abhinav-Anand2023**
- 
