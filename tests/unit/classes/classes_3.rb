class Hola
  attr_accessor :hola
  attr_reader :gola
  attr_writter :gola
end

class Foo
  attr_accessor :foo, :bar
  attr_accessor :fooo

  def bar
    true
  end
end

Hola.new()
a = Foo.new()
