from abc import ABC, abstractmethod
from typing import Self
from queue import Queue


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
        queue = Queue()
        # visited_map: visited -> parent node
        visited_map: dict[Self, Self|None] = {}

        # fill queue and visited_map with initial path
        for i, n in enumerate(path):
            queue.put(n)
            visited_map[n] = path[i-1] if i > 0 else None

        while not queue.empty():
            current = queue.get()

            if current.goal():
                # build path from visited_map
                path = []
                while current is not None:
                    path.append(current)
                    current = visited_map[current]
                return path[::-1]

            current_childs = [n for n in current.adj() if n not in visited_map.keys()]

            # fill queue with child nodes
            for n in current_childs:
                queue.put(n)
                visited_map[n] = current

        return []


    @staticmethod
    def path_to_str(path: list[Self]) -> str:
        out_str = ''
        for p in path:
            out_str += p.__str__() + '\n'
        return out_str
