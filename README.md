# LAB 2 — Switch-Controlled LED & Cyclic Blinking

**Embedded Systems Laboratory • TM4C123GH6PM**

---

## 👨‍💻 Student Information

| Field | Details |
|---|---|
| **Name** | Revant Garg |
| **Registration No.** | 28581 |
| **Program** | M.Tech — ESE |
| **Microcontroller** | TM4C123GH6PM |

---

# 1. Objective

The objective of this experiment is to configure the **GPIO Port F** of the **TM4C123GH6PM** microcontroller for interfacing with the onboard RGB LED and two push buttons, and to implement two switch-controlled programs.

### Program 1 — Switch-Controlled RGB LED

The two onboard switches are used to select the LED colour according to all four possible switch combinations.

### Program 2 — Switch-Controlled Cyclic Blinking

The two switches provide independent control over:

- **SW1:** Blinking rate
- **SW2:** LED colour

Both parameters cycle through their predefined values whenever the corresponding switch is pressed.

---

# 2. Hardware & GPIO Configuration

GPIO Port F is enabled and configured for the onboard RGB LED and two push buttons.

The following GPIO pins are used:

- **PF1** — Red LED
- **PF2** — Blue LED
- **PF3** — Green LED
- **PF4** — SW1
- **PF0** — SW2

PF1, PF2 and PF3 are configured as digital outputs for controlling the RGB LED channels.

PF4 and PF0 are configured as digital inputs with internal pull-up resistors for reading SW1 and SW2.

> **Important:** PF0 is a protected GPIO pin on the TM4C123GH6PM. Therefore, it must be unlocked and committed before being configured for use as a GPIO input.

Since the switches are **active-low**, their logic behaviour is:

| Switch Condition | GPIO Reading |
|---|---:|
| Released / Not Pressed | `1` |
| Pressed | `0` |

### GPIO Pin Configuration

| GPIO Pin | Connected Device | Configuration | Role |
|---|---|---|---|
| **PF0** | SW2 | Digital Input + Pull-up | Colour selection |
| **PF4** | SW1 | Digital Input + Pull-up | Blinking rate selection |
| **PF1** | Red LED | Digital Output | Red channel |
| **PF2** | Blue LED | Digital Output | Blue channel |
| **PF3** | Green LED | Digital Output | Green channel |

---

# 3. Program 1 — Switch-Controlled RGB LED

The first program continuously polls the **Port F input register** and isolates the states of SW1 and SW2 using bit masks.

The corresponding bit masks are:

```c
PF4 → 0x10
PF0 → 0x01
```

The switch states are then used to determine which colour should be displayed on the RGB LED.

## LED Output Codes

The RGB LED is controlled using the following output codes:

| Colour | Output Code |
|---|---:|
| Red | `0x02` |
| Blue | `0x04` |
| Green | `0x08` |
| White | `0x0E` |

White is obtained by enabling all three RGB LED channels simultaneously.

## Switch Combination Mapping

| SW1 | SW2 | Input State | LED Colour |
|---|---|---|---|
| Pressed | Not Pressed | `0, 1` | 🔵 **Blue** |
| Not Pressed | Pressed | `1, 0` | 🟢 **Green** |
| Pressed | Pressed | `0, 0` | ⚪ **White** |
| Not Pressed | Not Pressed | `1, 1` | 🔴 **Red** |

### Program Operation

The program continuously performs the following operations:

1. Read the GPIO Port F input register.
2. Isolate PF4 and PF0 using bit masks.
3. Determine the current switch combination.
4. Select the corresponding RGB LED output.
5. Write the selected colour to the GPIO Port F output register.
6. Repeat continuously.

### Demonstration

