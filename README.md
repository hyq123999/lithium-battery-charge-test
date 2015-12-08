# LiPo batter charge cyle

- battery: 3.7V 530 mAH batter (todo: check capacity when home)
- Adafruit Feather ESP8266

Program wakes every 3 minutes and reports to thingspeak then goes back to deep sleep. It does not try again if error. There is a very small and very very faint LED also being driven (faint as in sometimes can't find it in dark room).

Used voltage divider as in Adafruit tutorial. Measured resistors with meter and they were surprisingly close to nominal values (todo: add circuit) so 100% on chart s/b close to 3.7V.

[Voltage chart](batt-cycle.png) - started with topping off then removed power while measuring. It ran till it died.

Curious how low th voltage can get for the Feather ESP. It reported for quite a while before failing to wake.
