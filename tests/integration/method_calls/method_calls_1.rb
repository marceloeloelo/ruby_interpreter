class Perro

  attr_reader :raza

  def initialize(raza)
	@raza= raza
  end

  def ladrar
	puts ("guau!")
  end

  def auyar
	puts('auuuuu!')
  end
  
  def ruido
	ladrar
	auyar
  end

  def saltar(cant)
	i = 1
	while (i <= cant)
	  puts("salto!: " + i)
	  i = i + 1
	end
  end

  def info
	puts(@raza)
  end

  def pluralize_raza
	@raza = @raza + "s"
  end
end

a = Perro.new("Labrador")
puts(a.raza())
a.info();
a.ladrar()
a.auyar()
a.ruido()
a.saltar(3)
a.pluralize_raza()
puts(a.raza())
#a.raza= ("Salchicha") Syntax Error!
#puts(a.raza())

