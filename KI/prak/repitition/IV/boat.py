from enum import Enum
from dataclasses import dataclass, replace
from typing import Self
from graph_search import GraphSearch

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
    goat: int
    wolf: int
    cabbage: int
    ferryman: int

    def numericalInvalid(self) -> bool:
        return self.goat < 0 or self.wolf < 0 or self.cabbage < 0 or self.ferryman < 0

    def ferrymanGoatTogetherInvalid(self) -> bool:
        return self.goat > 0 and (self.wolf > 0 or self.cabbage > 0) and self.ferryman <= 0

    def invalid(self) -> bool:
        return self.numericalInvalid() or self.ferrymanGoatTogetherInvalid()

    def __repr__(self) -> str:
        return f"SideState(goat={self.goat}, wolf={self.wolf}, cabbage={self.cabbage}, ferryman={self.ferryman})"

@dataclass(frozen=True)
class BoatState:
    passengerState: SideState
    side: Pos

    # From { goat, wolf, cabbage } only one can be on the boat at the same time
    def setInvalid(self) -> bool:
        goat = self.passengerState.goat
        wolf = self.passengerState.wolf
        cabbage = self.passengerState.cabbage
        ferryman = self.passengerState.ferryman
        return goat > 0 and (wolf > 0 or cabbage > 0) or \
            wolf > 0 and (cabbage > 0 or goat > 0) or \
            cabbage > 0 and (wolf > 0 or goat > 0)

    def invalid(self) -> bool:
        return self.passengerState.invalid() or self.setInvalid()

    def __repr__(self) -> str:
        return f"BoatState(passengerState={self.passengerState}, side={self.side})"

@dataclass(frozen=True)
class GlobalState:
    left: SideState
    right: SideState
    boat: BoatState

    def generateAllAdjStates(self) -> list[Self]:
        res = []

        side_label: str = self.boat.side.value
        current_side: SideState = getattr(self, side_label)

        # move objects/passengers from boat to side
        res.append(replace(self, **{side_label: replace(current_side, goat=current_side.goat + 1)}, boat=replace(self.boat, passengerState=replace(self.boat.passengerState, goat=self.boat.passengerState.goat -1))))
        res.append(replace(self, **{side_label: replace(current_side, wolf=current_side.wolf + 1)}, boat=replace(self.boat, passengerState=replace(self.boat.passengerState, wolf=self.boat.passengerState.wolf -1))))
        res.append(replace(self, **{side_label: replace(current_side, cabbage=current_side.cabbage + 1)}, boat=replace(self.boat, passengerState=replace(self.boat.passengerState, cabbage=self.boat.passengerState.cabbage -1))))
        res.append(replace(self, **{side_label: replace(current_side, ferryman=current_side.ferryman + 1)}, boat=replace(self.boat, passengerState=replace(self.boat.passengerState, ferryman=self.boat.passengerState.ferryman -1))))

        # move objects/passengers from side to boat
        res.append(replace(self, **{side_label: replace(current_side, goat=current_side.goat - 1)}, boat=replace(self.boat, passengerState=replace(self.boat.passengerState, goat=self.boat.passengerState.goat +1))))
        res.append(replace(self, **{side_label: replace(current_side, wolf=current_side.wolf - 1)}, boat=replace(self.boat, passengerState=replace(self.boat.passengerState, wolf=self.boat.passengerState.wolf +1))))
        res.append(replace(self, **{side_label: replace(current_side, cabbage=current_side.cabbage - 1)}, boat=replace(self.boat, passengerState=replace(self.boat.passengerState, cabbage=self.boat.passengerState.cabbage +1))))
        res.append(replace(self, **{side_label: replace(current_side, ferryman=current_side.ferryman - 1)}, boat=replace(self.boat, passengerState=replace(self.boat.passengerState, ferryman=self.boat.passengerState.ferryman +1))))

        # move boat to other side when ferryman is on boat
        if self.boat.passengerState.ferryman > 0:
            res.append(replace(self, boat=replace(self.boat, side=Pos.LEFT if self.boat.side == Pos.RIGHT else Pos.RIGHT)))

        return res

    def invalid(self) -> bool:
        return self.left.invalid() or self.right.invalid() or self.boat.invalid()

    def filterInvalidStates(self, allStates: list[Self]) -> list[Self]:
        res = []
        for state in allStates:
            if state.invalid():
                continue
            res.append(state)
        return res

    def adj(self) -> list[Self]:
        return self.filterInvalidStates(self.generateAllAdjStates())
        
    def __repr__(self) -> str:
        return f"GlobalState(left={self.left}, right={self.right}, boat={self.boat})"

GOAL_STATE = GlobalState(left=SideState(goat=0, wolf=0, cabbage=0, ferryman=0), right=SideState(goat=1, wolf=1, cabbage=1, ferryman=1), boat=BoatState(passengerState=SideState(goat=0, wolf=0, cabbage=0, ferryman=0), side=Pos.RIGHT))

class EndGame(GraphSearch):
    def __init__(self, globState: GlobalState):
        self.globState = globState

    def adj(self) -> list[Self]:
        return [EndGame(nextState) for nextState in self.globState.adj()]

    def __eq__(self, other) -> bool:
        return isinstance(other, EndGame) and self.globState == other.globState
        
    def __hash__(self) -> int:
        return hash(self.globState)

    def goal(self) -> bool:
        return self.globState == GOAL_STATE

    def __repr__(self) -> str:
        return str(self.globState)

if __name__ == '__main__':
    startState = GlobalState(left=SideState(goat=1, wolf=1, cabbage=1, ferryman=1), right=SideState(goat=0, wolf=0, cabbage=0, ferryman=0), boat=BoatState(passengerState=SideState(goat=0, wolf=0, cabbage=0, ferryman=0), side=Pos.LEFT))
    endgame = EndGame(startState)
    path = endgame.iterDfs([endgame], cnt=15)
    print(path)