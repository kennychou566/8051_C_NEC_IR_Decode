# 8051_C_NEC_IR_Decode
Use 8051 c language decode NEC IR format to 7 segment Display &amp; TX output

### Development environment:

Ubuntu 22.04 + Microsoft Vs Code + EIDE + SDCC Compiler.

### Development board:

![image](https://github.com/kennychou566/8051_C_NEC_IR_Decode/blob/main/board.jpg)

### program run status:

![image](https://github.com/kennychou566/8051_C_NEC_IR_Decode/blob/main/project.jpg)

### PIN Assignment:

#### P1 : 8 pcs LED堆疊顯示

#### P2.0~3 : 4 digits  7segments display switch

#### P0 : 4 digits  7segments display

#### P3.1 : TxD

#### P3.3 : IR Receiver

#### P3.6 : Buzzer 

### Porting method:

python ~/project/tools/stcgal.py -P stc89 ir_decode.hex 

### TX output baud rate: 9600 bps
