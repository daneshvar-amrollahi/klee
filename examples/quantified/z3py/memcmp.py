from z3 import *
s = Solver()

i = Int('i')
j = Int('j')
n = Int('n')
ret = Int('ret')

I = IntSort()
a = Array('a', I, I)
b = Array('b', I, I)

s.add(
	Implies(ret == 0, 
		ForAll(i, 
			Implies(i < n, 
				Select(a, i) == Select(b, i))
		)
	)
)

s.add(
	Implies(ret == 1,
		Exists(i, 
			And(i < n,
				Select(a, i) != Select(b, i)
			)
		)
	)
)

s.add(n == 5)
s.add(ret == 1)



print(s.check())
print(s.model())
