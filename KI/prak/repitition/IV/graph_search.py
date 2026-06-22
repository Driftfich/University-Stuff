from abc import ABC, abstractmethod
from math import inf
from typing import Dict, List, Self, Tuple, Union
from queue import Queue

from numpy import empty


class GraphSearch(ABC):
    """Benutzung: Eigene Unterklasse anlegen mit class Unterklasse(GraphSearch). Diese müssen adj und goal implementieren. Dann lässt sich dfs verwenden."""
    @abstractmethod
    def adj(self) -> list[Self]:
        pass

    @abstractmethod
    def goal(self) -> bool:
        pass

    def dfs(self, path, cnt: int = 5) -> Tuple[list[Self], bool]:
        if self.goal():
            return path[::-1], True  # umdrehen

        if cnt == 0:
            return [], False

        all_children_reached_bottom = True

        new_neighbours = [n for n in self.adj() if n not in path]
        for n in new_neighbours:
            p, success = n.dfs([n] + path, cnt-1)

            if p:
                return p, success

            if not success:
                all_children_reached_bottom = False

        return [], all_children_reached_bottom

    def iterDfs(self, path, cnt: int=1, maxcnt: int|None=None) -> list[Self]:
        # run dfs with startcnt
        # if True, tree bottom reached, return path
        # false and empty path and cnt < maxcnt=> increase cnt and rerun
        bottomReached: bool = False
        startPath = path
        while not bottomReached and (maxcnt is None or cnt < maxcnt):
            path, bottomReached = self.dfs(startPath, cnt)

        return path

    def bfs(self, path: list[Self] = []) -> list[Self]:
        q = Queue()
        pred: Dict[Self, Union[Self, None]] = {}
        
        for i, n in enumerate(path):
            q.put(n)
            pred[n] = path[i-1] if i > 0 else None

        while not q.empty():
            current = q.get()
            if current.goal():
                path = []
                p = current
                while p is not None:
                    path.append(p)
                    p = pred[p]
                
                return path[::-1]

            neighbours = [n for n in current.adj() if n not in pred.keys()]
            for n in neighbours:
                q.put(n)
                pred[n] = current


        return []

    @staticmethod
    def path_to_str(path: list[Self]) -> str:
        out_str = ''
        for p in path:
            out_str += p.__str__() + '\n'
        return out_str
