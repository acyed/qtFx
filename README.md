qtFx
====

A simple AlienFx hack for the new Alienware 17

Version 1.0


This is a simple Qt interface for changing the colors on the New Alienware 17. The program can change everything except the battery status. I'll keep working on it and update when I figure it out. I borrowed code from several sources, including the following:
http://www.talisman.org/~erlkonig/software/pub/alienfx.README.html
http://3d.benjamin-thaut.de/?p=19

This program has to be run as root to access the USB device. If you don't want to run as root, I'm sure there's some group permisisons you could enable. You'll also need to install libusb-1.0 if you want to build from source.

If you're interested in trying to get the USB bytes sent from your own Alienware, here are the steps:
1.Install AlienFX in a Windows seven VMware image.
2.Follow the directions at http://vusb-analyzer.sourceforge.net/tutorial.html

Thanks for looking and please let me know if this works for you!
