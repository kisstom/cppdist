import math, sys, random

def szita(n):
  primes = [1] * (n + 1)
  primes[0] = 0
  primes[1] = 0
  
  for x in xrange(2, int(math.sqrt(n)) + 1):
    for m in xrange(2, n / x + 1):
      primes[m * x] = 0

  return[a for a,b in enumerate(primes) if b]

def genPrimes(n):
  retPrimes = []
  primes = szita(n * 100)
  for x in xrange(n):
    randindex = random.randint(0, len(primes) - 1)
    retPrimes += [primes[randindex]]
    primes.pop(randindex)
  return retPrimes

def is_prime(n):
  """ check if n is prime"""
  i = 2
  if n <=1:
    return False
  Sqrt_Of_n = n**0.5
  while i <= Sqrt_Of_n:
    if n % i == 0:
      return False
    else:
      i += 1
  return True
      

def next_prime(n, m):
  for i in xrange(m):
    if is_prime(n + i):
      return n + i
  return False

if __name__ == "__main__":
  import sys
  n = int(sys.argv[1])
  m = int(sys.argv[2])
  print next_prime(n, m)
