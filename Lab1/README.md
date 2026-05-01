# Microcontroller Lab 1 – GPIO, Keypad & BCD Display

## Overview
This project is part of Lab Session 1 for the Microcontroller course.  
The goal is to understand the interaction between **hardware and software** using GPIO on the **TM4C1294 microcontroller**.

The implementation includes:
- Keypad scanning (4x4 hexadecimal keypad)
- Detection of pressed keys
- Handling multiple key presses (error case)
- Displaying the result on a **7-segment (BCD) display**

---

## Hardware Used
- TM4C1294 (Tiva C Series) Microcontroller
- Hexadecimal Keypad (4x4 matrix)
- 7-segment BCD display

---

## Concepts Covered
- GPIO configuration (DIR, DEN, DATA registers)
- Peripheral register access
- Matrix keypad scanning
- Signal delay handling (debouncing concept)
- Binary Code Decimal (BCD) output

---

## How Keypad Scanning Works

The keypad is organized as a matrix:

- Columns (X1–X4) → outputs  
- Rows (Y1–Y4) → inputs  

Scanning process:
1. Set one column LOW (0), others HIGH (1)
2. Read row inputs
3. If a row becomes LOW → key is pressed
4. Repeat for all columns

This allows detecting 16 keys using only 8 pins.

---

## Features

### Single Key Detection
Each key is detected and printed using `printf()`.

### Multiple Key Detection
If more than one key is pressed: 
Error: multiple keys


### 🔁 One-Time Output
Each key is printed **only once per press** (no spamming).

### 🔢 BCD Display Output
Pressed key is converted to BCD and displayed:

| Key | BCD |
|-----|-----|
| 0–9 | 0000–1001 |
| A–F | 1010–1111 |

---

## 📂 Project Structure
Microcontrollers/
└── Lab1/
└── TM4C1294ncpdt_default_project/
├── main.c
├── inc/
├── targetConfigs/

---

## How to Run

1. Open project in Code Composer Studio (CCS)
2. Connect TM4C1294 board via USB
3. Start debug session (F5)
4. Run the program
5. Press keys on keypad and observe:
   - Console output (`printf`)
   - 7-segment display output

---

## ⚠️ Notes
- GPIO ports must be correctly configured based on hardware connections
- A small delay is required to allow signal stabilization
- BCD display uses only 4 data lines (D0–D3)

---

## Learning Outcome
After completing this lab, you should understand:
- How software controls hardware via registers
- How to configure and use GPIO
- How to interface input/output devices with a microcontroller

---

## Author
Nooshin Pourkamali