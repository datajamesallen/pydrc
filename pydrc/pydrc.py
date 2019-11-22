import _pydrc
from math import log10

sample_dose = [-7.52,-7,-6.52,-6,-5.52,-5,-4]
sample_res = [0,1.3,10.2,43.3,75.1,91.5,100]

def fit4pdrc(dose, response, power=False, top_const=False, bot_const=False):
    """
    takes a list of doses and responses and produces dictionary of return values
    """
    if type(power) is int:
        dose = M_to_log(dose, power)
    if (top_const and bot_const):
        ret = _pydrc.both_const_fit4pdrc(dose, response)
    elif top_const:
        ret = _pydrc.top_const_fit4pdrc(dose, response)
    elif bot_const:
        ret = _pydrc.bot_const_fit4pdrc(dose, response)
    else:
        ret = _pydrc.fit4pdrc(dose, response)
    ret["dose"] = dose
    ret["response"] = response
    return ret

import matplotlib.pyplot as plot
import numpy as np

def frange(start, stop, step):
    i = start
    while (i < stop):
        yield i
        i += step

def M_to_log(doselist, power = -6):
    """
    converts Molar conc to log conc

    default power is -6, meaning the doses are in uM
    """
    log_doselist = []
    for item in doselist:
        log_item = log10(item * (10 ** power))
        log_doselist.append(log_item)
    return log_doselist

def drcfunc4(x, t, b, c, h):
    return((b+((t-b)/(1+10**((c-x)*h)))))

def showfit(fit):
    """
    takes a the fit structure and plots the fit on a graph
    """
    mindose = min(fit["dose"])
    maxdose = max(fit["dose"])
    rangedose = abs(mindose - maxdose)
    graphmin = mindose - (rangedose * 0.25)
    graphmax = maxdose + (rangedose * 0.25)
    fig, ax = plot.subplots()
    x = np.linspace(graphmin, graphmax, 100)
    c = fit["c"]
    h = fit["h"]
    t = fit["t"]
    b = fit["b"]
    y = eval('(b+((t-b)/(1+10**((c-x)*h))))')
    ax.scatter(fit["dose"], fit["response"])
    ax.plot(x, y)
    plot.show()
    return None
    
