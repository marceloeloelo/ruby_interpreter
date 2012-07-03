class Perro

  attr_accessor :raza

  def initialize(raza)
	@raza = raza
  end

  #def raza=raza
  #	@raza= raza
  #end

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
a.set_raza("Salchicha") #raza= -> ayntax Error!
puts("La nueva raza es: " + a.raza())

puts(nil)

class Gato
  def initialize(raza)
	@raza = raza
	@razaraza = raza*2
  end

  def maullar(maullido)
	puts(maullido)
  end

  def info
	puts(@raza)
	puts(@razaraza)
  end

  def llamar_metodo_de_perro
	ladrar
  end

  def pelear_con_perro(perro)
	puts("Que miras " + perro.raza() + "!");
	puts("5!23!@!")
	perro.ladrar()
	puts("!$$$%%@!!")
	maullar("miau!")
	perro.auyar()
  end

  def crear_perro(raza_perro)
	Perro.new(raza_perro)
  end
end

g = Gato.new("Siames")
g.maullar('miau')
g.info()
#g.llamar_metodo_de_perro() # Deberia explotar
g.pelear_con_perro(a)
g.pelear_con_perro(Perro.new("Bulldog"))
b = g.crear_perro("Pekines")
puts(b.raza())
