#include <Adafruit_Protomatter.h>
#include <time.h>

#define MATRIX_WIDTH 64
#define MATRIX_HEIGHT 32

// Pin configurations M0
uint8_t rgbPins[]  = {2, 3, 4, 5, 6, 7};
uint8_t addrPins[] = {A0, A1, A2, A3};
uint8_t clockPin   = 8;
uint8_t latchPin   = 10;
uint8_t oePin      = 9;

Adafruit_Protomatter matrix(
  MATRIX_WIDTH, 4, 1, rgbPins, 4, addrPins,
  clockPin, latchPin, oePin, false, 1
);

// Binary Font
const uint8_t number1[26][10] = {
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,0},
  {0,0,1,1,1,1,0,0,0,0},
  {0,1,1,1,1,1,0,0,0,0},
  {1,1,1,0,1,1,0,0,0,0},
  {1,1,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1}
};

const uint8_t number2[26][10] = {
{0,0,1,1,1,1,1,1,0,0},
{0,1,1,1,1,1,1,1,1,0},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,1,1,0},
{0,0,0,0,0,0,0,1,1,0},
{0,0,0,0,0,0,1,1,0,0},
{0,0,0,0,0,0,1,1,0,0},
{0,0,0,0,0,1,1,0,0,0},
{0,0,0,0,0,1,1,0,0,0},
{0,0,0,0,1,1,0,0,0,0},
{0,0,0,0,1,1,0,0,0,0},
{0,0,0,1,1,0,0,0,0,0},
{0,0,0,1,1,0,0,0,0,0},
{0,0,1,1,0,0,0,0,0,0},
{0,0,1,1,0,0,0,0,0,0},
{0,1,1,0,0,0,0,0,0,0},
{0,1,1,0,0,0,0,0,0,0},
{1,1,0,0,0,0,0,0,0,0},
{1,1,0,0,0,0,0,0,0,0},
{1,1,0,0,0,0,0,0,0,0},
{1,1,0,0,0,0,0,0,0,0},
{1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1}
};

const uint8_t number3[26][10] ={
{0,0,1,1,1,1,1,1,0,0},
{0,1,1,1,1,1,1,1,1,0},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,1,1,0},
{0,0,0,1,1,1,1,1,0,0},
{0,0,0,1,1,1,1,1,0,0},
{0,0,0,0,0,0,0,1,1,0},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{0,1,1,1,1,1,1,1,1,0},
{0,0,1,1,1,1,1,1,0,0}
};

const uint8_t number4[26][10] = {
{0,0,0,0,0,0,1,1,0,0},
{0,0,0,0,0,0,1,1,0,0},
{0,0,0,0,0,1,1,1,0,0},
{0,0,0,0,0,1,1,1,0,0},
{0,0,0,0,1,1,1,1,0,0},
{0,0,0,0,1,1,1,1,0,0},
{0,0,0,0,1,1,1,1,0,0},
{0,0,0,1,1,0,1,1,0,0},
{0,0,0,1,1,0,1,1,0,0},
{0,0,1,1,0,0,1,1,0,0},
{0,0,1,1,0,0,1,1,0,0},
{0,1,1,0,0,0,1,1,0,0},
{0,1,1,0,0,0,1,1,0,0},
{1,1,0,0,0,0,1,1,0,0},
{1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1},
{0,0,0,0,0,0,1,1,0,0},
{0,0,0,0,0,0,1,1,0,0},
{0,0,0,0,0,0,1,1,0,0},
{0,0,0,0,0,0,1,1,0,0},
{0,0,0,0,0,0,1,1,0,0},
{0,0,0,0,0,0,1,1,0,0},
{0,0,0,0,0,0,1,1,0,0},
{0,0,0,0,0,0,1,1,0,0},
{0,0,0,0,0,0,1,1,0,0},
{0,0,0,0,0,0,1,1,0,0}
};

const uint8_t number5[26][10] ={
{1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,0,0},
{1,1,0,0,0,0,0,0,0,0},
{1,1,0,0,0,0,0,0,0,0},
{1,1,0,0,0,0,0,0,0,0},
{1,1,0,0,0,0,0,0,0,0},
{1,1,0,0,0,0,0,0,0,0},
{1,1,1,1,1,1,1,1,0,0},
{1,1,1,1,1,1,1,1,1,0},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{0,1,1,1,1,1,1,1,1,0},
{0,0,1,1,1,1,1,1,0,0}};

const uint8_t number6[26][10] =
{
{0,0,1,1,1,1,1,1,0,0},
{0,1,1,1,1,1,1,1,1,0},
{0,1,1,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,0,0},
{0,1,1,0,0,0,0,0,0,0},
{0,1,1,0,0,0,0,0,0,0},
{0,1,1,0,0,0,0,0,0,0},
{0,1,1,0,0,0,0,0,0,0},
{0,1,1,0,1,1,1,1,0,0},
{0,1,1,1,1,1,1,1,1,0},
{0,1,1,1,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1},
{0,1,1,1,1,1,1,1,1,0},
{0,0,1,1,1,1,1,1,0,0}
};

