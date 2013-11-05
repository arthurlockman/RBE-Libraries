#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

/**
 * @brief Tells whether or not a value is within a certain tolerance of another value.
 * @param value The value to check.
 * @param desiredValue The value to check value against.
 * @param tolerance The desired tolerance.
 * @return A boolean, whether or not the value is within the tolerance.
 */
template <typename T>
inline bool WithinTolerance(T value, T desiredValue, T tolerance)
{
        if ((value < desiredValue + tolerance) && (value > desiredValue - tolerance))
                return true;
        else
                return false;
};

/**
 * @brief Limits a value to a defined limit.
 * @param input The input value.
 * @param max The value to limit it to.
 * @return The limited value. 
 */
template <typename T> 
inline T Limit(T input, T max)
{
    if (input > max)
    {
        return max;
    } else if (input < -max)
    {
        return -max;
    }
    return input;
};

long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
 
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
 
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
  long result = (high<<8) | low;
 
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
};

#endif
