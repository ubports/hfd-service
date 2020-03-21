# hfd-service

**H**uman **f**eedback **d**evice **service** is a dbus activated service that manages human feedback devices sutch as leds and vibrators on mobile devices. 


This replaces usensord and unity8's own led/light handeler

## Why?

We have a need for a more modular system running service now that we have different devices handeling implementations differently. 

Secondly we could not use unity8's own led handeler as this only runs in userspace and we wont have access to sysfs devices from there in a secure maner. We can use a dbus service protected with apparmor. 
