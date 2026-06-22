from abc import ABC, abstractmethod
from typing import Dict, List, Self, Union
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

    def dfs(self, path, cnt: int = 5) -> list[Self]:
        if self.goal():
            return path[::-1] # umdrehen
        else:
            if cnt == 0:
                return []
            new_neighbours = [n for n in self.adj() if n not in path]
            for n in new_neighbours:
                p = n.dfs([n] + path, cnt-1)
                if p:
                    return p
            return []

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
