# Arduino Door Lock with Keypad
### By: Diego J. Graham

## Description

**Arduino Door Lock with Keypad** is a security system project that allows users to input a password through a 4x4 membrane keypad. Once the correct password is entered, the lock opens (via a relay), and the system provides audible feedback through a buzzer and displays the status on an LCD. If an incorrect password is entered three times, the system locks out and displays a "Break in Detected" message, requiring a reset.

---

## Features

- User input is gathered via a 4x4 membrane keypad.
- LCD display shows user input (hidden as `*` symbols) and status messages.
- Relay switches to open/close the lock.
- Buzzer provides feedback on button presses, errors, and access granted.
- The system allows for a 5-digit password and hides the user input for privacy.
- Incorrect password attempts are tracked, and after 3 failed attempts, the system locks out.
- "Break in Detected" message is shown, and the system must be reset.

---

## How It Works

1. The user enters a password on the keypad.
2. The password is displayed on the LCD as `*` symbols (or the actual password if visibility is toggled).
3. After the user presses `#` or `*` to submit the password:
   - If the password matches, the lock is opened, and a success message is displayed.
   - If the password is incorrect, an error message is shown, and the system allows another attempt.
4. After three incorrect attempts, the system locks out, displaying a "Break in Detected" message and requiring a reset.

---

## Required Libraries
- LiquidCrystal - handles the LCD display.
- Keypad - handles keypad input.

---

## Components Used

- Arduino MEGA (or compatible board)
- 4x4 Membrane Keypad
- 16x2 LCD Display
- Active Buzzer
- SRD-05VDC-SL-C Relay
- 10k ohm potentiometer
- Jumper Wires
- Breadboard

---

## Schematic

A complete schematic for wiring the components is included in the repository.

---

## Getting Started

1. Clone the repository.
2. Open `Keypad.ino` in the Arduino IDE.
3. Connect the components according to the schematic.
4. Upload the sketch to your Arduino board.
5. Use the keypad to enter the password and interact with the system.

---

## Notes

- The default password is `12345`. Change it in the `PASSWORD` array in the code for your own security.
- The system allows for a maximum of 3 incorrect password attempts before locking out.
- The relay pin and buzzer pin are set to pins `6` and `5` respectively.
- The keypad wiring should correspond to the `rowPins` and `colPins` in the code.
- The lock time duration is set to 5 seconds (modifiable in the `LOCKTIME` variable).

---

## Future Improvements

- Add support for password reset functionality.
- Implement a logging system to track user access attempts.
- Improve the security of the system by supporting more complex passwords.

---

## License

This project is open source and free to use with credit.

---