const uint8_t number7[26][10] =
{
{1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,1,1,1},
{0,0,0,0,0,0,0,1,1,0},
{0,0,0,0,0,0,1,1,0,0},
{0,0,0,0,0,0,1,1,0,0},
{0,0,0,0,0,1,1,0,0,0},
{0,0,0,0,0,1,1,0,0,0},
{0,0,0,0,1,1,0,0,0,0},
{0,0,0,0,1,1,0,0,0,0},
{0,0,0,0,1,1,0,0,0,0},
{0,0,0,0,1,1,0,0,0,0},
{0,0,0,0,1,1,0,0,0,0},
{0,0,0,0,1,1,0,0,0,0},
{0,0,0,0,1,1,0,0,0,0},
{0,0,0,0,1,1,0,0,0,0},
{0,0,0,0,1,1,0,0,0,0},
{0,0,0,0,1,1,0,0,0,0},
{0,0,0,0,1,1,0,0,0,0},
{0,0,0,0,1,1,0,0,0,0},
{0,0,0,0,1,1,0,0,0,0}};
const uint8_t number8[26][10] =
{
{0,0,1,1,1,1,1,1,0,0},
{0,1,1,1,1,1,1,1,1,0},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{0,1,1,0,0,0,0,1,1,0},
{0,0,1,1,1,1,1,1,0,0},
{0,0,1,1,1,1,1,1,0,0},
{0,1,1,0,0,0,0,1,1,0},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{0,1,1,1,1,1,1,1,1,0},
{0,0,1,1,1,1,1,1,0,0}};
const uint8_t number9[26][10] =
{
{0,0,1,1,1,1,1,1,0,0},
{0,1,1,1,1,1,1,1,1,0},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,1,1,1},
{0,1,1,1,1,1,1,1,1,1},
{0,0,1,1,1,1,1,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{0,0,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{0,1,1,1,1,1,1,1,1,0},
{0,0,1,1,1,1,1,1,0,0}};

const uint8_t number0[26][10] =
{
{0,0,1,1,1,1,1,1,0,0},
{0,1,1,1,1,1,1,1,1,0},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{0,1,1,1,1,1,1,1,1,0},
{0,0,1,1,1,1,1,1,0,0}}; 

const uint8_t (*numbers[10])[26][10] = {&number0, &number1, &number2, &number3, &number4, &number5, &number6, &number7, &number8, &number9};

void setupRTC() {
  // Enable the XOSC32K oscillator
  SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_ENABLE | SYSCTRL_XOSC32K_EN32K | SYSCTRL_XOSC32K_XTALEN;
  // Wait for the XOSC32K oscillator to be ready
  while (!(SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_XOSC32KRDY));
  
  // Set Generic Clock Generator 2 GCLK2 to use XOSC32K
  GCLK->GENDIV.reg = GCLK_GENDIV_ID(2) | GCLK_GENDIV_DIV(1);
  // Enable GCLK2
  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(2) | GCLK_GENCTRL_SRC_XOSC32K | GCLK_GENCTRL_GENEN;
  // Wait for GCLK2 to be ready
  while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
  
  // Connect GCLK2 to RTC
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_RTC | GCLK_CLKCTRL_GEN_GCLK2 | GCLK_CLKCTRL_CLKEN;
  while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
  
  // Configure RTC
  RTC->MODE0.CTRL.reg = RTC_MODE0_CTRL_SWRST;
  // Wait for RTC to be ready
  while (RTC->MODE0.STATUS.bit.SYNCBUSY);
  
  // Set the counter to increment every second (32.768 kHz / 32768 = 1 Hz)
  // The prescaler is 1024, so the counter will increment every 1024 * 32 = 32768 ticks, 32 ticks per second
  RTC->MODE0.CTRL.reg = RTC_MODE0_CTRL_MODE_COUNT32 | RTC_MODE0_CTRL_PRESCALER_DIV1024 | RTC_MODE0_CTRL_MATCHCLR;
  // Wait for RTC to be ready
  while (RTC->MODE0.STATUS.bit.SYNCBUSY);
  
  // Set the counter to 0
  RTC->MODE0.COUNT.reg = 0;
  // Wait for RTC to be ready
  while (RTC->MODE0.STATUS.bit.SYNCBUSY);
  
  // Enable RTC
  RTC->MODE0.CTRL.bit.ENABLE = 1;
  // Wait for RTC to be ready
  while (RTC->MODE0.STATUS.bit.SYNCBUSY);
}

// Initialize RTC
void initRTC() {
  // Reset RTC
  RTC->MODE0.CTRL.reg = RTC_MODE0_CTRL_SWRST;
  // Wait for RTC to be ready
  while (RTC->MODE0.STATUS.bit.SYNCBUSY);
  
  // Set the counter to increment every second (32.768 kHz / 32768 = 1 Hz)
  RTC->MODE0.CTRL.reg = RTC_MODE0_CTRL_MODE_COUNT32 | RTC_MODE0_CTRL_PRESCALER_DIV1024 | RTC_MODE0_CTRL_MATCHCLR;
  // Wait for RTC to be ready
  while (RTC->MODE0.STATUS.bit.SYNCBUSY);
  
  // Set the compare register to 0xFFFFFFFF
  RTC->MODE0.COMP[0].reg = 0xFFFFFFFF;
  // Wait for RTC to be ready
  while (RTC->MODE0.STATUS.bit.SYNCBUSY);
  
  // Set the counter to 0
  RTC->MODE0.COUNT.reg = 0;
  // Wait for RTC to be ready
  while (RTC->MODE0.STATUS.bit.SYNCBUSY);
  
  // Enable RTC
  RTC->MODE0.CTRL.bit.ENABLE = 1;
  // Wait for RTC to be ready
  while (RTC->MODE0.STATUS.bit.SYNCBUSY);
}

// Set the time in RTC
void RTC_SetTime(int hour, int minute, int second) {
  // Calculate total seconds
  uint32_t totalSeconds = hour * 3600 + minute * 60 + second;
  // Multiply by 32 to account for 32Hz count rate per second
  uint32_t count = totalSeconds * 32; 
  // Wait for RTC to be ready
  while (RTC->MODE0.STATUS.bit.SYNCBUSY);
  // Set the counter to the calculated count
  RTC->MODE0.COUNT.reg = count;
  // Wait for RTC to be ready
  while (RTC->MODE0.STATUS.bit.SYNCBUSY);
}

// Helper function
String getFormattedTime() {
  // Read the current count
  RTC->MODE0.READREQ.reg = RTC_READREQ_RREQ;
  while (RTC->MODE0.STATUS.bit.SYNCBUSY);
  uint32_t count = RTC->MODE0.COUNT.reg;

  // Convert count to seconds (accounting for 32Hz count rate)
  uint32_t totalSeconds = count / 32;
  // Reset at midnight
  totalSeconds %= 86400;

  // Convert to hours, minutes, seconds
  int hours = (totalSeconds / 3600) % 24;
  int minutes = (totalSeconds % 3600) / 60;
  int seconds = totalSeconds % 60;
  
  // Format the time as HH:MM:SS
  char buffer[9];
  snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
  return String(buffer);
}
// Draw a number based on the font
void drawNumber(int xOffset, int yOffset,const uint8_t pattern[26][10], uint16_t color) {
    for (int y = 0; y < 26; y++) {
        for (int x = 0; x < 10; x++) {
            if (pattern[y][x]) {
                matrix.drawPixel(xOffset + x,yOffset + y,color);
            }
        }
    }
}
void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  setupRTC();
  initRTC();
  
  // Set time manually
  RTC_SetTime(23, 59, 30);
  
  Serial.println("RTC initialized and set.");
  // Initialize the matrix
  ProtomatterStatus status = matrix.begin();
  if (status != PROTOMATTER_OK) {
      for (;;); // Handle error
  }

}

