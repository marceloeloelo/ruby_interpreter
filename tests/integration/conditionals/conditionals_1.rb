x = 0

if false
  x = -1
end

puts x # 0

if true
  x = 1
end

puts x # 1

if (x > 0)
  x = 2
else
  x = -2
end

puts x # 2

if (x == 4)
  x = 2
elsif (x == -4)
  x = -2
else
  x = 0
end

puts x # 0

x = 0

if false
  x = 0
elsif (x == 1)
  x = 0
elsif x == 0
  x = 3
end

puts x # 3

if false
  x = 0
elsif (x == 1)
  x = 0
elsif x == 0
  x = 0
else 
  x = 0
end

puts x # 0

if x == 0
  # puts x
  puts x # 0
  puts "hola"
end
