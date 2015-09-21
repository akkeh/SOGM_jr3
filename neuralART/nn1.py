import numpy as np
import matplotlib.pyplot as plt
from random import random as rand

treshold = 5

class neuron:
	id = -99
	N = 0
	y = 0
	w = np.array([])

	def proc(self, inp, outp):
		y = np.sum(inp * self.w)
		if y < self.tresh:
			y = 0
		outp[self.id] = y
		return outp

	def backtrace(self, outp, inp):
		y = outp[self.id]
		for n in range(len(inp)):
			inp[n] += y * w[n]
		return inp
	
	def __init__(self, t_id, t_N, t_tresh):
		self.id = t_id
		self.N = t_N
		self.tresh = t_tresh

		self.w = np.zeros(self.N)
		for n in range(self.N):
			self.w[n] = rand() * 2 - 1


def run(tresh = 5):
	Nin = 4		# input length
	Nhid = 50	# hidden length
	Nout = 3	# output length

	ROUNDS = 5
	inputs = np.array([
			[0, 1, 0, 1],
			[1, 1, 1, 1],
			[1, 1, 0, 0],
			[0, 0, 0, 0],
			[0, 1, 1, 0]
		]); 

	for round in range(ROUNDS):
		# input layer:
		input = np.empty(Nin)
		input = np.array([0, 1, 0, 1 ])

		# hidden layer:
		hidden = np.empty(shape=(Nhid,), dtype=object)
		for n in range(Nhid):
			hidden[n] = neuron(n, Nin, tresh)
		y_hid = np.zeros(Nhid)

		# output layer:
		output = np.empty(shape=(Nout,), dtype=object)
		for n in range(Nout):
			output[n] = neuron(n, Nhid, tresh)
		y_out = np.zeros(Nout)

		# calculating:
		for n in range(Nhid):
			y_hid = hidden[n].proc(input, y_hid)

		for n in range(Nout):
			y_out = output[n].proc(y_hid, y_out)

		print("output phase", round, ":")
		max_i = 0
		max = 0
		for i in range(len(y_out)):
			if y_out[i] > max:
				max_i = i
				max = y_out[i]
		print("winner:", max_i)
		print("hidden:", y_hid)
		print("------------------")


