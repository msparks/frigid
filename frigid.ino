// frigid
//
// Not your average temperature and humidity monitor.
#include <LiquidCrystal.h>
#include <stdlib.h>

#include "DHT.h"  // from adafruit
#include "sma.h"

namespace {

// LCD connections and properties.
const int kLcdRsPin = 7;
const int kLcdEnablePin = 8;
const int kLcdData4Pin = 12;
const int kLcdData5Pin = 11;
const int kLcdData6Pin = 10;
const int kLcdData7Pin = 9;

// LCD size.
const int kLcdColumns = 16;
const int kLcdRows = 2;

// DHT22 sensor connections.
const int kDhtPin = 2;

// External LCD display control.
LiquidCrystal lcd(kLcdRsPin, kLcdEnablePin,
                  kLcdData4Pin, kLcdData5Pin, kLcdData6Pin, kLcdData7Pin);

// DHT22 control.
DHT dht(kDhtPin, DHT22);

// Moving averages of temperatures.
SimpleMovingAverage<float, 60> temps_min;   // 1 per second
SimpleMovingAverage<float, 60> temps_hour;  // 1 per minute
SimpleMovingAverage<float, 24> temps_day;   // 1 per hour

// Moving averages of humidity percentages.
SimpleMovingAverage<float, 60> humidity_min;   // 1 per second
SimpleMovingAverage<float, 60> humidity_hour;  // 1 per minute
SimpleMovingAverage<float, 24> humidity_day;   // 1 per hour

// Time of the day.
struct Clock {
  int hours;
  int minutes;
  int seconds;
};
Clock time;

}  // namespace

void setup() {
  lcd.begin(kLcdColumns, kLcdRows);

  time.hours = 0;
  time.minutes = 0;
  time.seconds = 0;

  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  // Wait for the DHT22 to settle.
  delay(1000);
}

void loop() {
  // Take new sensor readings.
  const float humidity = dht.readHumidity();
  const float temp_c = dht.readTemperature();

  // Update averages over the last minute.
  temps_min.valueNew(temp_c);
  humidity_min.valueNew(humidity);

  // Temperature averages on the first line.
  lcd.setCursor(0, 0);
  printAveragesLine(
      temps_min.average(), temps_hour.average(), temps_day.average());
  lcd.print("\337");  // degree symbol

  // Humidity averages on the second line.
  lcd.setCursor(0, 1);  // col 0, row 1
  printAveragesLine(
      humidity_min.average(), humidity_hour.average(), humidity_day.average());
  lcd.print("%");

  // Take readings at the top of the next tick-second. This attempts to
  // keep reading times regular.
  const int ms_until_next_ticksecond = 1000 - (millis() % 1000);
  delay(ms_until_next_ticksecond);

  // Adjust clock.
  advanceTime();

  // Update historical averages.
  if (time.seconds == 0) {
    if (temps_min.size() > 0) {
      // Insert average over the last minute.
      temps_hour.valueNew(temps_min.average());
    }
    if (humidity_min.size() > 0) {
      humidity_hour.valueNew(humidity_min.average());
    }
  }
  if (time.minutes == 0) {
    if (temps_hour.size() > 0) {
      // Insert average over the last hour.
      temps_day.valueNew(temps_hour.average());
    }
    if (humidity_hour.size() > 0) {
      // Insert average over the last hour.
      humidity_day.valueNew(humidity_hour.average());
    }
  }
}

// Advances global clock by 1 second.
void advanceTime() {
  ++time.seconds;
  if (time.seconds == 60) {
    ++time.minutes;
    time.seconds = 0;
  }
  if (time.minutes == 60) {
    time.hours = (time.hours + 1) % 24;
    time.minutes = 0;
  }
}

void printAveragesLine(
    const float min_avg, const float hour_avg, const float day_avg) {
  const int width = 5;  // [-]dd.d
  const int precision = 1;
  char buffer[width];

  // Whitespace between fields is included in 'width'.
  dtostrf(min_avg, width, precision, buffer);
  lcd.print(buffer);
  dtostrf(hour_avg, width, precision, buffer);
  lcd.print(buffer);
  dtostrf(day_avg, width, precision, buffer);
  lcd.print(buffer);
}