[▶️ View Program 1 Demonstration](https://indianinstituteofscience-my.sharepoint.com/:v:/g/personal/revantgarg_iisc_ac_in/IQA8TEzvVGrqTLn7gGxJ46R-ATa8o938vnp0CwFPc6dxU1A?nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJPbmVEcml2ZUZvckJ1c2luZXNzIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXciLCJyZWZlcnJhbFZpZXciOiJNeUZpbGVzTGlua0NvcHkifX0&e=V1Bls9)

---

# 4. Program 2 — Switch-Controlled Blinking

The second program introduces two state variables:

```c
int rate = 0;
int colour = 0;
```

The `rate` variable determines the blinking speed, while the `colour` variable determines the RGB LED colour.

SW1 is used to change the blinking rate, while SW2 is used to change the LED colour.

---

## 4.1 Blinking Rate Selection

Three software delay values are used:

| Rate Index | Delay Value | Speed |
|---:|---:|---|
| `0` | `1000` | Slow |
| `1` | `500` | Medium |
| `2` | `250` | Fast |

The rate cycles continuously as:

```text
1000 → 500 → 250 → 1000 → ...
```

Whenever SW1 is pressed, the rate index is incremented.

When the final rate is reached, the index wraps back to zero.

For example:

```text
Initial:
1000

SW1 Press:
500

SW1 Press:
250

SW1 Press:
1000

SW1 Press:
500

...
```

---

## 4.2 LED Colour Selection

The colour index selects one of four RGB LED colours:

```text
Red → Green → Blue → White → Red → ...
```

The colour sequence is:

| Colour Index | LED Colour |
|---:|---|
| `0` | Red |
| `1` | Green |
| `2` | Blue |
| `3` | White |

Whenever SW2 is pressed, the colour index is incremented.

After White, the colour index wraps back to Red.

---

## 4.3 Control Summary

| Switch | Cyclic Values | Function |
|---|---|---|
| **SW1** | `1000 → 500 → 250 → 1000` | Select blinking rate |
| **SW2** | `R → G → B → W → R` | Select LED colour |

Therefore, SW1 and SW2 operate independently.

For example, the user can select:

```text
Rate   = Fast
Colour = Blue
```

and the LED will continue blinking blue at the selected fast rate until another switch input changes the settings.

---

# 5. Switch Press Detection

Since the switches are **active-low**, a pressed switch produces a logic `0`.

Simply checking whether the switch is currently pressed could cause the corresponding parameter to change repeatedly if the user holds the button down.

To prevent this, the program stores the previous switch states:

```c
int switch1Value = 1;
int switch2Value = 1;
```

The program detects a switch press by comparing the current state with the previous state.

A valid button press occurs when:

```text
Previous state = 1
Current state  = 0
```

This represents a **released-to-pressed transition**.

Conceptually:

```c
if((currentState == 0) && (previousState == 1))
{
    // Switch has just been pressed
}
```

This method ensures that one long button press produces only **one change** in the selected rate or colour.

### Edge Detection Sequence

```text
Switch Released
      │
      │ GPIO = 1
      ▼
┌─────────────┐
│ Wait for    │
│ button press│
└─────────────┘
      │
      │ GPIO changes 1 → 0
      ▼
┌─────────────┐
│ Detect Press│
└─────────────┘
      │
      ▼
Update Rate / Colour
      │
      ▼
Previous State = Current State
```

The switches are checked during both the **LED ON** and **LED OFF** intervals so that user input can be detected throughout the blinking cycle.

---

# 6. Program 2 Operation

The overall operation of Program 2 can be represented as follows:

```text
             ┌───────────────┐
             │   Start       │
             └───────┬───────┘
                     │
                     ▼
          ┌─────────────────────┐
          │ Configure GPIO Port │
          │        F            │
          └──────────┬──────────┘
                     │
                     ▼
          ┌─────────────────────┐
          │ Initialize rate and │
          │ colour variables    │
          └──────────┬──────────┘
                     │
                     ▼
          ┌─────────────────────┐
          │ Read SW1 and SW2    │
          └──────────┬──────────┘
                     │
          ┌──────────┴──────────┐
          ▼                     ▼
     ┌───────────┐         ┌───────────┐
     │ SW1 Press?│         │ SW2 Press?│
     └─────┬─────┘         └─────┬─────┘
           │ Yes                 │ Yes
           ▼                     ▼
     Change Rate            Change Colour
           │                     │
           └──────────┬──────────┘
                      │
                      ▼
             ┌─────────────────┐
             │ Turn LED ON     │
             │ Selected Colour │
             └────────┬────────┘
                      │
                      ▼
               Delay = Rate
                      │
                      ▼
             ┌─────────────────┐
             │ Turn LED OFF    │
             └────────┬────────┘
                      │
                      ▼
               Delay = Rate
                      │
                      ▼
                Repeat Loop
```

---

# 7. Demonstration

### Program 1

[▶️ View Program 1 Video](https://indianinstituteofscience-my.sharepoint.com/:v:/g/personal/revantgarg_iisc_ac_in/IQA8TEzvVGrqTLn7gGxJ46R-ATa8o938vnp0CwFPc6dxU1A?nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJPbmVEcml2ZUZvckJ1c2luZXNzIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXciLCJyZWZlcnJhbFZpZXciOiJNeUZpbGVzTGlua0NvcHkifX0&e=V1Bls9)

### Program 2

[▶️ View Program 2 Video](https://indianinstituteofscience-my.sharepoint.com/:v:/g/personal/revantgarg_iisc_ac_in/IQCb3hO3nXfQR674ofMWruM4AWWJWojvqN1QewwiGGFZ3HE?nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJPbmVEcml2ZUZvckJ1c2luZXNzIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6IldlYiIsInJlZmVycmFsVmlldyI6Ik15RmlsZXNMaW5rQ29weSJ9fQ&e=7nHXID)

---

# 8. Results & Observations

## Program 1

The RGB LED successfully produced the specified output for all four possible combinations of SW1 and SW2.

| SW1 | SW2 | LED Output |
|---|---|---|
| `0` | `1` | 🔵 Blue |
| `1` | `0` | 🟢 Green |
| `0` | `0` | ⚪ White |
| `1` | `1` | 🔴 Red |

Thus, all four possible switch combinations were successfully implemented.

---

## Program 2

Program 2 successfully implemented independent cyclic control of the LED blinking rate and colour.

### SW1

The blinking rate cycles through:

```text
1000 → 500 → 250 → 1000 → ...
```

### SW2

The LED colour cycles through:

```text
Red → Green → Blue → White → Red → ...
```

The edge-based switch detection ensures that a single button press results in only one change.

The LED successfully blinks using the selected colour and rate.

---

# 9. Conclusion

The experiment successfully demonstrated GPIO configuration and switch-controlled RGB LED operation using the **TM4C123GH6PM** microcontroller.

The following concepts were implemented and verified:

- GPIO clock and pin configuration
- Digital input and output operation
- Active-low switch logic
- Internal pull-up resistors
- Bit-mask based input detection
- RGB LED colour generation
- Software-based timing and delays
- State variables
- Cyclic parameter selection
- Released-to-pressed edge detection
- Switch-controlled LED blinking

Program 1 successfully mapped all four possible switch combinations to the required RGB LED colours.

Program 2 successfully provided independent control of the blinking rate and LED colour using SW1 and SW2.

Overall, the required **switch-controlled LED and cyclic blinking functionality** was successfully implemented and tested on the **TM4C123GH6PM** development board.

---

# 10. Tools & Hardware

| Component | Description |
|---|---|
| **Microcontroller** | TM4C123GH6PM |
| **Development Board** | TM4C123G LaunchPad |
| **Programming Language** | C |
| **IDE** | Code Composer Studio |
| **GPIO Port** | Port F |
| **RGB LED** | Onboard RGB LED |
| **Switches** | SW1 and SW2 |

---

---

**Embedded Systems Laboratory**  
**Indian Institute of Science, Bengaluru**

**LAB 2 — Switch-Controlled LED & Cyclic Blinking**
