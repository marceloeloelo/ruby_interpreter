=begin
puts "foo"

d.ladrar
d.ladrar x
d.ladrar 'foo'
d.ladrar 8
d.saludar "hola"
d.saludar "foo", "bar"
d.saludar("foo")
d.saludar("foo", "bar")

# con bloques

d.each do |n| 
  puts 'foo'
end

d.foo(nil) do |n|
  puts 'bar'
end

d.foo do |n|
  puts 'bar'
end

foo('bar','bar') do |n|
 true
end

d.foo do |k,v|
  puts k
  puts v
end

foo(nil) do |k, v, i|
  true
end
=end
