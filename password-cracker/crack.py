#!/usr/bin/env python3

import concurrent.futures
import hashlib
import os
import string
import sys

# Constants

ALPHABET = string.ascii_lowercase + string.digits

# Functions

def usage(exit_code=0):
    progname = os.path.basename(sys.argv[0])
    print(f'''Usage: {progname} [-a ALPHABET -c CORES -l LENGTH -p PATH -s HASHES]
    -a ALPHABET Alphabet to use in permutations
    -c CORES    CPU Cores to use
    -l LENGTH   Length of permutations
    -p PREFIX   Prefix for all permutations
    -s HASHES   Path of hashes file''')
    sys.exit(exit_code)

def md5sum(s):
    ''' Compute md5 digest for given string. '''
    res = hashlib.md5(s.encode())

    return res.hexdigest()

def permutations(length, alphabet=ALPHABET):
    ''' Recursively yield all permutations of alphabet up to given length. '''
    if length == 1:
        for char in alphabet:
            yield char
    else:
        for char in alphabet:
            for perm in permutations(length - 1, alphabet):
                yield char + perm
                

def flatten(sequence):
    ''' Flatten sequence of iterators. '''
    for iterator in sequence:
        yield from iterator


def crack(hashes, length, alphabet=ALPHABET, prefix=''):
    ''' Return all password permutations of specified length that are in hashes
    by sequentially trying all permutations. '''
    return [prefix + perm for perm in permutations(length, alphabet) if md5sum(prefix + perm) in hashes]

def whack(arguments):
    ''' Call the crack function with the specified list of arguments '''
    return crack(arguments[0], arguments[1], arguments[2], arguments[3])

def smash(hashes, length, alphabet=ALPHABET, prefix='', cores=1):
    ''' Return all password permutations of specified length that are in hashes
    by concurrently subsets of permutations concurrently.
    '''

    args = ((hashes, length-1, alphabet, prefix + char) for char in alphabet)

    with concurrent.futures.ProcessPoolExecutor(cores) as executor:
        return flatten(executor.map(whack, args))

def main():
    arguments   = sys.argv[1:]
    alphabet    = ALPHABET
    cores       = 1
    hashes_path = 'hashes.txt'
    length      = 1
    prefix      = ''

    # Parse command line arguments

    while arguments and arguments[0].startswith('-'):
        arg = arguments.pop(0)
        if arg == '-a':
            alphabet = arguments.pop(0)
        elif arg == '-c':
            cores = int(arguments.pop(0))
        elif arg == '-l':
            length = int(arguments.pop(0))
        elif arg == '-p':
            prefix = arguments.pop(0)
        elif arg == '-s':
            hashes_path = arguments.pop(0)
        elif arg == '-h':
            usage(0)
        else:
            usage(1)

    infile = open(hashes_path, 'r')
    hashes = set(map(lambda x: x.strip('\n'), infile.readlines())) 

    if length == 1:
        for pswd in crack(hashes, length, alphabet, prefix):
            print(pswd)
    else:
        for pswd in smash(hashes, length, alphabet, prefix, cores):
            print(pswd)

# Main Execution

if __name__ == '__main__':
    main()

# vim: set sts=4 sw=4 ts=8 expandtab ft=python:
