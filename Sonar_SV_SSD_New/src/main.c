#include "PmodOLEDrgb.h"
#include "PmodMAXSONAR.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xstatus.h"
#include "xil_printf.h"
#include <stdio.h>

/************************** Configuration Macros *****************************/
#define PMOD_MAXSONAR_BASEADDR  XPAR_PMODMAXSONAR_0_AXI_LITE_GPIO_BASEADDR
#define GPIO_SSD_ID             XPAR_GPIO_0_DEVICE_ID

#define SSD_CHANNEL  1

// If using MicroBlaze, get freq from xparameters; else assume 100 MHz
#ifdef __MICROBLAZE__
 #define CLK_FREQ XPAR_CPU_M_AXI_DP_FREQ_HZ
#else
 #define CLK_FREQ 100000000
#endif

/************************** Global Driver Instances *************************/
static PmodOLEDrgb  oledrgb;
static PmodMAXSONAR myDevice;
static XGpio        Gpio;  // For the SSD GPIO block

/************************** Function Prototypes ******************************/
static void DemoInitialize(void);
static void DemoRun(void);
static void DemoCleanup(void);
static void EnableCaches(void);
static void DisableCaches(void);

/* Helper function:
   UpdateDistanceOnSSD(dist) => sends distance to SSD GPIO */
static void UpdateDistanceOnSSD(u32 dist);

/* Helper function:
   DelayMs(ms) => delays execution for a given number of milliseconds */
static void DelayMs(u32 ms);

/******************************* main() **************************************/
int main(void)
{
   DemoInitialize();
   DemoRun();
   DemoCleanup();
   return 0;
}

/*************************** DemoInitialize() *******************************/
static void DemoInitialize(void)
{
   EnableCaches();

   // 1) Initialize the OLED
   OLEDrgb_begin(
       &oledrgb,
       XPAR_PMODOLEDRGB_0_AXI_LITE_GPIO_BASEADDR,
       XPAR_PMODOLEDRGB_0_AXI_LITE_SPI_BASEADDR);

   // 2) Initialize the MaxSonar
   MAXSONAR_begin(&myDevice, PMOD_MAXSONAR_BASEADDR, CLK_FREQ);

   // 3) Initialize the AXI GPIO for the SSD
   {
       XGpio_Config *cfgPtr = XGpio_LookupConfig(GPIO_SSD_ID);
       if (!cfgPtr) {
           xil_printf("XGpio_LookupConfig failed.\r\n");
           return;
       }
       int status = XGpio_CfgInitialize(&Gpio, cfgPtr, cfgPtr->BaseAddress);
       if (status != XST_SUCCESS) {
           xil_printf("XGpio_CfgInitialize failed.\r\n");
           return;
       }

       // Set GPIO direction as output
       XGpio_SetDataDirection(&Gpio, SSD_CHANNEL, 0x00);
   }
}

/**************************** DemoRun() *************************************/
static void DemoRun(void)
{
   u32 dist = 0;
   int blinkState = 0; // 0 = OFF, 1 = ON

   while (1)
   {
       // 1) Get distance
       dist = MAXSONAR_getDistance(&myDevice);
       if (dist > 99) dist = 99;

       // 2) Update OLED based on distance
       if (dist <= 5) {
           // Blink red (500 ms ON, 500 ms OFF)
           blinkState = !blinkState;
           if (blinkState) {
               OLEDrgb_DrawRectangle(&oledrgb, 0, 0, 95, 63, OLEDrgb_BuildRGB(255, 0, 0), 1, OLEDrgb_BuildRGB(255, 0, 0));
           } else {
               OLEDrgb_DrawRectangle(&oledrgb, 0, 0, 95, 63, OLEDrgb_BuildRGB(0, 0, 0), 1, OLEDrgb_BuildRGB(0, 0, 0));
           }
           DelayMs(500);
       }
       else if (dist <= 8) {
           // Blink yellow (1000 ms ON, 1000 ms OFF)
           blinkState = !blinkState;
           if (blinkState) {
               OLEDrgb_DrawRectangle(&oledrgb, 0, 0, 95, 63, OLEDrgb_BuildRGB(255, 255, 0), 1, OLEDrgb_BuildRGB(255, 255, 0));
           } else {
               OLEDrgb_DrawRectangle(&oledrgb, 0, 0, 95, 63, OLEDrgb_BuildRGB(0, 0, 0), 1, OLEDrgb_BuildRGB(0, 0, 0));
           }
           DelayMs(1000);
       }
       else {
           // Constant green (no blinking)
           OLEDrgb_DrawRectangle(&oledrgb, 0, 0, 95, 63, OLEDrgb_BuildRGB(0, 255, 0), 1, OLEDrgb_BuildRGB(0, 255, 0));
           DelayMs(1000); // Refresh every 1 second
       }

       // 3) Send distance value to SSD GPIO
       UpdateDistanceOnSSD(dist);
   }
}

/********************** UpdateDistanceOnSSD(dist) ***************************/
/**
 * Sends the distance value directly to the SSD GPIO block.
 */
static void UpdateDistanceOnSSD(u32 dist)
{
   XGpio_DiscreteWrite(&Gpio, SSD_CHANNEL, dist);
}

/***************************** DelayMs(ms) *********************************/
/**
 * Simple delay function that blocks for the given number of milliseconds.
 */
static void DelayMs(u32 ms)
{
   volatile u32 count = ms * 10000;
   while (count > 0) {
      count--;
   }
}

/*************************** DemoCleanup() **********************************/
static void DemoCleanup(void)
{
   DisableCaches();
}

/*********************** EnableCaches / DisableCaches ***********************/
static void EnableCaches(void)
{
#ifdef __MICROBLAZE__
 #ifdef XPAR_MICROBLAZE_USE_ICACHE
   Xil_ICacheEnable();
 #endif
 #ifdef XPAR_MICROBLAZE_USE_DCACHE
   Xil_DCacheEnable();
 #endif
#endif
}

static void DisableCaches(void)
{
#ifdef __MICROBLAZE__
 #ifdef XPAR_MICROBLAZE_USE_DCACHE
   Xil_DCacheDisable();
 #endif
 #ifdef XPAR_MICROBLAZE_USE_ICACHE
   Xil_ICacheDisable();
 #endif
#endif
}
