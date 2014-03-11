import heapq

h = []
heapq.heappush(h, (3, 7))
heapq.heappush(h, (2, 6))
print len(h)
heapq.heapreplace(h, (1, 2))

print h
