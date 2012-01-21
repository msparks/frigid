# frigid: a temperature and humidity display

Frigid is an [Arduino] program designed to monitor the temperature and humidity
of the inside of a refrigerator. However, it can be used in just about any
environment.

Frigid displays the temperature and humidity as
[simple moving averages][Moving average] over the last minute, hour, and
day. This idea is inspired by [load average] displays on Unix systems. This
display style allows the user to get a rough sense of the temperature stability
over time.

Here's a photo of what the display looks like:

![Frigid hardware prototype](http://farm8.staticflickr.com/7159/6737218447_dab0cac891_z.jpg)

(Averages that have not been computed yet appear as 0.)

[Arduino]: http://arduino.cc
[Moving average]: http://en.wikipedia.org/wiki/Moving_average
[load average]: http://en.wikipedia.org/wiki/Load_(computing)#Unix-style_load_calculation
