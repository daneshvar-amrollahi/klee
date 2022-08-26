from z3 import *
s = Solver()

i = Int('i')
j = Int('j')
str1_len = Int('str1_len')
str2_len = Int('str2_len')
ret = Int('ret')

I = IntSort()
str1 = Array('str1', I, I)
str2 = Array('str2', I, I)

s.add(0 <= ret)
s.add(ret <= str1_len)

s.add(
	Implies(ret == 0,
		Select(str1, 0) != Select(str2, 0)
	)
)

s.add(
	Implies(And(1 <= ret, ret <= str1_len),
		ForAll(i, 
			Implies(And(0 <= i, i < ret),
				Exists(j, 
					And(
						And(0 <= j, j < str2_len),
						Select(str1, i) == Select(str2, j)
					)
				)
			)
		)
	)
)

s.add(
	Implies(ret < str1_len,
		ForAll(i, 
			Implies(And(0 <= i, i < str2_len),
				Select(str2, i) != Select(str1, ret + 1)
			)
		)
	)
)

s.add(ret == 2)
s.add(str1_len == 5)
s.add(str2_len == 2)

# s.add(Select(str1, 0) == 1)
# s.add(Select(str1, 1) == 2)
# s.add(Select(str1, 2) == 3)
# s.add(Select(str1, 3) == 4)
# s.add(Select(str1, 5) == 5)
# s.add(Select(str2, 0) == 2)
# s.add(Select(str2, 1) == 1)




print(s.check())
print(s.model())
