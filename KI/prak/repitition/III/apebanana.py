from dataclasses import dataclass, replace
from typing import Self, List
from graph_search import GraphSearch

MAX_X = 10
MAX_Y = 10

@dataclass(frozen=True)
class Pos:
    x: int
    y: int

    def getAdjPos(self) -> List[Self]:
        pos = []
        if self.x>0: pos.append(replace(self, x=self.x-1))
        if self.x < MAX_X: pos.append(replace(self, x=self.x+1))
        if self.y>0: pos.append(replace(self, y=self.y-1))
        if self.y<MAX_Y: pos.append(replace(self, y=self.y+1))
        return pos

    def __repr__(self) -> str:
        return f"Pos({self.x}, {self.y})"

@dataclass(frozen=True)
class GlobalState:
    apePos: Pos
    bananaPos: Pos
    boxPos: Pos
    climbed: bool = False
    grabbed: bool = False

    def getNextStates(self) -> List[Self]:
        res = []
        
        # apeNot not on chair <=> climbed = False
        if not self.climbed:
            # ape can move around
            adjPos = [newApePos for newApePos in self.apePos.getAdjPos()]
            [res.append(replace(self, apePos=newApePos, grabbed=False)) for newApePos in adjPos]

            # if apePos = boxPos, ape can move box or climb
            if (self.apePos == self.boxPos):
                [res.append(replace(self, apePos=newApePos, boxPos=newApePos, grabbed=False)) for newApePos in adjPos]

                res.append(replace(self, climbed=True, grabbed=False))
            
        else:
            # climb down
            res.append(replace(self, climbed=False, grabbed=False))

            # try grab for banana
            res.append(replace(self, grabbed=True))

        return res

    def __repr__(self) -> str:
        return f"GlobalState(apePos={self.apePos}, bananaPos={self.bananaPos}, boxPos={self.boxPos}, climbed={self.climbed}, grabbed={self.grabbed})"

class Solver(GraphSearch):

    def __init__(self, globState: GlobalState):
        self.globState: GlobalState = globState

    def adj(self) -> list[Self]:
        return [Solver(nextState) for nextState in self.globState.getNextStates()]

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
    startState = GlobalState(apePos=Pos(0,0), boxPos=Pos(4,9), bananaPos=Pos(9,4))
    solv = Solver(startState)
    # path = solv.Oldbfs([solv])
    path = solv.bfs([solv])
    print(path)