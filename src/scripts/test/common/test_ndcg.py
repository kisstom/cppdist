import sys, os

base = os.path.dirname(os.path.abspath(__file__)) 
sys.path.append(base + '/../../main/common/measures/')

import ndcg
import unittest

class TestNDCG(unittest.TestCase):
  def test_ndcg(self):
    myndcg = ndcg.NDCG("foo1", "foo2", 3)
    rel = [2, 1]
    dcg = myndcg.ndcg(rel) 
    self.assertAlmostEqual(dcg, 3.6309, 3)

  def test_ndcg_user(self):
    myndcg = ndcg.NDCG("foo1", "foo2", 3)
    
    simrank_score = dict()
    simrank_score["p1"] = 1
    simrank_score["p2"] = 2
    simrank_score["p3"] = 3

    other_score = dict()
    other_score["p1"] = 3
    other_score["p2"] = 1
    other_score["p3"] = 2

    simrank_d = dict()
    other_d = dict()
    simrank_d["user"] = simrank_score
    other_d["user"] = other_score

    myndcg.ndcgForOneUser(simrank_score, other_score) 
    self.assertAlmostEqual(myndcg.cum_ndcg, 1.0, 3)


  def test_ndcg_user2(self):
    myndcg = ndcg.NDCG("foo1", "foo2", 3)
    
    simrank_score = dict()
    simrank_score["p1"] = 1
    simrank_score["p2"] = 2
    simrank_score["p3"] = 3

    other_score = dict()
    other_score["p1"] = 3
    other_score["p2"] = 1
    other_score["p4"] = 2

    simrank_d = dict()
    other_d = dict()
    simrank_d["user"] = simrank_score
    other_d["user"] = other_score

    myndcg.ndcgForOneUser(simrank_score, other_score) 
    self.assertAlmostEqual(myndcg.cum_ndcg, 0.5307, 3)



if __name__ == '__main__':
    unittest.main()

