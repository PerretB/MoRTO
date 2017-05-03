import unittest
import morto
import numpy as np


class TestMorto(unittest.TestCase):
    def test_wrapperWeighted(self):
        parents = np.asarray([4, 4, 6, 6, 7, 7, 7, 7], dtype=np.int32)
        values = np.asarray([13, 14, 6, 8, 11, 7, 5, 10], dtype=np.float64)
        weights = np.asarray([1, 1, 1, 1, 1, 1, 2, 1], dtype=np.float64)
        results = np.zeros((8,), dtype=np.float64)
        morto.monotonicRegressionOnTree(parents, values, results, weights)
        self.assertTrue(np.allclose(results, np.asarray([12, 12, 6, 6, 12, 7, 6, 12], dtype=np.float64)))

    def test_wrapperNotWeighted(self):
        parents = np.asarray([4, 4, 6, 6, 7, 7, 7, 7], dtype=np.int32)
        values = np.asarray([13, 14, 6, 8, 11, 7, 5, 10], dtype=np.float64)
        results = np.zeros((8,), dtype=np.float64)
        morto.monotonicRegressionOnTree(parents, values, results)
        self.assertTrue(np.allclose(results, np.asarray([12, 12, 6, 6.5, 12, 7, 6.5, 12], dtype=np.float64)))

    def test_fail(self):
        parents = np.asarray([4, 4, 6, 6, 7, 7, 7, 7], dtype=np.int32)
        values = np.asarray([13, 14, 6, 8, 11, 7, 5, 10], dtype=np.float64)
        results = np.zeros((5,), dtype=np.float64)
        self.assertRaises(morto.MortoException, morto.monotonicRegressionOnTree, parents, values, results)


if __name__ == '__main__':
    unittest.main()
