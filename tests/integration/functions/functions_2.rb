def method_1
  1
end

def method_2 x, y
  x * y
end

def method_3(x, y, z)
  puts(x + y)
  puts("entre aca")
  z = x
  z
end

puts(method_1)
puts(method_2(1 + 2, "hola"))

a = method_2(2, 2+1)
puts(a)
puts(a + 1)

puts(b = method_2(3, 3))
puts(b)

b = "hola "
c = method_3(b, "chau", 10)
puts(c)
