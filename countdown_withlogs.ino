#include <Adafruit_Protomatter.h>
#include <time.h>
#include <RTClib.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SPIFlash.h>

// Configuration for Metro M0 Express
#if defined(__SAMD51__) || defined(NRF52840_XXAA)
  Adafruit_FlashTransport_QSPI flashTransport(PIN_QSPI_SCK, PIN_QSPI_CS, PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3);
#else
  #if (SPI_INTERFACES_COUNT == 1)
    Adafruit_FlashTransport_SPI flashTransport(SS, &SPI);
  #else
    Adafruit_FlashTransport_SPI flashTransport(SS1, &SPI1);
  #endif
#endif

Adafruit_SPIFlash flash(&flashTransport);
FatVolume fatfs;

File32 dataFile;
const char* dataFileName = "/data_log.txt";

#define MATRIX_WIDTH 64
#define MATRIX_HEIGHT 32↓


// GPIO pins
#define GPIO_EN      11
#define GPIO_IRQ     2  // We'll keep this for compatibility, but won't use it for interrupts

// I2C slave address
#define ES100_SLAVE_ADDR 0x32

// ES100 API register addresses
#define ES100_CONTROL0_REG       0x00
#define ES100_IRQ_STATUS_REG     0x02
#define ES100_STATUS0_REG        0x03
#define ES100_YEAR_REG           0x04
#define ES100_MONTH_REG          0x05
#define ES100_DAY_REG            0x06
#define ES100_HOUR_REG           0x07
#define ES100_MINUTE_REG         0x08
#define ES100_SECOND_REG         0x09

// Array to store date and time
#define DT_STATUS  0
#define DT_YEAR    1
#define DT_MONTH   2
#define DT_DAY     3
#define DT_HOUR    4
#define DT_MINUTE  5
#define DT_SECOND  6
#define DT_LENGTH  7
// Pin configurations M0
uint8_t rgbPins[]  = {2, 3, 4, 5, 6, 7};
uint8_t addrPins[] = {A0, A1, A2, A3};
uint8_t clockPin   = 8;
uint8_t latchPin   = 10;
uint8_t oePin      = 9;

Adafruit_Protomatter matrix(
  MATRIX_WIDTH, 2, 1, rgbPins, 4, addrPins,
  clockPin, latchPin, oePin, true, 1
);
bool es100Waiting = false; // Flag to indicate if we are waiting for ES100 response
unsigned long es100Timeout = 0; // Timeout for ES100 response
unsigned long lastUpdateTime = 0;
unsigned long countdownSeconds = 0;
bool successful_es100_adjust = false; // Flag to indicate successful ES100 adjustment
unsigned long lastSuccessfulAdjustTime = 0; // Timestamp of the last successful adjustment

