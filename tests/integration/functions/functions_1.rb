def method_1
end

def method_2 x
  x
end

def method_3 x, y
  y
end

def method_4(x)
  "hola"
end

def method_5(x,y,z)
  x
  y
  z
end

x = y = z = 5

method_1()

method_2(x)
puts(method_2(x))
puts(method_2(1))

method_3(x, y)
puts(method_3(x, y))
puts(method_3(2, y))

method_4(x)
puts(method_4(x))
puts(method_4(2))

method_5(x, y, z)
puts(method_5(x, y, z))
puts(method_5(2, y, 1))
