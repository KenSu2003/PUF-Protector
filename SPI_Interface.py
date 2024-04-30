import csv

def hamming(t1, t2):
    '''Hamming distance between two vectors, same size'''
    return sum(map(int.__ne__, t1, t2))


def calc_reliability(n_bits, response_ref, responses_v, k):
    factor = (1/k)
    vector_sum = 0
    for i in range(k-1):
        vector_sum += (hamming(response_ref, responses_v[i])/n_bits)
    reliability = factor * vector_sum * 100
    return reliability


def calc_uniqueness(n_bits, responses, k):
    hd_sum = 0                      # Sum of the hamming distances                     
    factor = 2/(k(k-1))
    for i in range(1,k-1):
        for j in range(i+1,k):
            R_i = responses[i]
            R_j = responses[j]
            hd_sum += (hamming(R_i, R_j)/n_bits)

    uniqueness = factor*hd_sum*100
    return uniqueness

    
def most_frequent(List):
    return max(set(List), key = List.count)


def golden_response(input_set):
    #return mode of input set
    resp_list = [input_set]
    return most_frequent(resp_list)
    




