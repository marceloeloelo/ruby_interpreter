#lenth
puts ("Hello".length())

puts ("H".length() * 2)

puts ("Hello".length() + "Goodbye".length())

puts ("".length())

#nil?
n = "hola".nil?()
puts(n)
puts(1.nil?())
class Algo
end
a = Algo.new()
puts(a.nil?())
puts(nil.nil?())

#respond_to?
a = "hola".respond_to?("length")
puts(a)
a = "hola".respond_to?("fruta")
puts(a)
a = ["das", "asd"]
puts(a.respond_to?("length"))
a = ["das", "asd"]
puts(a.respond_to?("fruta"))
b = a[0]
puts(b.respond_to?("length"))
puts(b.respond_to?("fruta"))
puts(1.respond_to?("nil?"))
puts(1.respond_to?("fruta"))
class Custom
  def method_1
  end
end
c = Custom.new()
puts(c.respond_to?("method_1"))
puts(c.respond_to?("fruta"))
puts(c.respond_to?("fruta", "asda"))

