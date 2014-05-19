//////////////////////////////////////////////////////////////////////
// sliptty.cpp -- SLIP Serial (tty) Packet Class for POSIX
// Date: Mon May 19 08:44:52 2014
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <assert.h>

#include "sliptty.hpp"

//////////////////////////////////////////////////////////////////////
// In these serial I/O routines, we ignore device errors and treat
// the situation as if the device became temporary disconnect (as
// can happen with Bluetooth communication).
//////////////////////////////////////////////////////////////////////

static uint8_t
sliptty_in(void *arg) {
	int fd = *(int *)arg;
	int rc;
	uint8_t b;

	do	{
		rc = read(fd,&b,1);
	} while ( rc == -1 );			// EINTR may be one of the expected errors here

	return b;
}

static void
sliptty_out(uint8_t b,void *arg) {
	int fd = *(int *)arg;
	int rc;

	do	{
		rc = write(fd,&b,1);
	} while ( rc == -1 && errno == EINTR );
}

//////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////

SlipTty::SlipTty() : slip(sliptty_in,sliptty_out,&fd) {
	fd = -1;
	buf = 0;
}

//////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////

SlipTty::~SlipTty() {
	close();
}

//////////////////////////////////////////////////////////////////////
// Open tty device, configure for raw I/O at baud rate speed.
// Returns zero for success, else errno.
//////////////////////////////////////////////////////////////////////

int
SlipTty::open(const char *device_path,int speed,unsigned maxbuflen,bool enable_crc8) {

	if ( fd >= 0 )
		close();

	//////////////////////////////////////////////////////////////
	// First open the serial device
	//////////////////////////////////////////////////////////////

	fd = ::open(device_path,O_RDWR);
	if ( fd < 0 )
		return errno;

	//////////////////////////////////////////////////////////////
	// Set serial parameters for raw I/O
	//////////////////////////////////////////////////////////////

	struct termios tattr;
	int rc;

	rc = tcgetattr(fd,&tattr);
	if ( rc == -1 ) {
		::close(fd);
		fd = -1;
		return errno;
	}

	cfmakeraw(&tattr);
	rc = cfsetspeed(&tattr,speed);
	if ( rc == -1 ) {
		::close(fd);
		fd = -1;
		return errno;
	}

	rc = tcsetattr(fd,TCSANOW,&tattr);
	if ( rc == -1 ) {
		::close(fd);
		fd = -1;
	}

	//////////////////////////////////////////////////////////////
	// Allocate a Rx buffer
	//////////////////////////////////////////////////////////////

	buf = new char[maxbuflen];
	buflen = maxbuflen;
	assert(buf && buflen > 0);

	slip.enable_crc8(enable_crc8);

	return 0;
}

int
SlipTty::close() {
	int rc = ::close(fd);
	
	if ( buf ) {
		delete[] buf;
		buf = 0;
		buflen = 0;
	}

	fd = -1;
	return rc == -1 ? errno : 0;
}

//////////////////////////////////////////////////////////////////////
// Write a packet
//////////////////////////////////////////////////////////////////////

void
SlipTty::write(void *data,unsigned length) {
	slip.write(data,length);
}

//////////////////////////////////////////////////////////////////////
// Read a packet
//////////////////////////////////////////////////////////////////////

void *
SlipTty::read(unsigned& length) {
	SLIP::Status s;

	while ( (s = slip.read(buf,buflen,length)) != SLIP::ES_Ok )
		;

	return buf;
}

// End sliptty.cpp
