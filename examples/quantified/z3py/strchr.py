from z3 import *
s = Solver()

c = Int('c')
i = Int('i')
j = Int('j')
n = Int('a_len')
ret = Int('ret')

I = IntSort()
a = Array('a', I, I)

s.add(
	Implies(ret == n, 
		ForAll(i, 
			Implies(i < n, 
				Select(a, i) != c)
		)
	)
)

s.add(
	Implies(ret < n,
		And(ForAll(j,
				Implies(j < ret,
					Select(a, j) != c)
			),
			Select(a, ret) == c
		)
	)
)

s.add(n == 5)
# s.add(ret == 0)

s.add(ret == 2)
# s.add(Select(a, 0) == c) # must fail
# s.add(Select(a, 1) == c) # must fail

print(s.check())
print(s.model())