RTC_DS3231 rtc; // Create an RTC object
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
{1,1,0,0,0,0,0,0,1,1},
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
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
{1,1,0,0,0,0,0,0,1,1},
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
{0,0,0,1,1,1,1,1,0,0},
{0,0,1,1,1,1,1,1,1,0},
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
{0,0,1,1,1,1,1,1,1,0},
{0,0,0,1,1,1,1,1,0,0}
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

  Wire.begin();
  pinMode(GPIO_EN, OUTPUT);
  // Initialize flash library and check its chip ID.
  if (!flash.begin()) {
    Serial.println("Error, failed to initialize flash chip!");
        while(1);

  }
  
  // Mount the filesystem
  if (!fatfs.begin(&flash)) {
    Serial.println("Error, failed to mount filesystem!");
        while(1);

  }
  logData("ES100 Time Reception Test");

   // initializing the rtc
    if(!rtc.begin()) {
        logData("Couldn't find RTC!");
        Serial.flush();
        while (1) delay(10);
    }

        // this will adjust to the date and time at compilation
        if(rtc.lostPower()){
      // Prompt user for the current time
          Serial.println("Please enter the current time:");
          Serial.print("Year (e.g., 2023): ");
          while (!Serial.available()); // Wait for input
          int year = Serial.parseInt();
          Serial.read(); // Clear the newline character

          Serial.print("Month (1-12): ");
          while (!Serial.available()); // Wait for input
          int month = Serial.parseInt();
          Serial.read(); // Clear the newline character

          Serial.print("Day (1-31): ");
          while (!Serial.available()); // Wait for input
          int day = Serial.parseInt();
          Serial.read(); // Clear the newline character

          Serial.print("Hour (0-23): ");
          while (!Serial.available()); // Wait for input
          int hour = Serial.parseInt();
          Serial.read(); // Clear the newline character

          Serial.print("Minute (0-59): ");
          while (!Serial.available()); // Wait for input
          int minute = Serial.parseInt();
          Serial.read(); // Clear the newline character

          Serial.print("Second (0-59): ");
          while (!Serial.available()); // Wait for input
          int second = Serial.parseInt();
          Serial.read(); // Clear the newline character

          // Set the RTC time to the current time at runtime
          DateTime runtime = DateTime(year, month, day, hour, minute, second);
          rtc.adjust(runtime);
        }
        // rtc.adjust(DateTime(2024,10,18,00,00,06));

  // Print the set time
  DateTime now = rtc.now(); // Get the current time from the RTC
  countdownSeconds = now.hour() * 3600 + now.minute() * 60 + now.second();

  updateDisplay(countdownSeconds, (countdownSeconds + 11111) % 100000);
  logData(String("RTC initialized and set") + countdownSeconds);

  // Initialize the matrix
  ProtomatterStatus status = matrix.begin();
  if (status != PROTOMATTER_OK) {
      for (;;); // Handle error
  }
}

void es100_enable() {
  digitalWrite(GPIO_EN, HIGH);
}

void es100_disable() {
  digitalWrite(GPIO_EN, LOW);
}

// Function to write to a register
bool writeRegister(uint8_t addr, uint8_t data) {
    Wire.beginTransmission(ES100_SLAVE_ADDR);
    Wire.write(addr);
    Wire.write(data);
    return (Wire.endTransmission() == 0); // Return true if successful
}

// Function to read from a register
bool readRegister(uint8_t addr, uint8_t &data) {
    Wire.beginTransmission(ES100_SLAVE_ADDR);
    Wire.write(addr);
    if (Wire.endTransmission() != 0) {
        return false; // Return false if failed to communicate
    }
    Wire.requestFrom(ES100_SLAVE_ADDR, 1);
    if (Wire.available()) {
        data = Wire.read();
        return true; // Return true if data is available
    }
    return false; // Return false if no data is available
}

// Start reception of ES100 time
void startReception() {
    if (!writeRegister(ES100_CONTROL0_REG, 0x01)) {
        logData("Failed to start reception");
    }
}

