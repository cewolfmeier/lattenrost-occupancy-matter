# Lattenrost Presence Detection With Matter
This project is based on this [idea](https://community.simon42.com/t/meine-wohnung-weiss-wann-ich-im-bett-bin/10240).
It uses two MPU6050 accelerometers connected to an Arduino Nano Matter and detects bed occupancy by measuring angle changes in the slats and exposes occupancy state via the Matter smart home protocol over Thread.

## Components Used
- Arduino Nano Matter
- Two GY-87 modules with MPU6050 sensors

## Using The Right Data
![GY-87](/docs/GY_87_2.png)

## Sources
- https://community.simon42.com/t/meine-wohnung-weiss-wann-ich-im-bett-bin/10240
- https://docs.arduino.cc/tutorials/nano-matter/user-manual/
- Claude Code

**This post helped me while having trouble uploading a sketch to the board:**
- https://forum.arduino.cc/t/arduino-nano-matter-whats-the-well-matter/1407966/2 (Post #2)

**Calibration Sketch**
- https://wired.chillibasket.com/2015/01/calibrating-mpu6050/