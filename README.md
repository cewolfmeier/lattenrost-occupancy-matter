# Lattenrost Occupancy Detection With Matter
> [!WARNING] 
> This project is at an early stage. It works but there is much room for improvement. Use with caution.

It is based on this [idea](https://community.simon42.com/t/meine-wohnung-weiss-wann-ich-im-bett-bin/10240).
One MPU6050 accelerometer per bed connected to an Arduino Nano Matter detects bed occupancy by measuring angle changes in the slats. The occupancy state is exposed via the Matter smart home protocol over Thread.

## Components Used
- Arduino Nano Matter
- Two GY-87 modules with MPU6050 sensors

## Using The Right Data
![GY-87](/docs/GY_87_2.png)
Have a look at the serial output for configuration.

## Sources
- https://community.simon42.com/t/meine-wohnung-weiss-wann-ich-im-bett-bin/10240
- https://docs.arduino.cc/tutorials/nano-matter/user-manual/
- Claude Code

**This post helped me while having trouble uploading a sketch to the board:**
- https://forum.arduino.cc/t/arduino-nano-matter-whats-the-well-matter/1407966/2 (Post #2)

**Calibration Sketch**
- https://wired.chillibasket.com/2015/01/calibrating-mpu6050/