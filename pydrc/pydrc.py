import _pydrc

sample_dose = [-7.52,-7,-6.52,-6,-5.52,-5,-4]
sample_res = [0,1.3,10.2,43.3,75.1,91.5,100]

def fit4pdrc(dose, response):
    """
    takes a list of doses and responses and produces dictionary of return values
    """
    ret = _pydrc.fit4pdrc(dose, response)
    ret["dose"] = dose
    ret["response"] = response
    return ret