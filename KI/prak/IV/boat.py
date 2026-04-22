from typing import Self
from graph_search import GraphSearch
from dataclasses import dataclass, field, replace
from enum import Enum

MAX_BOAT_CAPACITY = 2

class Pos(Enum):
    LEFT = "left"
    RIGHT = "right"

    def get_adj_pos(self) -> list["Pos"]:
        members = list(Pos)
        idx = members.index(self)
        adj = []
        if idx > 0: adj.append(members[idx - 1])
        if idx < len(members) - 1: adj.append(members[idx + 1])
        return adj

    def __repr__(self):
        return f"Pos({self.value})"

@dataclass(frozen=True)
class SideState:
    pos: Pos
    missionaries: int = 0
    cannibals: int = 0

    def __repr__(self):
        return f"SideState(pos={self.pos}, missionaries={self.missionaries}, cannibals={self.cannibals})"

    def invalid(self) -> bool:
        return (self.missionaries > 0 and self.missionaries < self.cannibals) or self.missionaries < 0 or self.cannibals < 0

@dataclass(frozen=True)
class BoatState:
    pos: Pos
    missionaries: int = 0
    cannibals: int = 0

    def __repr__(self):
        return f"BoatState(pos={self.pos}, missionaries={self.missionaries}, cannibals={self.cannibals})"

    def totalPeople(self) -> int:
        return self.missionaries + self.cannibals

    def adjBoatPos(self) -> list[Self]:
        res = []
        for pos in self.pos.get_adj_pos():
            res.append(replace(self, pos=pos))
        return res

    def invalid(self) -> bool:
        return (self.missionaries > 0 and self.missionaries < self.cannibals) or self.totalPeople() > MAX_BOAT_CAPACITY or self.missionaries < 0 or self.cannibals < 0

@dataclass(frozen=True)
class GlobalState:
    left: SideState
    right: SideState
    boat: BoatState

    def create_state(self, boat_miss: int, boat_cann: int, side_miss: int, side_cann: int) -> Self:

        side_label: str = self.boat.pos.value
        current_side: SideState = getattr(self, side_label)

        new_boat = replace(self.boat, missionaries=boat_miss, cannibals=boat_cann)
        new_side = replace(current_side, missionaries=side_miss, cannibals=side_cann)
        return replace(self, boat=new_boat, **{side_label: new_side})

    def genAllAdjStates(self) -> list[Self]:
        res = []
        if self.boat.totalPeople() > 0:
            # add boat moves
            for boatStates in self.boat.adjBoatPos():
                res.append(replace(self, boat=boatStates))

        # get current side as label and attribute
        side_label: str = self.boat.pos.value
        current_side: SideState = getattr(self, side_label)

        # add missionary moves
        # leave boat
        res.append(self.create_state(self.boat.missionaries - 1, self.boat.cannibals, current_side.missionaries + 1, current_side.cannibals))
        # join boat
        res.append(self.create_state(self.boat.missionaries + 1, self.boat.cannibals, current_side.missionaries - 1, current_side.cannibals))

        # add cannibal moves
        # leave boat
        res.append(self.create_state(self.boat.missionaries, self.boat.cannibals - 1, current_side.missionaries, current_side.cannibals + 1))
        # join boat
        res.append(self.create_state(self.boat.missionaries, self.boat.cannibals + 1, current_side.missionaries, current_side.cannibals - 1))

        return res

    def filterInvalidStates(self, allStates: list[Self]) -> list[Self]:
        res = []
        for state in allStates:
            if state.invalid():
                continue
            res.append(state)
        return res

    def adj(self) -> list[Self]:
        return self.filterInvalidStates(self.genAllAdjStates())

    def invalid(self) -> bool:
        return self.left.invalid() or self.right.invalid() or self.boat.invalid()

    def __repr__(self):
        return f"GlobalState(left={self.left}, right={self.right}, boat={self.boat})"

GOAL_STATE = GlobalState(boat=BoatState(pos=Pos.RIGHT), left=SideState(pos=Pos.LEFT, missionaries=0, cannibals=0), right=SideState(pos=Pos.RIGHT, missionaries=20, cannibals=20))

class EndGame(GraphSearch):
    def __init__(self, globState: GlobalState):
        self.globState = globState

    def adj(self) -> list[Self]:
        return [EndGame(nextState) for nextState in self.globState.adj()]

    def __eq__(self, other):
        return isinstance(other, EndGame) and self.globState == other.globState
    
    def __hash__(self):
        return hash(self.globState)
    
    def goal(self) -> bool:
        return GOAL_STATE == self.globState
    
    def __repr__(self) -> str:
        return str(self.globState)
    
if __name__ == '__main__':
    startState = GlobalState(boat=BoatState(pos=Pos.LEFT), left=SideState(pos=Pos.LEFT, missionaries=20, cannibals=20), right=SideState(pos=Pos.RIGHT, missionaries=0, cannibals=0))
    endgame = EndGame(startState)
    path = endgame.bfs([endgame])
    # print(path)
    if isinstance(path, list):
        for p in path:
            print(p)
    else:
        print(path)