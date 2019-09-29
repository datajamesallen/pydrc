import unittest

from pydrc import fit4pdrc
from pydrc import sample_dose
from pydrc import sample_res

class TestPyDRC(unittest.TestCase):
    def setUp(self):
        pass
    def test_fit4pdrc(self):
        expected_dict = {
            "c":-5.8947269651897845,
            "h":1.3715668582354823,
            "b":0.0,
            "t":98.97939492567849,
            "p":4.0,
            "i":1000.0,
            "dose":sample_dose,
            "response":sample_res
        }
        actual_dict = fit4pdrc(sample_dose, sample_res)
        self.assertEqual(expected_dict, actual_dict)

if __name__ == '__main__':
    unittest.main()