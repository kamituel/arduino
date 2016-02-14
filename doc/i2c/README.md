# I2C

## Known issue: I2C hangs
There is [a known issue](http://stackoverflow.com/questions/17272277/arduino-code-not-executing-after-wire-endtransmission-line) where I2C library would hang the board when there's an issue communicating with a slave device.

That's because `Wire.endTransmission()` does not support timeouts, it will wait indefinitely which hangs the board.

Solution is to use [an alternate library called I2C](http://www.dsscircuits.com/index.php/articles/66-arduino-i2c-master-library). In case this site ever goes down, a copy of the most recent version of this library [is available here](./I2C_Rev5.zip).

Fortunately, it's easy to convert code that used Wire, to use I2C library. There's an demo code located in the same ZIP file as a library itself, which contains a sample Wire-based code and its equivalent with I2C lib.
