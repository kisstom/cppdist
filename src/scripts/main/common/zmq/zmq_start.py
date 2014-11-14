import zmq, sys, time

port = int(sys.argv[1])
context = zmq.Context()
zmq_socket = context.socket(zmq.PUB)
zmq_socket.bind('tcp://*:%d'%port)

time.sleep(1)
zmq_socket.send('1')

