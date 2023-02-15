# I2C Motor Driver
This firmware allow user send i2c command to drive 4 motor.

## I2C Address 
- I2C Slave Address: **0x30**

## I2C Command Table

- I2C Command have 3 bytes:
    | # | Param  | Range  |  Description  | 
    |---|---|---|---|
    |  0  | Motor Index  |  0 -> 3 | Motor index to controller |  
    |  1 |  Direction | 0 -> 1  | Direction of Motor  |  
    |  2 |  Speed |0 -> 100    | Duty Cycle | 