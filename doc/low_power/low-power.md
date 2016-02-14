# Low power Arduino

## Power efficient sleep

Using [LowPower Arduino library](https://github.com/rocketscream/Low-Power) (article with samples [here](http://www.rocketscream.com/blog/2011/07/04/lightweight-low-power-arduino-library/)) one can save some power by sleeping efficiently.

In case either repo, or an article ever goes down, [here is a copy of the library](./Low-Power-master.zip).

Sample:

```
#include "LowPower.h"
LowPower.powerDown(SLEEP_120MS, ADC_OFF, BOD_OFF);
```
