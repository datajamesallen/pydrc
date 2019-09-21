# pydrc
dose response curve fitting package for python

uses the C library [LEVMAR](http://users.ics.forth.gr/~lourakis/levmar/) as
a backend to compute the non linear regression problem. This is an inplementation of the [Levenberg-Marquardt algorithm](https://en.wikipedia.org/wiki/Levenberg%E2%80%93Marquardt_algorithm), which combines Guass-Newton and gradient descent for a more robust problem solver even with distant starting values.
