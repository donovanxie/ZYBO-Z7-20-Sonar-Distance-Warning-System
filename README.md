# ZYBO Z7-20 Sonar Distance Warning System
This project demonstrates a Sonar Distance Warning System implemented on the ZYBO Z7-20 FPGA development board. The system uses a Pmod MAXSONAR to measure distance, a Pmod OLED RGB display to display color-based distance warnings, and a Pmod SSD (Seven-Segment Display) to display distance values (up to 99).
## Features
- **Distance Measurement**: 
  - Uses Pmod MAXSONAR to measure distance values in real-time in inches.
- **Warning Display**:
  - Distance warnings are displayed as colors on the Pmod OLED RGB display:
    - **Red**: Distance <= 5 (blinking fast).
    - **Yellow**: Distance between 6 and 8 (blinking slower).
    - **Green**: Distance > 8 (constant display).
- **Seven-Segment Display (SSD)**:
  - Displays the distance value (0-99) using hardware logic.
  - Implements multiplexing to drive the two-digit display.
    
## Hardware Requirements
- **ZYBO Z7-20 FPGA Development Board**
- **Pmod MAXSONAR**: For distance measurement.
- **Pmod OLED RGB**: For warning color display.
- **Pmod SSD**: For numerical distance display.

- **Vivado**: Design, synthesis, and implementation of the hardware.
- **Vitis**: Application software development for the Zynq Processing System, please use Vitis version 2023.1 or before. The new Vitis is very buggy compiling this project.
  
## Project Structure
The project consists of the following components:

1. **Vivado Hardware Design**:
   - Contains block design for integrating the Zynq PS, Pmod peripherals, and custom hardware logic for PmodSSD in SystemVerilog (I have another version without SystemVerilog       PmodSSD hardware, purely using GPIO block to control everything).
   - Verilog/SystemVerilog modules for SSD control.

2. **Vitis Software Application**:
   - Distance measurement using the Pmod MAXSONAR.
   - Warning display logic for the Pmod OLED RGB.
   - SSD refresh logic for smooth display of numerical values.
     
## System Design
### Block Design
- Use block design in Vivado, using ZYNQ processor to connect all components.
- Add Digilent provided IP files to the block design(the Pmod Oled RGB Display and Pmod MAXSONAR)
- Connect Pmod MAXSONAR to Pmod Port JA, Pmod Oled RGB Display to Pmod port JB, and Pmod SSD Display to Pmod port JD and JE.
  
### SSD Logic
- The SSD display is driven by a custom SystemVerilog module (`SSDPmod`). It handles digit selection and segment control using a hardware clock divider for multiplexing.
### Vitis Application Logic
- Reads distance from the Pmod MAXSONAR.
- Updates the SSD and OLED RGB display in real-time based on distance thresholds.

## Getting Started
### Building the Hardware (Vivado)
1. Open the Vivado Project from [SonarProjectSV/](./SonarProjectSV/) folder and locate the .xpr file
2. Make your own adjustments as you wish.
3. Build and generate the bitstream (the .xsa file).

### Building the Software (Vitis)
1. Open the Vitis and build the platform project based on the .xsa file.
2. Locate the main.c file from [Sonar_SV_SSD_New/src/main.c](./Sonar_SV_SSD_New/src/main.c) and paste the C code into your Vitis.
3. Build and program the ZYBO Z7-20 board.
4. Have fun and make your own adjustments freely as you wish.
