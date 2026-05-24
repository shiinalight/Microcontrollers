# Lab 2 – Ultrasonic Sensor and LED Pendulum

This project was developed for the TM4C1294NCPDT microcontroller board as part of the Microcontrollers laboratory.

The goal of the lab was to combine timer-based signal generation, ultrasonic distance measurement, GPIO handling, and LED visualization into one embedded system prototype.

---

# Overview

The system performs the following tasks:

- Generates a rectangular trigger signal using Timer0
- Sends trigger pulses to the HC-SR04 ultrasonic sensor
- Measures the echo pulse duration
- Calculates the object distance from the sound travel time
- Detects pendulum movement using the L/R signal
- Displays the measured distance visually on the LED pendulum
- Verifies signals using an oscilloscope

---

# Hardware Used

- TM4C1294NCPDT LaunchPad
- HC-SR04 Ultrasonic Sensor
- LED Pendulum Module
- Oscilloscope
- Breadboard and jumper wires

---

# Pin Configuration

| Pin     | Function            |
| ------- | ------------------- |
| PD0     | Ultrasonic Trigger  |
| PL1     | Ultrasonic Echo     |
| PM0–PM7 | LED Outputs         |
| PL0     | Pendulum L/R Signal |

---

# Timer-Based Sleep Function

A blocking `sleep(ms)` function was implemented using Timer0 in one-shot mode.

The timer counts down internally and the CPU waits until the timeout flag is set.

```c
while((TIMER0_RIS_R & 0x01) == 0){}
```

This delay function was first verified by generating a rectangular signal on PD0 and observing it on the oscilloscope.

---

# Rectangular Trigger Signal

A rectangular signal was generated on PD0 to test the timer and GPIO functionality.

```text
HIGH → delay → LOW → delay → repeat
```

The waveform was verified using the oscilloscope.

---

# Ultrasonic Distance Measurement

The HC-SR04 ultrasonic sensor was used for distance measurement.

## Working Principle

1. A trigger pulse is sent to the sensor.
2. The sensor emits an ultrasonic sound wave.
3. The sound reflects from an object.
4. The Echo pin becomes HIGH while the wave travels.
5. Timer0 measures the Echo HIGH duration.
6. The measured time is converted to distance.

---

# Distance Formula

The distance is calculated using the speed of sound:

```text
distance = (time_us × 0.0343) / 2
```

where:

- `time_us` = echo duration in microseconds
- `0.0343 cm/us` = speed of sound
- division by 2 accounts for forward and return travel

---

# Example Output

```text
Echo Time = 3311 us, Distance = 56.78 cm
```

---

# Echo Signal Measurement

The Echo signal was measured using polling and Timer0.

The program waits for:

- rising edge → Echo HIGH
- falling edge → Echo LOW

The measured timer difference corresponds to the ultrasonic travel time.

---

# LED Pendulum Visualization

The LED pendulum was used to display the measured values visually.

The pendulum reversal points were detected using the L/R signal.

- Rising edge → left reversal point
- Falling edge → right reversal point

LED patterns were generated based on distance conditions.

Example:

- Close object → fewer LEDs
- Farther object → more LEDs

---

# Oscilloscope Verification

The oscilloscope was used throughout the lab to verify:

- rectangular trigger signal
- trigger pulse timing
- echo pulse duration
- signal stability

## Observed Signals

### Trigger Signal

```text
____|‾|________
```

### Echo Signal

```text
________|‾‾‾‾‾‾|______
```

The Echo pulse width increased as the object moved farther away.

---

# Challenges and Debugging

During development several issues were encountered and resolved:

- Incorrect GPIO direction configuration
- Timer configuration issues
- Infinite loops while waiting for Echo
- VS Code include-path warnings
- Oscilloscope timing scale mismatch
- AHB/non-AHB register inconsistencies

Timeout logic was added to avoid infinite waiting when no echo signal was detected.

---

# Project Structure

```text
Lab 2 ultrasonic sensor and LED pendulum/
├── main.c
├── README.md
├── oscilloscope.jpg
└── demo.mp4
```

---

# Image

Add your oscilloscope image here:

```markdown
![Oscilloscope Result](oscilloscope.jpg)
```

---

# Notes

This project uses the TM4C1294 header files provided by Code Composer Studio / TivaWare.

VS Code may display include-path warnings for:

```c
#include "inc/tm4c1294ncpdt.h"
```

even though the project builds and runs correctly in CCS.

---

# Learning Outcomes

This lab helped demonstrate:

- Timer programming
- GPIO configuration
- Polling techniques
- Signal timing measurement
- Ultrasonic sensing
- Oscilloscope debugging
- Embedded system integration

---

# Author

Nooshin Pourkamali
