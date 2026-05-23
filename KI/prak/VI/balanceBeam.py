from ortools.init.python import init
from ortools.sat.python import cp_model
from ortools.linear_solver import pywraplp

length = 20
precision = 100

# sat model
model = cp_model.CpModel()

half: int = int((length * precision) / 2)
max_dist: int = int(length * precision)

# variables
s = model.new_int_var(-half, half, "s")
m = model.new_int_var(-half, half, "m")
l = model.new_int_var(-half, half, "l")

# constraints
# not in the middle
model.add(s != 0)
model.add(m != 0)
model.add(l != 0)

def min_sep(model, a, b, name: str) -> None:
    d = model.new_int_var(0, max_dist, name)
    model.add_abs_equality(d, a - b)
    model.add(d >= precision)

min_sep(model, s, m, "dist_sm")
min_sep(model, m, l, "dist_ml")
min_sep(model, s, l, "dist_sl")

model.add((s * 12) + (m * 30) + (l * 40) == 0)

solver = cp_model.CpSolver()
status = solver.Solve(model)

if status == cp_model.OPTIMAL or status == cp_model.FEASIBLE:
    print(f"Status: {solver.StatusName(status)}")
    print(f"Solution s: {solver.Value(s) / precision}")
    print(f"Solution m: {solver.Value(m) / precision}")
    print(f"Solution l: {solver.Value(l) / precision}")
else:
    print(f"Status: {solver.StatusName(status)}")
    print("No solution found.")
