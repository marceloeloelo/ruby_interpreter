# int vs int
#COMENTING, NOT SUPPORTING === OPERATOR
=begin
puts(1 === 2)
puts(2 === 1)
puts(2 === 2)

# double vs double

puts(1.1 === 2.1)
puts(2.1 === 1.1)
puts(2.1 === 2.1)

# int vs double

puts(1 === 2.1)
puts(1.1 === 2)
puts(1 === 1.0)

# string vs string

puts("foo" === "bar")
puts("bar" === "foo")
puts("foo" === "foo")
puts("" === "foo")
puts("" === "")

# string vs int

puts("foo" === 1)
puts(2 === "foo")
puts(2 === "2")

# string vs double

puts(2.1 === "foo")
puts("foo" === 1.1)
puts("1.1" === 1.1)

# bool vs bool

puts(true === false)
puts(true === true)
puts(false === true)
puts(false === false)

# bool vs int

puts(true === 2)
puts(true === 1)
puts(true === 0)
puts(2 === true)
puts(1 === true)
puts(0 === true)

puts(false === 2)
puts(false === 1)
puts(false === 0)
puts(2 === false)
puts(1 === false)
puts(0 === false)

# bool vs nil

puts(true === nil)
puts(nil === true)

puts(false === nil)
puts(nil === false)

# bool vs string

puts(true === "foo")
puts(true === "")
puts("foo" === true)
puts("" === true)

puts(false === "foo")
puts(false === "")
puts("foo" === false)
puts("" === false)

# bool vs double

puts(true === 2.1)
puts(true === 1.0)
puts(true === 0.0)
puts(2.1 === true)
puts(1.0 === true)
puts(0.0 === true)

puts(false === 2.1)
puts(false === 1.0)
puts(false === 0.0)
puts(2.1 === false)
puts(1.0 === false)
puts(0.0 === false)

# nil vs nil

puts(nil === nil)

# nil vs int

puts(2 === nil)
puts(1 === nil)
puts(0 === nil)

puts(nil === 2)
puts(nil === 1)
puts(nil === 0)

# nil vs double

puts(2.1 === nil)
puts(1.0 === nil)
puts(0.0 === nil)

puts(nil === 2.1)
puts(nil === 1.0)
puts(nil === 0.0)
=end
 
