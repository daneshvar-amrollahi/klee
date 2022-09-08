from z3 import *
s = Solver()

i = BitVec('i', 32)
j = BitVec('j', 32)
ret = BitVec('ret', 32)

BV32 = BitVecSort(32)
BV8 = BitVecSort(8)
a = Array('s', BV32, BV8)



s.add(
	ForAll(i, 
		Implies(
			And(0 <= i, i < ret),
			Select(a, i) != 0
		)
	)
)

s.add(
	Select(a, ret) == 0
)


s.add(ret == 3)
s.add(Select(a, 0) == 1)
s.add(Select(a, 1) == 3)
s.add(Select(a, 2) == 2)
# s.add(Select(a, 3) != 0)

print(s.sexpr())

print(s.check())
print(s.model())