def isprime(startnumber):
      for divisor in range(2,int(startnumber**0.5+1)):
        if startnumber % divisor == 0:
          return False
      return True

import sys

x = int(sys.argv[1])
for i in xrange(100000):
  if isprime(x + i):
    print x + i
    break
  i += 1
