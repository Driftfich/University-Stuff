from abc import ABC, abstractmethod
from typing import Self
from queue import Queue, PriorityQueue


class GraphSearch(ABC):
    """Benutzung: Eigene Unterklasse anlegen mit class Unterklasse(GraphSearch). Diese müssen adj und goal implementieren. Dann lässt sich dfs verwenden."""
    @abstractmethod
    def adj(self) -> list[Self]:
        pass

    @abstractmethod
    def goal(self) -> bool:
        pass

    @abstractmethod
    def heuristic(self) -> int:
        return 0

    @abstractmethod
    def cost(self, child: Self) -> int:
        return 1

    def dfs(self, path, cnt: int = 5) -> list[Self]|bool:
        if self.goal():
            return path[::-1] # umdrehen
        else:
            if cnt == 0:
                return False
            new_neighbours = [n for n in self.adj() if n not in path]
            cutoff_occured = False
            for n in new_neighbours:
                p = n.dfs([n] + path, cnt-1)
                if isinstance(p, list) and len(p) > 0:
                    return p
                if p is False:
                    cutoff_occured = True
            return False if cutoff_occured else []
            
    def iterDfs(self, path, cnt: int = 5) -> list[Self]:
        i: int = 0
        while True:
            print(f"Iteration {i}")
            i += 1
            resPath = self.dfs(path, cnt)
            if isinstance(resPath, list) and len(resPath) == 0:
                print("Full tree depth got searched")
                return []
            if isinstance(resPath, bool) and resPath is False: 
                cnt+=1
                continue
            else: return resPath

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

    def aStar(self, path: list[Self] = [], ) -> list[Self]:
        priority_queue = PriorityQueue[tuple[int, Self]]()
        visited_map: dict[Self, Self|None] = {}
        g_map: dict[Self, int] = {}

        priority_queue.put((0, self))
        visited_map[self] = None
        g_map[self] = 0

        while not priority_queue.empty():
            _, current = priority_queue.get()
            if current.goal():
                path = []
                while current is not None:
                    path.append(current)
                    current = visited_map[current]
                return path[::-1]
            
            adj_nodes: list[Self] = current.adj()
            adj_h: dict[Self, int] = {n: n.heuristic() for n in adj_nodes}
            adj_g: dict[Self, int] = {n: current.cost(n) + g_map[current] for n in adj_nodes}
            current_childs: list[Self] = [n for n in adj_nodes if n not in visited_map.keys() or adj_g[n] < g_map[n]]
            for n in current_childs:
                priority_queue.put((adj_h[n] + adj_g[n], n))
                g_map[n] = adj_g[n]
                visited_map[n] = current

        return []

    def _idaStar(self, g: int, path: list[Self] = [], fThreshold: int = 10) -> tuple[list[Self]|None, float]:
        f = g + self.heuristic()

        if self.goal():
            return path[::-1], float(fThreshold)

        if f > fThreshold:
            return None, float(f)

        min_next_f = float('inf')

        new_neighbours = [n for n in self.adj() if n not in path]
        new_neighbours.sort(key=lambda x: x.heuristic() + self.cost(x))
        
        cutoff_occured = False
        for n in new_neighbours:
            res, next_f = n._idaStar(g + self.cost(n), [n] + path, fThreshold)

            if isinstance(res, list) and len(res) > 0:
                return res, float(next_f)
            if res is None:
                min_next_f = min(min_next_f, next_f)
                cutoff_occured = True

        if cutoff_occured:
            return None, min_next_f
        else:
            return [], min_next_f


    def idaStar(self, path: list[Self] = [], fThreshold: int = 10) -> list[Self]:
        i: int = 0
        while True:
            print(f"Iteration {i}")
            i += 1
            resPath, nextF = self._idaStar(0, path, fThreshold)
            if resPath is None:
                if nextF == float('inf'):
                    print("Goal unreachable.")
                    return []
                fThreshold = int(nextF)
                continue
            elif isinstance(resPath, list) and len(resPath) > 0:
                return resPath
            else:
                return []
            

    @staticmethod
    def path_to_str(path: list[Self]) -> str:
        out_str = ''
        for p in path:
            out_str += p.__str__() + '\n'
        return out_str
