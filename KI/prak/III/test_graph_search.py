import unittest
from typing import Self
from graph_search import GraphSearch

class Graph(GraphSearch):
    adj_list = {0: [1,3], 1: [2,3], 2: [0], 3: []} # Adjazenzliste als Hashtabelle

    def __init__(self, start=0):
        self.node = start

    def __repr__(self):
        return str(self.node)

    def adj(self) -> list[Self]:
        return [Graph(n) for n in self.adj_list[self.node]]

    def goal(self) -> bool:
        return self.node == 2

    def __eq__(self, other):
        if not isinstance(other, Graph):
            return False
        else:
            return self.node == other.node

    def __hash__(self):
        return hash(self.node)


class TestGraphSearch(unittest.TestCase):
    def test_dfs(self):
        s = Graph(0)
        p = s.dfs([s])
        print("Dfs Path:\n", p)
        self.assertEqual([Graph(n) for n in [0,1,2]], p)

    def test_bfs(self):
        s = Graph(0)
        p = s.bfs([s])
        print("Bfs Path:\n", p)
        self.assertEqual([Graph(n) for n in [0,1,2]], p)

if __name__ == '__main__':
    unittest.main()
