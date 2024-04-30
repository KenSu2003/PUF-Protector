#!/usr/bin/env python3

# docs: https://eblot.github.io/pyftdi/api/spi.html

from pyftdi.spi import SpiController
import csv
''' Initialize Variables '''
# Upper 4-bits for data | Lower 4-bits for commands
START_CMD=0x01 
SET_RUNS_CMD=0x02  
SET_DELTA_HIGH=0x03
SET_DELTA_REST=0x04

def configure_timing(slave, d_high, d_rest):
    #exchange does a read/write from SPI
    return slave.exchange([d_high|SET_DELTA_HIGH, d_rest|SET_DELTA_REST], 1)    # returns 1 bytes if timing configurated

def configure_runs(slave, k):
    '''
    k: number of times to challange the PUF 
    expect k responses (16 bytes each)
    0x01 <= k <= 0x0F
    '''
    k = k << 4
    return slave.exchange([k | SET_RUNS_CMD], 1)                # returns 1 byte if run configured

def start_run(slave, k):
    '''start the configured run and retrieve results''' 
    read_bytes = 16*k
    return slave.exchange([START_CMD],read_bytes)              # returns all the response bytes for the k runs

def main():
    # Instantiate a SPI controller
    spi = SpiController()
    
    # Configure the first interface (IF/1) of the FTDI device as a SPI master
    spi.configure('ftdi://ftdi:2232h/1')

    # Get a port to a SPI slave w/ /CS on A*BUS3 and SPI mode 0 @ 30MHz
    slave = spi.get_port(cs=0, freq=30E6, mode=0) # mode 0: half-duplex (2 is full-duplex)

    # Set the number of runs
    runs = 1
    # set configuration register: number of clock cycles for delta-high and delta-rest
    # start with 128 cycles for each (if internal running at 50 MHz)
    check_byte_timing = configure_timing(slave, 128, 128)
    print("Configure timing:",check_byte_timing)
    if int.from_bytes(check_byte_timing) != 128:
        return
    # if possible: configure number of runs
    check_byte_runs = configure_runs(slave, runs)
    print("Configure runs:",check_byte_runs)
    if int.from_bytes(check_byte_runs) != runs:
        return
    # When both the timing and number of runs are set start the tests 
    responses = start_run(slave, runs)
    responses_int = list(responses)
    data = []
    #data = izip_longest(*(iter(len(responses_int)),)*16) ---- if you want to be cool
    data = [responses_int[x:x+16] for x in range(0, len(responses_int), 16)]
    with open("responses.csv", "w") as f:
        w = csv.writer(f)
        w.writerows(data)

if __name__ == '__main__':
    main()