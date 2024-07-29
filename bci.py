import socket
import json
import numpy
import tensorflow
import os.path

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("127.0.0.1", 12346))

input = numpy.empty([31000])
free = numpy.empty([1000])
free.fill(0)
last = numpy.empty([1000])
last.fill(0)

if os.path.exists("brain.keras") == True:
	model = tensorflow.keras.models.load_model("brain.keras", custom_objects={'LeakyReLU': tensorflow.keras.layers.LeakyReLU})
else:
	model = tensorflow.keras.models.Sequential([
		tensorflow.keras.layers.Input(shape=(31000,)),
		tensorflow.keras.layers.Dense(5000, activation=tensorflow.keras.layers.LeakyReLU()),
		tensorflow.keras.layers.Dense(5000, activation=tensorflow.keras.layers.LeakyReLU()),
		tensorflow.keras.layers.Dense(5000, activation=tensorflow.keras.layers.LeakyReLU()),
		tensorflow.keras.layers.Dense(3000)
])

	loss_fn = tensorflow.keras.losses.MeanSquaredError()

	model.compile(optimizer='adam', loss=loss_fn, metrics=['accuracy'])

while True:
	data, addr = sock.recvfrom(65536)
	if data == bytes("save", "utf-8"):
		model.save("brain.keras")
		continue
	j = json.loads(data)
	if j["type"] != 'fft':
		exit(1)
	o = numpy.concatenate((numpy.array(j["data"]).flatten(), free))
	model.fit(numpy.array([input]), numpy.array([o]), epochs=1)
	i = numpy.array(input[:28000])
	p = numpy.array(o[:2000])
	input = numpy.concatenate((p, i, last))
	o = model.predict(numpy.array([input]))[0]
	p = numpy.array(o[2000:2010])
	q = numpy.array(last[10:])
	last = numpy.concatenate((p, q))
	free = numpy.array(o[2000:])
	output = numpy.array(o[:2000])
	j = json.dumps({"type":"fft","data":numpy.reshape(output,(16,125)).tolist()})
	sock.sendto(bytes(j, "utf-8"), ("127.0.0.1", 12345))