void loop() {
  // Static variable to store the last seconds
  static uint32_t lastSeconds = 0;

  // Read the current count
  RTC->MODE0.READREQ.reg = RTC_READREQ_RREQ;
  while (RTC->MODE0.STATUS.bit.SYNCBUSY);
  uint32_t currentCount = RTC->MODE0.COUNT.reg;

  // Convert count to seconds (accounting for 32Hz count rate)
  uint32_t currentSeconds = (currentCount / 32);
  currentSeconds %= 86400;
  // Check if the seconds have changed
  if (currentSeconds != lastSeconds) {
    char currentStr[6];
    char lastStr[6];
    // Convert current and last seconds to strings
    snprintf(currentStr, sizeof(currentStr), "%05lu", currentSeconds);
    snprintf(lastStr, sizeof(lastStr), "%05lu", lastSeconds);

    // Set the color for the digits
    uint16_t redColor = matrix.color565(255, 0, 0);
    uint16_t blackColor = matrix.color565(0, 0, 0);

    // Update only the digits that have changed
    for (int i = 0; i < 5; i++) {
      if (currentStr[i] != lastStr[i]) {
        // Clear the old digit
        drawNumber(3 + i * 12, 3, (*numbers[lastStr[i] - '0']), blackColor);
        // Draw the new digit
        drawNumber(3 + i * 12, 3, (*numbers[currentStr[i] - '0']), redColor);
      }
    }

    // Update the display
    matrix.show();

    // Update the last seconds
    lastSeconds = currentSeconds;
  }
}