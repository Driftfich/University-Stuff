from dataclasses import dataclass, replace
from typing import List, Self
from graph_search import GraphSearch

@dataclass
class Node:
    x: int
    y: int

    def getNeighbors(self, up: bool, down: bool, left: bool, right: bool) -> list["Node"]:
        neighbors = []
        if up:
            neighbors.append(Node(self.x, self.y - 1))
        if down:
            neighbors.append(Node(self.x, self.y + 1))
        if left:
            neighbors.append(Node(self.x - 1, self.y))
        if right:
            neighbors.append(Node(self.x + 1, self.y))
        return neighbors

    def __repr__(self) -> str:
        return f"Node(x={self.x}, y={self.y})"

    def __hash__(self) -> int:
        return hash((self.x, self.y))


GOAL_NODE = Node(4, 0)

class Labyrinth(GraphSearch):
    adj_coors: dict[Node, list[Node]] = {
        Node(0,0): Node(0,0).getNeighbors(False, False, False, True),
        Node(1,0): Node(1,0).getNeighbors(False, True, True, True),
        Node(2, 0): [Node(1,0), Node(3, 0)],
        Node(3, 0): [Node(2, 0)],
        Node(4, 0): [Node(4,1)],

        Node(0,1): [Node(0,2)],
        Node(1,1): [Node(1,0), Node(1,2), Node(2,1)],
        Node(2,1): [Node(1,1)],
        Node(3,1): [Node(3,2), Node(4,1)],
        Node(4,1): [Node(3,1), Node(4,0)],

        Node(0,2): [Node(0,1), Node(1,2), Node(0,3)],
        Node(1,2): [Node(1,1), Node(0,2)],
        Node(2,2): [Node(2,3), Node(3,2)],
        Node(3,2): Node(3,2).getNeighbors(True, True, True, False),
        Node(4,2): Node(4,2).getNeighbors(False, True, False, False),

        Node(0,3): Node(0,3).getNeighbors(True, True, False, True),
        Node(1,3): Node(1,3).getNeighbors(False, True, True, False),
        Node(2,3): Node(2,3).getNeighbors(True, True, False, False),
        Node(3,3): Node(3,3).getNeighbors(True, False, False, True),
        Node(4,3): Node(4,3).getNeighbors(True, True, True, False),

        Node(0,4): Node(0,4).getNeighbors(True, False, False, False),
        Node(1,4): Node(1,4).getNeighbors(True, False, False, True),
        Node(2,4): Node(2,4).getNeighbors(True, False, True, True),
        Node(3,4): Node(3,4).getNeighbors(False, False, True, True),
        Node(4,4): Node(4,4).getNeighbors(True, False, True, False),
    }

    def __init__(self, node: Node):
        self.node = node

    def adj(self) -> List["Labyrinth"]:
        return [Labyrinth(n) for n in self.adj_coors[self.node]]

    def goal(self):
        return self.node == GOAL_NODE

    def __hash__(self) -> int:
        return self.node.__hash__()

if __name__ == "__main__":
    lab = Labyrinth(Node(0,0))
    path = lab.bfs([lab])
    node_path = [node.node for node in path]
    if path:
        print(f"Pfad gefunden: {node_path}")