import csv
from SPI_Interface import *

''' Environment Set-Up '''
runs = 1                        # numbers 
k = 2                           # k denotes the number of considered devices 
n_bits = 16*8*runs              # n_bits

file_name = 'responses.csv'     # file has to be in the same directory
responses = []                  # responses from the FPGA


# Turn the csv responses in to a python list
def parse_data():
    with open(file_name, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for response in reader:
            responses.append(response)

def main():
    parse_data()
    response_ref = golden_response(responses)
    reliability = calc_reliability(n_bits, response_ref, responses, k)
    uniqueness = calc_uniqueness(n_bits,responses, k)
    print("Reliability = %f.2, Uniqueness = $f.2"%(reliability, uniqueness))