// Check if time has been received
bool checkTimeReceived(int dtArray[]) {
    uint8_t status;
    if (!readRegister(ES100_STATUS0_REG, status)) {
        logData("Failed to read status");
        return false;
    }
    if (status & 0x01) { // Check if time is ready
        uint8_t year, month, day, hour, minute, second;

        // Read each register into a local variable
        if (readRegister(ES100_YEAR_REG, year)) {
            dtArray[DT_YEAR] = year;
        }
        if (readRegister(ES100_MONTH_REG, month)) {
            dtArray[DT_MONTH] = month;
        }
        if (readRegister(ES100_DAY_REG, day)) {
            dtArray[DT_DAY] = day;
        }
        if (readRegister(ES100_HOUR_REG, hour)) {
          int timezoneOffset = -5; // UTC-5
          int adjustedHour = hour + timezoneOffset;

          // Handle wrap-around for hours
          if (adjustedHour < 0) {
              adjustedHour += 24; // Wrap around to the previous day
          } else if (adjustedHour >= 24) {
              adjustedHour -= 24; // Wrap around to the next day
          }

            dtArray[DT_HOUR] = adjustedHour;
        }
        if (readRegister(ES100_MINUTE_REG, minute)) {
            dtArray[DT_MINUTE] = minute;
        }
        if (readRegister(ES100_SECOND_REG, second)) {
            dtArray[DT_SECOND] = second;
        }
        return true; // Time received successfully
    }
    return false; // Time not ready
}
void updateDisplay(uint32_t currentSeconds, uint32_t lastSeconds) {
    matrix.fillScreen(0);  // Clear the entire display
    char currentStr[6];
    snprintf(currentStr, sizeof(currentStr), "%05lu", currentSeconds);
    uint16_t blueColor = matrix.color565(1, 26, 221);
    for (int i = 0; i < 5; i++) {
        drawNumber(3 + i * 12, 3, (*numbers[currentStr[i] - '0']), blueColor);
    }
}
void logData(const char* data) {
  File logFile = fatfs.open(dataFileName, FILE_WRITE);
  if (logFile) {
    logFile.println(data);
    logFile.close();
  } else {
    Serial.println("Error opening log file!");
  }
}
void logData(const String& data) {
    logData(data.c_str()); // Convert String to const char*
}

// Main loop function
void loop() {
    // Static variable to store the last seconds
    static uint32_t lastSeconds = 0;
    static unsigned long lastES100Check = 0;
    static unsigned long lastUpdateTime = 0; // Ensure this is declared

    // Get the current time from the external RTC
    DateTime now = rtc.now(); // Get the current time from the RTC

    // Convert the current time to seconds since midnight
    uint32_t currentSeconds = now.hour() * 3600 + now.minute() * 60 + now.second();

    // Check if the seconds have changed
    if (millis() - lastUpdateTime >= 1000) {
        lastUpdateTime = millis();
        if (countdownSeconds > 0) {
            countdownSeconds--;
            Serial.println(countdownSeconds);
            updateDisplay(countdownSeconds, countdownSeconds + 1);
            matrix.show();
        } else {
            countdownSeconds = 86399; // Reset to 23:59:59
            Serial.println(countdownSeconds);
            updateDisplay(countdownSeconds, 0); // Pass 0 instead of 00000
            matrix.show();
        }
    }

    // // Check for ES100 signal every second
    // if (millis() - lastES100Check >= 1000) {
    //     lastES100Check = millis();

    //     // Check if the last successful adjustment was over 24 hours ago
    //     if (!successful_es100_adjust || (millis() - lastSuccessfulAdjustTime >= 24 * 60 * 60 * 1000)) {
    //         // Start reception and check for time
    //         int dtArray[DT_LENGTH];
    //         if (checkTimeReceived(dtArray)) {
    //             successful_es100_adjust = true; // Set the flag to true
    //             lastSuccessfulAdjustTime = millis(); // Update the timestamp

    //             // Create a DateTime object
    //             DateTime newTime(dtArray[DT_YEAR], dtArray[DT_MONTH], dtArray[DT_DAY], dtArray[DT_HOUR], dtArray[DT_MINUTE], dtArray[DT_SECOND]);

    //             // Adjust the RTC with the new time
    //             rtc.adjust(newTime);
    //             now = rtc.now(); // Get the current time from the RTC again
    //             countdownSeconds = now.hour() * 3600 + now.minute() * 60 + now.second();

    //             // Format the date and time into a string
    //             String dateTimeString = String(newTime.year()) + "-" +
    //                                     String(newTime.month()) + "-" +
    //                                     String(newTime.day()) + " " +
    //                                     String(newTime.hour()) + ":" +
    //                                     String(newTime.minute()) + ":" +
    //                                     String(newTime.second());

    //             // Log the update with the formatted date and time
    //             logData(String("RTC updated with ES100 time: ") + dateTimeString);
    //         } else {
    //             Serial.println("No valid time signal received.");
    //         }
    //     }
    // }
}

    }
}