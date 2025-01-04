`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2025/01/03 14:15:30
// Design Name: 
// Module Name: SSDPmod
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module SSDPmod(
    input logic        clk,        // Clock signal
    input logic [6:0]  distance,       // Distance value (0-99) from ZYNQ
    output logic [7:0] ssd_pins    // SSD output pins (7:0 => GFEDCBA + digit select)
    );
    // Digit patterns for 0-9 (GFEDCBA format, active high)
    localparam logic [6:0] digitPatterns[0:9] = {
        7'b0111111,  // 0
        7'b0000110,  // 1
        7'b1011011,  // 2
        7'b1001111,  // 3
        7'b1100110,  // 4
        7'b1101101,  // 5
        7'b1111101,  // 6
        7'b0000111,  // 7
        7'b1111111,  // 8
        7'b1101111   // 9
    };
    // Internal registers
    logic [3:0] tens, ones;       // Tens and ones digits
    logic digit_select;           // Select between tens (0) and ones (1)
    logic [6:0] segment_data;     // Current segment data
    // Clock divider for multiplexing (adjust as needed)
    reg [19:0] clk_divider;       // Divides clock for multiplexing
    always_ff @(posedge clk) begin
        clk_divider <= clk_divider + 1;
    end
    assign digit_select = clk_divider[19]; // Use MSB for digit selection toggle

    // Split distance into tens and ones digits
    always_comb begin
        if (distance > 99) begin
            tens = 9;
            ones = 9;
        end else begin
            tens = distance / 10;
            ones = distance % 10;
        end
    end

    // Assign segment data based on digit select
    always_comb begin
        if (digit_select == 1) begin
            // Tens digit (left)
            segment_data = digitPatterns[tens];
        end else begin
            // Ones digit (right)
            segment_data = digitPatterns[ones];
        end
    end

    // Output logic: append digit select to segment data
    always_comb begin
        ssd_pins[6:0] = segment_data;    // GFEDCBA
        ssd_pins[7] = digit_select;      // Digit select (0 = tens, 1 = ones)
    end
endmodule
