/**************************************************************************/
/*!
  @file RWS_I2C.cpp

  @section intro Introduction

  I2C Sensor Management functions

  i2c_scanner adapted from https://playground.arduino.cc/Main/I2cScanner/
  that tests only the standard 7-bit addresses.

  https://cdn-learn.adafruit.com/downloads/pdf/i2c-addresses.pdf
  provides a list of sensors that might go with those addresses.

  @subsection author Author

  Written by Rick Sellens.

  @subsection license License

  CCBY license
*/
/**************************************************************************/
#include "RWS_UNO.h"

/**************************************************************************/
/*!
    @brief Check an address for a sensor at the appropriate register.
    @param addr the address to check for the sensor.
    @return the sensor ID, or 0 on failure
*/
/**************************************************************************/
unsigned RWS_UNO::checkBMX(uint8_t addr) {
  unsigned sensorID = read8(addr, BMX_REGISTER_CHIPID);
  if (sensorID == 0xFF)
    return 0;
  return sensorID;
}
/**************************************************************************/
/*!
    @brief Check an address for a sensor at the appropriate register.
    @param addr the address to check for the sensor.
    @return the sensor ID, or 0 on failure
*/
/**************************************************************************/
unsigned RWS_UNO::checkBNO(uint8_t addr) {
  unsigned sensorID = read8(addr, BNO_REGISTER_CHIPID);
  if (sensorID == 0xFF)
    return 0;
  return sensorID;
}
/**************************************************************************/
/*!
    @brief Check an address for a sensor at the appropriate register.
    @param addr the address to check for the sensor.
    @return the sensor ID, or 0 on failure
*/
/**************************************************************************/
unsigned RWS_UNO::checkMCP9808(uint8_t addr) {
  unsigned manufID = read16(addr, MCP9808_REG_MANUF_ID);
  unsigned sensorID = read8(addr, MCP9808_REG_DEVICE_ID);
  if (manufID != 0x0054)
    return 0;
  return sensorID;
}
/**************************************************************************/
/*!
    @brief Check an address for a sensor at the appropriate register.
    @param addr the address to check for the sensor.
    @return the sensor ID, or 0 on failure
*/
/**************************************************************************/
unsigned RWS_UNO::checkTMP006(uint8_t addr) {
  unsigned manufID = read16(addr, TMP006_MANID);
  unsigned sensorID = read16(addr, TMP006_DEVID);
  if (manufID != 0x5449)
    return 0;
  return sensorID;
}
/**************************************************************************/
/*!
    @brief Check an address for a sensor at the appropriate register.
    @param addr the address to check for the sensor.
    @return the sensor ID, or 0 on failure
*/
/**************************************************************************/
unsigned RWS_UNO::checkLIS3DH(uint8_t addr) {
  unsigned sensorID = read8(addr, LIS3DH_REG_WHOAMI);
  if (sensorID == 0xFF)
    return 0;
  return sensorID;
}

/**************************************************************************/
/*!
    @brief Print a two digit HEX address.
    @param address the address to print
*/
/**************************************************************************/
void RWS_UNO::printAddr(unsigned address) {
  Serial.print("0x");
  if (address < 16)
    Serial.print("0");
  Serial.print(address, HEX);
}

/**************************************************************************/
/*!
    @brief Print a sensor description.
    @param nom the name of the sensor
    @param id the sensor id
    @param address the address to print
*/
/**************************************************************************/
void RWS_UNO::printSensor(const char *nom, unsigned id, uint8_t address) {
  Serial.print("Found a ");
  Serial.print(nom);
  Serial.print(" Sensor ID = ");
  printAddr(id);
  Serial.print(" at address ");
  printAddr(address);
  Serial.print("\n");
}

