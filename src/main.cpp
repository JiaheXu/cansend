/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-3-Clause) */
/*
 * cansend.c - send CAN-frames via CAN_RAW sockets
 *
 * Copyright (c) 2002-2007 Volkswagen Group Electronic Research
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Volkswagen nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * Alternatively, provided that this notice is retained in full, this
 * software may be distributed under the terms of the GNU General
 * Public License ("GPL") version 2, in which case the provisions of the
 * GPL apply INSTEAD OF those given above.
 *
 * The provided data structures and external interfaces from this code
 * are not restricted to be used by modules with a GPL compatible license.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * Send feedback to <linux-can@vger.kernel.org>
 *
 */

#include <cansend.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <boost/thread/thread.hpp>

template <typename I>
std::string n2hexstr(I w, size_t hex_len = sizeof(I)<<1 )
{
    static const char* digits = "0123456789ABCDEF";
    std::string rc(hex_len, '0');
    for(size_t i=0,j=(hex_len-1)*4; i<hex_len;++i,j-=4 )
    {
        rc[i] = digits[(w>>j) & 0x0f];
    }
    return rc;
}
void velo_cmd()
{   
    int val = 4096;     
// CANBUS
    int signal = 1;
    std::string st;
    while(1)
    {
    	char cmd[18];
    	memset(cmd, '0', sizeof(cmd));
        
    	cmd[5] = '3';
    	cmd[6] = '3';
    	cmd[7] = '3';
    	cmd[8] = '#';
        std::string st = n2hexstr(val,4);
        
    	cmd[13] = '1';

    	cmd[17] = '\0';
    	cansend(cmd);
        std::cout << "velo: " << st << "\n";

    	std::this_thread::sleep_for( std::chrono::milliseconds(20));

    }
}

void servo_cmd()
{
    char cmd[18];
    memset(cmd, '0', sizeof(cmd));
    int signal = 1;
    std::string st;
    int val = 40;
    while(1)
    {
    
    	cmd[4] = '3';
    	cmd[5] = 'F';
    	cmd[6] = '3';
    	cmd[7] = '3';
    	cmd[8] = '#';
        std::string st = n2hexstr(val,4);
        
    	cmd[13] = st[0];
    	cmd[14] = st[1];
    	cmd[15] = st[2];
    	cmd[16] = st[3];
    	cmd[17] = '\0';
    	
        std::cout << "servo: " << st << "\n";
    	cansend(cmd);
    	val += 20*signal;
    	if( val<=40 )
    	{ 
    	    val = 40;
    	    signal = 1;
    	}
    	if( val >=960 )
    	{
    	    val = 960;
    	    signal = -1;
    	}
    	      
        std::this_thread::sleep_for( std::chrono::milliseconds(100));
    }
}

int main(int argc, char **argv)
{
	boost::thread th1(&velo_cmd);
	
	boost::thread th2(&servo_cmd);
	
	th1.join();
	th2.join();
	return 0;
}
