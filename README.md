sliptty
=======

Posix SLIP serial class for communicating with Arduino/Teensy.

It is designed for Posix side communicaton with Aduino/Teensy-3.x where
packet communication over serial/Bluetooth is involved. This class
eliminates most of the tedium of establishing the serial parameters
and SLIP I/O.

This module depends upon the SLIP project:

	git@github.com:ve3wwg/slip.git


    class SlipTty {
        SLIP            slip;           // SLIP I/O Object
        int             fd;             // Open tty
        char            *buf;           // Rx buffer
        unsigned        buflen;         // Rx buffer length

    public: 
        SlipTty();
        ~SlipTty();

        int open(const char *device_path,int speed,unsigned maxbuflen,bool enable_crc8=true);
        int close();
        
        void write(void *data,unsigned length);
        void *read(unsigned& length);
    };

open method:
    device_path     "/dev/cu.usbserialXYZZY" (example)
    speed           38400 (baud rate)
    maxbuflen       This is the maximum sized receive packet
    enable_crc8     Enabled by default, but enables SLIP's CRC8 feature
