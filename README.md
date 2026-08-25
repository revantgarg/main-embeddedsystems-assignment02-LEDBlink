# LAB 2 — SWITCH-CONTROLLED LED & CYCLIC BLINKING

**Embedded Systems Laboratory • TM4C123GH6PM**

| Field | Details |
|---|---|
| **Name** | Revant Garg |
| **Registration No.** | 28581 |
| **Program** | M.Tech — ESE |

---

## 1. Objective

To configure the TM4C123GH6PM GPIO for switch and RGB LED interfacing and implement two programs.

**Program 1** uses the two switches to select the LED colour according to all four possible switch combinations.

**Program 2** provides user-controlled blinking, where SW1 selects the blinking rate and SW2 selects the LED colour cyclically.

---

## 2. Hardware & GPIO Configuration

GPIO Port F is enabled and configured for the onboard RGB LED and two push buttons.

PF1, PF2 and PF3 are configured as digital outputs for the Red, Blue and Green LED channels respectively.

PF4 (SW1) and PF0 (SW2) are configured as digital inputs with internal pull-up resistors.

PF0 is a protected pin and is therefore unlocked and committed before configuration.

Since the switches are active-low:

- A released switch reads logic `1`
- A pressed switch reads logic `0`

### GPIO Configuration

| Pin | Connected Device | Role |
|---|---|---|
| PF0 | SW2 | Input + pull-up |
| PF4 | SW1 | Input + pull-up |
| PF1 | Red LED | Output |
| PF2 | Blue LED | Output |
| PF3 | Green LED | Output |

---

## 3. Program 1 — Switch-Controlled RGB LED

The program continuously polls the Port F input register and isolates PF4 and PF0 using bit masks `0x10` and `0x01`.

The detected switch combination is then mapped to an RGB output.

The LED output codes are:

| LED Colour | Output Code |
|---|---|
| Red | `0x02` |
| Green | `0x08` |
| Blue | `0x04` |
| White | `0x0E` |

White is obtained by enabling all three RGB channels simultaneously.

### Switch Combination

| SW1 | SW2 | Input State | LED |
|---|---|---|---|
| Pressed | Not pressed | `0,1` | Blue |
| Not pressed | Pressed | `1,0` | Green |
| Pressed | Pressed | `0,0` | White |
| Not pressed | Not pressed | `1,1` | Red |

### Demonstration

[Part01](https://indianinstituteofscience-my.sharepoint.com/:v:/g/personal/revantgarg_iisc_ac_in/IQA8TEzvVGrqTLn7gGxJ46R-ATa8o938vnp0CwFPc6dxU1A?nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJPbmVEcml2ZUZvckJ1c2luZXNzIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXciLCJyZWZlcnJhbFZpZXciOiJNeUZpbGVzTGlua0NvcHkifX0&e=V1Bls9)

---

## 4. Program 2 — Switch-Controlled Blinking

The second program introduces two state variables, `rate` and `colour`.

The rate index selects one of three software delay values:

| Rate | Delay |
|---|---:|
| Slow | `1000` |
| Medium | `500` |
| Fast | `250` |

The colour index selects Red, Green, Blue or White.

SW1 increments the rate index and SW2 increments the colour index. When the last value is reached, the index wraps back to zero.

### Control

| Control | Cyclic Values | Function |
|---|---|---|
| SW1 | `1000 → 500 → 250 → 1000` | Blinking rate |
| SW2 | `R → G → B → W → R` | LED colour |

To avoid multiple changes from one long button press, the program stores `switch1Value` and `switch2Value` as the previous input states.

A selection changes only when the current input is `0` and the previous value was `1`, corresponding to a released-to-pressed transition.

The LED is driven **ON** for the selected delay, then turned **OFF** for the same delay. The switches are checked during both intervals so that a user input can be detected throughout the blink cycle.

### Demonstration

[Part02](https://indianinstituteofscience-my.sharepoint.com/:v:/g/personal/revantgarg_iisc_ac_in/IQCb3hO3nXfQR674ofMWruM4AWWJWojvqN1QewwiGGFZ3HE?nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJPbmVEcml2ZUZvckJ1c2luZXNzIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6IldlYiIsInJlZmVycmFsVmlldyI6Ik15RmlsZXNMaW5rQ29weSJ9fQ&e=7nHXID)

---

## 5. Results, Observation & Conclusion

Program 1 produces the specified **Red, Green, Blue and White** outputs for all four switch combinations.

Program 2 successfully changes the blinking speed using **SW1** and the LED colour using **SW2**, with both parameters cycling through their fixed sets.

The experiment demonstrates:

- GPIO clock and pin configuration
- Digital input/output operation
- Active-low switch logic
- Bit-mask based input detection
- RGB colour generation
- Software timing
- State variables
- Edge-based switch detection

The required switch-controlled LED functionality was successfully implemented on the **TM4C123GH6PM**.
