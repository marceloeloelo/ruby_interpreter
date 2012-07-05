# define la clase Perro
class Perro

  def initialize(raza, nombre)
    # atributos
    @raza = raza
    @nombre = nombre
  end

  # metodo ladrar
  def ladrar
    puts('Guau! Guau!')
  end

  # metodo saludar
  def saludar
    raza = @raza
    puts("Mi raza es #{raza}")
  end
end

a = Perro.new("Labrador", "Chicho")
#puts(a)
#puts(Perro.new())
puts(a.saludar())
