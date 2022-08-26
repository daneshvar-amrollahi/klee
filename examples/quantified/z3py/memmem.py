from z3 import *
s = Solver()

i = Int('i')
j = Int('j')
needle_len = Int('needle_len')
haystack_len = Int('haystack_len')
ret = Int('ret')

I = IntSort()
needle = Array('needle', I, I)
haystack = Array('haystack', I, I)

s.add(
	Implies(ret == haystack_len, 
		ForAll(i, 
			Implies(i < haystack_len - needle_len, 
				Exists(j, 
					And(And(0 <= j, j < needle_len),
						Select(needle, j) != Select(haystack, i + j)
					)
				)
			)
		)
	)
)

s.add(
	Implies(ret < haystack_len,
		And(
			ForAll(i, 
				Implies(
					And(0 <= i, i < needle_len), 
					Select(haystack, ret + i) == Select(needle, i)
				)
			), 
			ForAll(i, 
				Implies(i < ret, 
					Exists(j, 
						And(
							And(0 <= j, j < needle_len),
							Select(haystack, j + i) != Select(needle, j)
						)
					)
				)
			)
		)
	)
)

s.add(haystack_len == 5)
s.add(needle_len == 2)
s.add(ret == 2)

# should fail on this
# s.add(Select(haystack, 0) == Select(needle, 0)) 
# s.add(Select(haystack, 1) == Select(needle, 1)) 

# should fail on this
# s.add(Select(haystack, 1) == Select(needle, 0)) 
# s.add(Select(haystack, 2) == Select(needle, 1)) 


s.add(Select(haystack, 2) == Select(needle, 0))
s.add(Select(haystack, 3) == Select(needle, 1))


print(s.check())
print(s.model())
