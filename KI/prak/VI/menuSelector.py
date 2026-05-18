from ortools.init.python import init
from ortools.sat.python import cp_model
from enum import Enum
from dataclasses import dataclass

class MenuType(Enum):
    PRE = 0
    MAIN = 1
    POST = 2

@dataclass(frozen=True)
class MenuItem:
    name: str
    type: MenuType
    calories: int

"""
Splits a list of menu items into a lol by the type
:param menuList: list of menu items
:return: list of lists of menu items
"""
def splitByType(menuList: list[MenuItem]) -> list[list[MenuItem]]:
    # Group items by their type
    pre = [item for item in menuList if item.type == MenuType.PRE]
    main = [item for item in menuList if item.type == MenuType.MAIN]
    post = [item for item in menuList if item.type == MenuType.POST]
    
    return [pre, main, post]

def createMenuModel(menuLol: list[list[MenuItem]], maxCalories: int) -> tuple[cp_model.CpModel, list[cp_model.IntVar], list[cp_model.IntVar]]:
    model = cp_model.CpModel()

    menuVars = []
    calorieVars = []
    for typeIndex, typeList in enumerate(menuLol):
        if not typeList:
            continue
            
        calories = [item.calories for item in typeList]

        # The index variable (the "choice")
        m_var = model.new_int_var(0, len(typeList) - 1, f"menu_{typeIndex}")
        menuVars.append(m_var)

        # The calorie variable (the "result" of the choice)
        c_var = model.new_int_var(min(calories), max(calories), f"calorie_{typeIndex}")
        calorieVars.append(c_var)
    
        # This links the choice to the result: calorieVars[i] == calories[m_var]
        model.add_element(m_var, calories, c_var)

    # Add constraint so the sum of selected calories is <= maxCalories
    model.add(sum(calorieVars) <= maxCalories)

    return model, menuVars, calorieVars

class MenuSolutionCallback(cp_model.CpSolverSolutionCallback):
    def __init__(self, menuVars, calorieVars, menuLol):
        cp_model.CpSolverSolutionCallback.__init__(self)
        self._menuVars = menuVars
        self._calorieVars = calorieVars
        self._menuLol = menuLol
        self._solution_count = 0

    def on_solution_callback(self):
        self._solution_count += 1
        print(f"--- Solution {self._solution_count} ---")
        total_calories = 0
        for i in range(len(self._menuVars)):
            menu_idx = self.Value(self._menuVars[i])
            calories = self.Value(self._calorieVars[i])
            menu_item = self._menuLol[i][menu_idx]
            print(f"  {MenuType(i).name}: {menu_item.name} ({calories} cal)")
            total_calories += calories
        print(f"  Total: {total_calories} cal")

    def solution_count(self):
        return self._solution_count

myMenus = [
    MenuItem(name="Pizza", type=MenuType.MAIN, calories=500),
    MenuItem(name="Lasagne", type=MenuType.MAIN, calories=600),
    MenuItem(name="Spaghetti", type=MenuType.MAIN, calories=400),
    MenuItem(name="Risotto", type=MenuType.MAIN, calories=300),
    MenuItem(name="Burger", type=MenuType.MAIN, calories=400),
    MenuItem(name="BigHouse Burger", type=MenuType.MAIN, calories=1000),


    MenuItem(name="Salat", type=MenuType.PRE, calories=100),
    MenuItem(name="Bruchetta", type=MenuType.PRE, calories=200),
    MenuItem(name="Tomatensuppe", type=MenuType.PRE, calories=100),

    MenuItem(name="Tiramisu", type=MenuType.POST, calories=300),
    MenuItem(name="Gelato", type=MenuType.POST, calories=200),
    MenuItem(name="Kaffee", type=MenuType.POST, calories=100),
]

maxCalories = 1000

typeSplitLol = splitByType(myMenus)

model, menuVars, calorieVars = createMenuModel(typeSplitLol, maxCalories)

solver = cp_model.CpSolver()
solution_callback = MenuSolutionCallback(menuVars, calorieVars, typeSplitLol)
status = solver.SearchForAllSolutions(model, solution_callback)

print(f"\nStatus: {solver.StatusName(status)}")
print(f"Total solutions found: {solution_callback.solution_count()}")