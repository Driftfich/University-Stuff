from ortools.init.python import init
from ortools.sat.python import cp_model
from ortools.linear_solver import pywraplp

length = 20
precision = 100

# sat model
model = cp_model.CpModel()

half: int = int((length * precision) / 2)

# variables
s = model.new_int_var(-half, half, "s")
m = model.new_int_var(-half, half, "m")
l = model.new_int_var(-half, half, "l")

# constraints
# not in the middle
model.add(s != 0)
model.add(m != 0)
model.add(l != 0)

# distance between all 1 * precision
model.add_abs_equality(precision, m-s)
model.add_abs_equality(precision, m-l)
model.add_abs_equality(precision, l-s)

model.add((s * 12) + (m * 30) + (l * 40) == 0)

solver = cp_model.CpSolver()
status = solver.Solve(model)

if status == cp_model.OPTIMAL or status == cp_model.FEASIBLE:
    print(f"Status: {solver.StatusName(status)}")
    print(f"Solution: {solver.Value(s) / precision}")
    print(f"Solution: {solver.Value(m) / precision}")
    print(f"Solution: {solver.Value(l) / precision}")
else:
    print(f"Status: {solver.StatusName(status)}")
    print("No solution found.")
