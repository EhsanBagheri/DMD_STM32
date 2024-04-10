/*--------------------------------------------------------------------------------------
  Demo for RGB panels

  DMD_STM32a example code for STM32 and RP2040 boards

  The example shows the use of a double buffer for 
  simultaneous output of fixed and scrolling text.
 ------------------------------------------------------------------------------------- */
#include "DMD_RGB.h"

// Fonts includes
#include "st_fonts/UkrRusArial14.h"

//Number of panels in x and y axis
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1

// Enable of output buffering
// if true, changes only outputs to matrix after
// swapBuffers(true) command
// If dual buffer not enabled, all output draw at matrix directly
// and swapBuffers(true) cimmand do nothing
#define ENABLE_DUAL_BUFFER true
/* =================== *
      STM32F4 pins
   =================== */
#if (defined(__STM32F1__) || defined(__STM32F4__))
// ==== DMD_RGB pins ====
// mux pins - A, B, C... all mux pins must be selected from same port!
#define DMD_PIN_A PB6
#define DMD_PIN_B PB5
#define DMD_PIN_C PB4
#define DMD_PIN_D PB3
#define DMD_PIN_E PB8
// put all mux pins at list
uint8_t mux_list[] = { DMD_PIN_A , DMD_PIN_B , DMD_PIN_C , DMD_PIN_D , DMD_PIN_E };

// pin OE must be one of PB0 PB1 PA6 PA7
#define DMD_PIN_nOE PB0
#define DMD_PIN_SCLK PB7

// Pins for R0, G0, B0, R1, G1, B1 channels and for clock.
// By default the library uses RGB color order.
// If you need to change this - reorder the R0, G0, B0, R1, G1, B1 pins.
// All this pins also must be selected from same port!
uint8_t custom_rgbpins[] = {PA6, PA0, PA1, PA2, PA3, PA4, PA5 }; // CLK, R0, G0, B0, R1, G1, B1

/* =================== *
      RP2040 pins
   =================== */
#elif (defined(ARDUINO_ARCH_RP2040))
// ==== DMD_RGB pins ====
// mux pins - A, B, C... mux pins must be consecutive in ascending order
#define DMD_PIN_A 6
#define DMD_PIN_B 7
#define DMD_PIN_C 8
#define DMD_PIN_D 9
#define DMD_PIN_E 10
// put all mux pins at list
uint8_t mux_list[] = { DMD_PIN_A , DMD_PIN_B , DMD_PIN_C , DMD_PIN_D , DMD_PIN_E };

// pin OE must be one of PB0 PB1 PA6 PA7
#define DMD_PIN_nOE 15
#define DMD_PIN_SCLK 12

// Pins for R0, G0, B0, R1, G1, B1 channels and for clock.
// By default the library uses RGB color order.2
// If you need to change this - reorder the R0, G0, B0, R1, G1, B1 pins.
// All this pins also must be consecutive in ascending order
uint8_t custom_rgbpins[] = { 11, 0, 1, 2, 3, 4, 5 }; // CLK, R0, G0, B0, R1, G1, B1
#endif
// Fire up the DMD object as dmd<MATRIX_TYPE, COLOR_DEPTH>
// We use 64x32 matrix with 16 scans and 4bit color:
DMD_RGB <RGB64x32plainS16, COLOR_4BITS> dmd(mux_list, DMD_PIN_nOE, DMD_PIN_SCLK, custom_rgbpins, DISPLAYS_ACROSS, DISPLAYS_DOWN, ENABLE_DUAL_BUFFER);
// other options are:
// <RGB32x16plainS8> -  32x16 matrix with 8scans
// <RGB80x40plainS20> - 80x40 matrix with 20scans
// <RGB64x64plainS32> - 64x64 matrix with 32scans
// Color depth - <COLOR_4BITS_Packed>(STM32 only), <COLOR_4BITS> or <COLOR_1BITS>


// --- Define fonts ----
// DMD.h old style font
DMD_Standard_Font Arial_F(Arial_14);


void setup(void)
{
  // initialize DMD objects
  dmd.init();
  dmd.disableFastTextShift(true);
}


void loop(void)
{

  // create foreground colors
  uint16_t col[] = {
    dmd.Color888(255, 0, 0), // red
    dmd.Color888(0, 255, 0), // green
    dmd.Color888(0, 0, 255)  // blue

  };
  uint16_t bg = 0;  // background - black
  int col_cnt = 3;   // color count

  // select standard font
  dmd.selectFont(&Arial_F);

  // set text foreground and background colors
  dmd.setTextColor(col[0], bg);

  // text
  // We do not recode it when use standard fonts.
  // Transcode message to UTF coding needs for use with GFX fonts only.
  char t[] = "Привет Ардуино!";
  char s[] = "Hello Arduino!";


  // shift steps in pixels for running text (positive - shift right, negative - left)
  int8_t step[] = { 1, -1, -2, 2 };
  // running text shift interval
  uint16_t interval = 30;

  long prev_step = millis();
  uint8_t col_ptr = 0;
  uint8_t i = 0;
  uint8_t test = 255;
  uint8_t test_cnt = 4;
  dmd.setBrightness(200);

  // Cycle for tests:
  // -- running texts moving at x and y axis with single and double speed

  while (1) {
    if ((millis() - prev_step) > interval) {
      prev_step = millis();  // moved to the start of the loop to prevent "scroll not smooth" issue
      if (test >= test_cnt) {
        test = 0;
        // draw message
        dmd.drawMarqueeX(s, -1 * (dmd.stringWidth(s)), 0);
        dmd.swapBuffers(true);
      }
      switch (test) {
        // moving text at x axis
        case 0:
          if (dmd.stepMarquee(step[i], 0) & 1) { // if text is reached screen bounds

            dmd.clearScreen(true);
            // set next text color
            col_ptr++;
            if (col_ptr >= col_cnt) col_ptr = 0;
            dmd.setTextColor(col[col_ptr], bg);
            // set new text speed
            i++;
            // if all moving finished
            if (i > 3) { // return to start
              i = 0;
            }

            if (step[i] < 0) dmd.drawMarqueeX(t, dmd.width() - 1, 0);
            else dmd.drawMarqueeX(s, -1 * dmd.stringWidth(s), 0);
          }

          // renew fixed text
          dmd.drawFilledBox(0, 0, 15, dmd.height() - 1, bg);
          dmd.drawStringX(0, 0, "23", col[1]);

          // output mem buffer to matrix
          dmd.swapBuffers(true);
          break;

      }

     
    }
  }
}

