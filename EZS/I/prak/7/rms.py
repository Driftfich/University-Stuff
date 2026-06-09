from dataclasses import dataclass, field
from typing import List, Set, Union

@dataclass
class Task:
    id: int
    p: int
    e: int
    d: int = 0
    executionTime: int = 0

    def __post_init__(self):
        if self.d == 0:
            self.d = self.p

    def release(self) -> None:
        # print(f"Task {self.id} released at time {startTime}")
        self.executionTime = 0

    def ready(self) -> bool:
        return self.executionTime < self.e

    def execute(self) -> None:
        self.executionTime += 1

    def __hash__(self) -> int:
        return hash(self.id)

def sortByRmsPriority(tasks: List[Task]) -> List[Task]:
    return sorted(tasks, key=lambda x: x.p)

startTime: int = 0
endTime: int = 500

## Task 1
# tasks: List[Task] = [
#     Task(id=1, p=8, e = 1),
#     Task(id=2, p=15, e = 3),
#     Task(id=3, p=20, e = 4)
# ]

## Task 2b)
tasks: List[Task] = [
    Task(id=1, p=10, e=2),
    Task(id=2, p=12, e=5),
    Task(id=3, p=15, e=4)
]

if len(tasks) < 1:
    print("No tasks to run")
    exit(1)

oldPriorityJob: Task|None = None
while startTime < endTime:
    for task in tasks:
        if startTime % task.p == 0:
            task.release()

    readyTasks = [task for task in tasks if task.ready()]

    if readyTasks:
        highestPriorityJob = sortByRmsPriority(readyTasks)[0]
        highestPriorityJob.execute()
        if oldPriorityJob != highestPriorityJob:
            print(f"Time {startTime}: Running task {highestPriorityJob.id}")
        oldPriorityJob = highestPriorityJob

            
    startTime += 1

    for task in tasks:
        if task.executionTime < task.e and startTime % (task.d) == 0:
            print(f"Time {startTime}: Task {task.id} over deadline")
