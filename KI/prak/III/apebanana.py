from graph_search import GraphSearch
from typing import Self
from dataclasses import dataclass, replace

SIZE_X = 10
SIZE_Y = 10

@dataclass(frozen=True)
class Pos:
    x: int
    y: int

    def getAdjPos(self, xMax: int, yMax: int) -> list['Pos']:
        res = []
        if self.x > 0:        res.append(replace(self, x=self.x - 1))
        if self.y > 0:        res.append(replace(self, y=self.y - 1))
        if self.x < xMax - 1: res.append(replace(self, x=self.x + 1))
        if self.y < yMax - 1: res.append(replace(self, y=self.y + 1))
        return res

    def __repr__(self) -> str:
        return f"Pos({self.x}, {self.y})"

@dataclass(frozen=True)
class GlobalState:
    apePos: Pos
    bananaPos: Pos
    boxPos: Pos
    climbed: bool = False
    grabbed: bool = False

    def getNextStates(self, xMax: int, yMax: int) -> list['GlobalState']:
        res = []
        # For all next States other than grab operation, default grabbed to False
        # Add Ape Move and Ape + Box Move operations if not climbed
        if not self.climbed:
            for apePosUpdate in self.apePos.getAdjPos(xMax, yMax):
                res.append(replace(self, apePos=apePosUpdate, grabbed=False))
                if (self.apePos == self.boxPos):
                    res.append(replace(self, apePos=apePosUpdate, boxPos=apePosUpdate, grabbed=False))

        # Add climb down from box and grab banana operation
        else:
            res.append(replace(self, climbed=not self.climbed, grabbed=False))
            res.append(replace(self, grabbed=True))


        # Add climb on box operation
        if (self.apePos == self.boxPos and not self.climbed):
            res.append(replace(self, climbed=not self.climbed, grabbed=False))

        return res

    def __repr__(self) -> str:
        return f"GlobalState(apePos={self.apePos}, bananaPos={self.bananaPos}, boxPos={self.boxPos}, climbed={self.climbed}, grabbed={self.grabbed})"

# GOAL = GlobalState(apePos=Pos(2,3), bananaPos=Pos(2,3), boxPos=Pos(2,3), grabbed=True, climbed=True)

class Solver(GraphSearch):

    def __init__(self, globState: GlobalState):
        self.globState: GlobalState = globState

    def adj(self) -> list[Self]:
        return [Solver(nextState) for nextState in self.globState.getNextStates(SIZE_X, SIZE_Y)]

    def goal(self) -> bool:
        # return GOAL == self.globState
        return self.globState.bananaPos == self.globState.boxPos == self.globState.apePos and self.globState.grabbed and self.globState.climbed

    def __hash__(self) -> int:
            return hash(self.globState)

    def __eq__(self, other) -> bool:
        if not isinstance(other, Solver):
            return False
        return self.globState == other.globState

    def __repr__(self) -> str:
        return str(self.globState)

if __name__ == '__main__':
    startState = GlobalState(apePos=Pos(0,0), boxPos=Pos(8,9), bananaPos=Pos(4,3))
    solv = Solver(startState)
    path = solv.bfs([solv])
    print(path)
