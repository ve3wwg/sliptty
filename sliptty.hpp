//////////////////////////////////////////////////////////////////////
// sliptty.hpp -- SLIP Serial (tty) Packet Class for POSIX Systems
// Date: Mon May 19 08:43:06 2014   (C) Warren Gay ve3wwg
///////////////////////////////////////////////////////////////////////

#ifndef SLIPTTY_HPP
#define SLIPTTY_HPP

#include "slip.hpp"

class SlipTty {
	SLIP		slip;		// SLIP I/O Object
	int		fd;		// Open tty
	char		*buf;		// Rx buffer
	unsigned	buflen;		// Rx buffer length

public:	SlipTty();
	~SlipTty();

	int open(const char *device_path,int speed,unsigned maxbuflen,bool enable_crc8=true);
	int close();
	
	void write(void *data,unsigned length);
	void *read(unsigned& length);
};


#endif // SLIPTTY_HPP

// End sliptty.hpp
