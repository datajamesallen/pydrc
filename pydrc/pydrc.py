import _pydrc

def fit4pdrc(dose, response):
    """
    takes a list of doses and responses and produces dictionary of return values
    """
    ret = _pydrc.fit4pdrc(dose, response)
    ret["dose"] = dose
    ret["response"] = response
    return ret