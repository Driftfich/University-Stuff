from graph_search import GraphSearch
from typing import Self, List, Dict
from dataclasses import replace, dataclass, field
import random

def getAllIndices(list: list[int], value: int) -> list[int]:
    return [i for i, v in enumerate(list) if v == value]

def list_replace(list: list[int], replacement: dict[int, int]) -> list[int]:
    # create a dict which tracks the indices with the values to replace
    replacement_dict: Dict[int, List[int]] = {}
    for k, v in replacement.items():
        replacement_dict[v] = getAllIndices(list, v)

    for new_val, indices in replacement_dict.items():
        for index in indices:
            list[index] = new_val
    return list

@dataclass(frozen=True)
class PuzzleState:
    state: list[int] = field(default_factory=lambda: list(range(16)))
    
    def __repr__(self) -> str:
        board = " | ".join([str(self.state[i: i+4]) for i in range(0, 16, 4)])
        return f"PuzzleState(\n{board}\n)"

    def adj(self) -> list[Self]:
        res = []
        zero_idx = self.state.index(0)
        r, c = divmod(zero_idx, 4)
        for dr, dc in [(-1, 0), (1, 0), (0, -1), (0, 1)]: # hoch, runter, links, rechts
            nr, nc = r + dr, c + dc
            if 0 <= nr < 4 and 0 <= nc < 4:
                new_state = list(self.state)
                new_idx = nr * 4 + nc
                new_state[zero_idx], new_state[new_idx] = new_state[new_idx], new_state[zero_idx]
                res.append(replace(self, state=new_state))
        return res

    def __hash__(self) -> int:
        return hash(tuple(self.state))

def stateFromZeroIndex(zero_index: int) -> list[int]:
    remaining = list(range(1, 16))
    res = []
    for i in list(range(16)):
        if i == zero_index:
            res.append(0)
        else:
            # get random value from remaining
            random_value = random.choice(remaining)
            res.append(random_value)
            remaining.remove(random_value)
    return res

def solvableState(GOAL_STATE: PuzzleState, permutations: int) -> PuzzleState:
    for i in range(permutations):
        adj = GOAL_STATE.adj()
        # choose random next state
        GOAL_STATE = random.choice(adj)

    return GOAL_STATE


GOAL_STATE = PuzzleState(state=stateFromZeroIndex(0))
START_STATE = solvableState(GOAL_STATE, 191)

def hammingDistance(state: PuzzleState, goal: PuzzleState) -> int:
    return sum(1 for i in range(16) if state.state[i] != GOAL_STATE.state[i] and state.state[i] != 0)

def manhattanDistance(state: PuzzleState, goal: PuzzleState) -> int: # 
    return sum(abs(state.state.index(i) - goal.state.index(i)) for i in range(1, 16))

class Puzzle(GraphSearch):
    def __init__(self, state: PuzzleState):
        self.state = state

    def adj(self) -> list["Puzzle"]:
        return [Puzzle(state) for state in self.state.adj()]

    def goal(self) -> bool:
        return self.state == GOAL_STATE

    def heuristic(self) -> int:
        return manhattanDistance(self.state, GOAL_STATE)

    def cost(self, child: "Puzzle") -> int:
        return 1

    def __repr__(self) -> str:
        return self.state.__repr__()

    def __eq__(self, other: "Puzzle") -> bool:
        return isinstance(other, Puzzle) and self.state == other.state

    def __hash__(self) -> int:
        return self.state.__hash__()

    def __lt__(self, other: "Puzzle") -> bool:
        return self.heuristic() < other.heuristic()

if __name__ == "__main__":
    puzzle = Puzzle(START_STATE)
    print("START STATE:")
    print(START_STATE)
    print("GOAL STATE:")
    print(GOAL_STATE)
    print("IDA* SOLUTION:")
    print(puzzle.idaStar([puzzle]))