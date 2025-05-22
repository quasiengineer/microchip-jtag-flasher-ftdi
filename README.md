# microchip-jtag-flasher-ftdi

Tool to upload firmware for Microchip/Microsemi FPGAs by using FTDI-based JTAG probe.

Supported FPGA families:
  - Microsemi PolarFire
  - RTG4
  - IGLOO2
  - SmartFusion2
  - ProASIC (including ProASIC3 nano)
  - IGLOO (including IGLOO nano)
  - SmartFusion
  - Fusion

Devices: A3PE600, A3PE1500, A3PE3000, A3P015, A3P030, A3P060, A3P125, A3P250, A3P400, A3P600, A3P1000, AFS090, AFS250, AFS600, AFS1500, A2F200M3F, A2F500M3G, M2GL010, M2GL025, M2GL050, M2S005, M2S010, M2S025, M2S050, RT4G150, MPF300

## Compilation

You need to have `meson` to compile that project from source code:
```
meson setup build
meson compile -C build
```

## Usage

Connect FTDI board to JTAG pins for FPGA:
  * TCK - D0
  * TDI - D1
  * TDO - D2
  * TMS - D3
  * TRST - D7

And then just run compiled binary:

```
./microchip-jtag-flasher PATH_TO_BITSTREAM_DAT
```