/**************************************************************************/
/*!
    @brief Scan the entire address space looking for I2C devices and report
    to the console. This is a diagnostic function to help check connections.
*/
/**************************************************************************/
void RWS_UNO::i2cScan() {
  byte error, address;
  int nDevices;

  Serial.print("Scanning I2C...\n\n");

  nDevices = 0;
  for (address = 0x0; address < 0x7F; address++) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) // there is something there
    {
      Serial.print("I2C device found at address ");
      printAddr(address);
      Serial.print("\n");
      if (address >= BMX_ADDRESS_A && address <= BMX_ADDRESS_B) {
        unsigned sensorID = checkBMX(address);
        if (sensorID) {
          printSensor("BMP/BME", sensorID, address);
          Serial.print("BMP085/BMP180=0x55, BMP280=0x56-0x58, BME280=0x60, BME680=0x61\n");
        }
      }
      if (address >= BNO_ADDRESS_A && address <= BNO_ADDRESS_B) {
        unsigned sensorID = checkBNO(address);
        if (sensorID)
          printSensor("BNO", sensorID, address);
      }
      if (address >= MCP9808_ADDRESS_A && address <= MCP9808_ADDRESS_B) {
        unsigned sensorID = checkMCP9808(address);
        if (sensorID)
          printSensor("MCP9808", sensorID, address);
      }
      if (address >= TMP006_ADDRESS_A && address <= TMP006_ADDRESS_B) {
        unsigned sensorID = checkTMP006(address);
        if (sensorID)
          printSensor("TMP006", sensorID, address);
      }
      if (address >= LIS3DH_ADDRESS_A && address <= LIS3DH_ADDRESS_B) {
        unsigned sensorID = checkLIS3DH(address);
        if (sensorID)
          printSensor("LIS3DH", sensorID, address);
      }
      if (address >= 0x3C && address <= 0x3D)
        Serial.print("Could be an Adafruit_SSD1306 128x32 OLED Display\n");
      if (address == 0x68)
        Serial.print("Could be an RTClib supported Real Time Clock\n");
      if (address >= 0x1C && address <= 0x1D)
        Serial.print("Could be an MMA845X Accelerometer\n");
      if (address == 0x18)
        Serial.print("Could be an LSM303 Accelerometer / Magnetometer\n");
      if (address == 0x1E)
        Serial.print("Could be an LSM303 Accelerometer / Magnetometer\n");
      if (address >= 0x6A && address <= 0x6B)
        Serial.print("Could be an L3GD20H Gyro\n");
      if (address >= 0x40 && address <= 0x41)
        Serial.print("Could be an HTU31D Humidity / Temperature Sensor\n");
      if (address >= 0x4C && address <= 0x4C)
        Serial.print("Could be an Adafruit EMC2101 Fan Control Breakout\n");
      if (address >= 0x0C && address <= 0x0C)
        Serial.print("This is the SMBus Alert Response Address (0x0C), which is a kind of broadcast address\n");
        
      nDevices++;
      Serial.print("\n");
      delay(100); // for the monitor to catch up
    }
  }
  if (nDevices == 0)
    Serial.print("No I2C devices found\n");
  else
    Serial.print("done, check https://learn.adafruit.com/downloads/pdf/i2c-addresses.pdf "
      "for more info.\n\n");
}

/*!
 *    @brief  Low level I2C 16 bit read procedure adapted from Adafruit
 *    @param addr The I2C address of the device
 *    @param reg The register on the device
 *    @return value
 */
uint16_t RWS_UNO::read16(uint8_t addr, uint8_t reg) {
  uint16_t val = 0xFFFF;
  uint8_t state;

  Wire.beginTransmission(addr);
  Wire.write((uint8_t)reg);
  state = Wire.endTransmission();

  if (state == 0) {
    Wire.requestFrom((uint8_t)addr, (uint8_t)2);
    val = Wire.read();
    val <<= 8;
    val |= Wire.read();
  }

  return val;
}

/*!
 *    @brief  Low level I2C 8 bit read procedure adapted from Adafruit
 *    @param addr The I2C address of the device
 *    @param reg The register on the device
 *    @return value
 */
uint8_t RWS_UNO::read8(uint8_t addr, uint8_t reg) {
  uint8_t val = 0xFF;
  uint8_t state;

  Wire.beginTransmission(addr);
  Wire.write((uint8_t)reg);
  state = Wire.endTransmission();

  if (state == 0) {
    Wire.requestFrom((uint8_t)addr, (uint8_t)1);
    val = Wire.read();
  }

  return val;
}

/*!
 *    @brief  Low level 8 bit I2C write procedure adapted from Adafruit
 *    @param addr The I2C address of the device
 *    @param reg The register on the device
 *    @param data The value to write to the register
 *    @return none
 */
void RWS_UNO::write8(uint8_t addr, uint8_t reg, uint8_t data) {
  Wire.beginTransmission(addr); // start transmission to device 
  Wire.write(reg); // sends register address to read from
  Wire.write(data);  // write data
  Wire.endTransmission(); // end transmission
}
