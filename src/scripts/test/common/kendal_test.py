import kendal
import random
import unittest
import math

class TestKendal(unittest.TestCase):

    def setUp(self):
        self.seq = range(10)

    def test_kendal_code1(self):
        ken = kendal.Kendal('f1', 'f2', 10, 0.1) 

        d1 = dict()
        d2 = dict()

        d1[0] = 2
        d1[1] = 1
        d2[0] = 2
        d2[1] = 1

        code = ken.kendalTau(0, 1, d1, d2)
        self.assertEqual(1, code)

    def test_kendal_code2(self):
        ken = kendal.Kendal('f1', 'f2', 10, 0.1) 

        d1 = dict()
        d2 = dict()

        d1[0] = 1
        d1[1] = 2
        d2[0] = 2
        d2[1] = 1

        code = ken.kendalTau(0, 1, d1, d2)
        self.assertEqual(2, code)

    def test_kendal_code3(self):
        ken = kendal.Kendal('f1', 'f2', 10, 0.1) 

        d1 = dict()
        d2 = dict()

        d1[0] = 1
        d1[1] = 1
        d2[0] = 2
        d2[1] = 1

        code = ken.kendalTau(0, 1, d1, d2)
        self.assertEqual(3, code)


    def test_kendal_code4(self):
        ken = kendal.Kendal('f1', 'f2', 10, 0.1) 

        d1 = dict()
        d2 = dict()

        d1[0] = 2
        d1[1] = 1
        d2[0] = 2
        d2[1] = 2

        code = ken.kendalTau(0, 1, d1, d2)
        self.assertEqual(4, code)

    def test_kendal_code5(self):
        ken = kendal.Kendal('f1', 'f2', 10, 0.1) 

        d1 = dict()
        d2 = dict()

        d1[0] = 2
        d1[1] = 2
        d2[0] = 2
        d2[1] = 2

        code = ken.kendalTau(0, 1, d1, d2)
        self.assertEqual(5, code)


    def test_kendal_user(self):
        ken = kendal.Kendal('f1', 'f2', 10, 0.1)
        d1 = dict()
        d2 = dict()

        d1[0] = 1
        d1[1] = 2
        d1[2] = 3

        d2[0] = 1
        d2[1] = 3
        d2[2] = 2

	u = dict()
        u[0] = d1
        u[1] = d2
        ken.kendalForOneUser(d1, d2)
        self.assertEqual(1/3.0, ken.kendal)

    def test_kendal_user2(self):
        ken = kendal.Kendal('f1', 'f2', 10, 0.1)
        d1 = dict()
        d2 = dict()

        d1[0] = 1
        d1[1] = 2
        d1[2] = 3

        d2[0] = 1
        d2[1] = 2
        d2[2] = 2

	u = dict()
        u[0] = d1
        u[1] = d2
        ken.kendalForOneUser(d1, d2)
        self.assertEqual(2/math.sqrt(6), ken.kendal)


    def test_kendal_user_delta(self):
        ken = kendal.Kendal('f1', 'f2', 10, 0.1)
        d1 = dict()
        d2 = dict()

        d1[0] = 1
        d1[1] = 2
        d1[2] = 3

        d2[0] = 1
        d2[1] = 2.0
        d2[2] = 2.05

	u = dict()
        u[0] = d1
        u[1] = d2
        ken.kendalForOneUser(d1, d2)
        self.assertEqual(2/math.sqrt(6), ken.kendal)


    def test_kendal_user_delta2(self):
        ken = kendal.Kendal('f1', 'f2', 10, 0.001)
        d1 = dict()
        d2 = dict()

        d1[0] = 1
        d1[1] = 2
        d1[2] = 3

        d2[0] = 1
        d2[1] = 2.05
        d2[2] = 2

	u = dict()
        u[0] = d1
        u[1] = d2
        ken.kendalForOneUser(d1, d2)
        self.assertEqual(1/3.0, ken.kendal)

if __name__ == '__main__':
    unittest.main()

