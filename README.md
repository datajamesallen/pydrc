# pydrc
A simple dose response curve fitting package for python

usage:

```python
import pydrc

# sample dose response data
dose = pydrc.sample_dose
>> [-7.52,-7,-6.52,-6,-5.52,-5,-4]
res = pydrc.sample_res
>> [0, 1.3, 10.2, 43.3, 75.1, 91.5, 100]

pydrc.fit4pdrc(dose, res)
>> {
	'c':-5.89...          # EC50
	'h':-1.37...          # hillslope
	'b':0.0               # bottom
	't':98.97...          # top
	'p':4.0               # number of parameters
	'i':1000              # number of iterations
	'dose':[-7.52...]     # doses used to fit
	'response':[0,1.3...] # responses used to fit
}
```

uses the C library [LEVMAR](http://users.ics.forth.gr/~lourakis/levmar/) as
a backend to compute the non linear regression problem. This is an inplementation of the [Levenberg-Marquardt algorithm](https://en.wikipedia.org/wiki/Levenberg%E2%80%93Marquardt_algorithm), which combines Guass-Newton and gradient descent for a more robust problem solver even with distant starting values.
