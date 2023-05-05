# I2C Motor Driver
This firmware allow user send i2c command to drive 4 motor.

## I2C Address 
- I2C Slave Address: **0x30**

## I2C Command Table

- I2C Command have 3 bytes:
    | # | Param  | Range  |
    |---|---|---|
    |  0  | Motor Type  |  0 -> 3 | 
    |  1  | Motor Index  |  0 -> 3 | 
    |  2 |  Direction | 0 -> 1  | 
    |  3 |  Speed |0 -> 100    |

- Motor Type: DC_MOTOR (0x00) , STEP_MOTOR (0x01)
- Motor Index: DC_MOTOR (0->3) , STEP_MOTOR (0:1)
- Direction: FORWARD(0x00) , BACKWARD(0x01)
- Speed: DC_MOTOR(0:100) , STEP_MOTOR(0:255)
## Flash released firmware
- Using [Stm32 Programmer](https://www.st.com/en/development-tools/stm32cubeprog.html)
- Using St-flash in Linux
    ```
    st-flash write stm32f030-motor-driver.bin 0x08000000
    ```