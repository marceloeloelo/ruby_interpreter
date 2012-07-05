['A','B','C','D',nil,0].each() do |n|
  puts(n)
end

puts("")

a = [1,2,3]

a.each() do |n|
  puts(n)
end

puts("")

a.each() do |n,k,v|
  puts(n)
end

puts("")

a.each() do |n|
  puts(n * 4)
end

puts("")

res = a.each() do |n|
  b = 3	
  puts(b)	
  puts("hola")
  c = (1 + 2)
  puts(n + c)
end

puts(res)
