#############################################
################## length ###################
#############################################
puts ("hola".length())

b = []

puts (b.length())

a = 'd'

puts ("hola #{a}".length())

a = [1,'chau']

puts (a[1].length())

puts (a.length())

# puts (a[0].length()) NoMethodError: undefined method `length' for 1:Fixnum

puts ("Hello".length())

puts ("H".length() * 2)

puts ("Hello".length() + "Goodbye".length())

puts ("".length())

################################################
##################### nil? #####################
################################################
n = "hola".nil?()
puts(n)
puts(1.nil?())
class Algo
end
a = Algo.new()
puts(a.nil?())
puts(nil.nil?())
a = [1, 2, 3]
a.nil?()
puts(a.nil?())
b = a[0]
b.nil?()
puts(b.nil?())


###############################################
################# respond_to? #################
###############################################
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


###############################################
################# object_id? ##################
###############################################
class Perro
end
a = Perro.new()
b = a.object_id()
#puts(b)
c = a.object_id()
#puts(c)
puts(b == c)

a = [1, 2, 3, 4]
c = a[0]
#puts(c.object_id())
#puts(c.object_id())
puts(c.object_id() == c.object_id())

puts(c.respond_to?("fruta", "asda"))
