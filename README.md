# Smart People Detection System

## 📌 About the Project

This project is a simple **people/object detection system** built using an **ESP32** and an **HC-SR04 ultrasonic sensor**.

The idea is pretty straightforward: the ultrasonic sensor measures how far an object or person is from the sensor. Based on the distance, the ESP32 calculates how close the object is and uses an LED to indicate the proximity.

The distance threshold can also be entered through the **Serial Monitor**, making it easy to change the detection range without modifying the code.

## 🛠️ Components Used

- ESP32 Development Board
- HC-SR04 Ultrasonic Sensor
- LED
- 220Ω Resistor
- 1kΩ Resistor
- 2kΩ Resistor
- Breadboard
- Jumper Wires
- USB Cable

## 🔌 Connections

| Component | ESP32 Pin |
|---|---|
| HC-SR04 TRIG | GPIO 14 |
| HC-SR04 ECHO | GPIO 12 |
| LED | GPIO 27 |
| HC-SR04 VCC | VIN / 5V |
| HC-SR04 GND | GND |

### ⚠️ Echo Pin Protection

The HC-SR04 normally outputs around **5V** from its ECHO pin, while the ESP32 GPIO pins are designed for **3.3V**.

So, a simple voltage divider is used before connecting ECHO to GPIO 12:

- **1kΩ resistor** between ECHO and the GPIO 12 junction
- **2kΩ resistor** between the GPIO 12 junction and GND

This brings the voltage down to a safer level for the ESP32.

## ⚙️ How It Works

The system works in a simple cycle:

1. The HC-SR04 sends out an ultrasonic pulse.
2. The pulse reflects back after hitting a nearby object.
3. The sensor measures the time taken for the echo to return.
4. The ESP32 uses this time to calculate the distance.
5. The distance is converted into a proximity percentage.
6. The LED indicates how close the detected object is.
7. The measurement is repeated every **10 seconds**.

## 📊 Proximity Calculation

The proximity is calculated using:

```text
Proximity (%) = ((Threshold - Distance) / Threshold) × 100
```

The final value is limited between **0% and 100%**.

For example, if the threshold is 100 cm and an object is detected at 50 cm:

```text
((100 - 50) / 100) × 100 = 50%
```

So the detected proximity would be **50%**.

## 🎯 Purpose

This project was built as a simple way to understand how **ultrasonic sensing, ESP32 GPIO control, distance measurement, and basic proximity detection** work together.

It can also serve as a starting point for larger projects involving object detection, smart environments, automation, or assistive systems.

## 📚 Note

This project was developed for **educational and academic purposes**.
