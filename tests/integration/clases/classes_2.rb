class Auto
	attr_reader :marca
	attr_reader :anio
	attr_accessor :duenio
	attr_accessor :colores_disponibles

	puts("entre a la clase")

	def initialize marca, anio
		@marca = marca
		@anio  = anio
		@colores_disponibles = ["Rojo", "Negro", "Azul"]
	end

	# Genero Setter que no tengo sin attr_writter
	def set_marca(marca)
		@marca = marca
	end

	def info
		puts("________INFO_________")
		if (@duenio.nil?())
		  nombre_duenio = "Nadie"
		else
		  nombre_duenio = @duenio
		end
		puts("marca: " + @marca + ", anio: " + @anio, "duenio: " + nombre_duenio)
		@colores_disponibles.each() do |color|
		  puts(color)
		end
		puts("________INFO_________")
	end

	def poner_rueda(rueda)
		if rueda.rodado_uno?()
			puts("puse rueda")
		else 
			puts("Este auto solo acepta rodado uno")
		end
	end

end

##### Crear Auto #####
puts("")
puts("")
auto = Auto.new("Chevrolet", 1998)
#puts(auto)
puts(auto.marca())
auto.info()

#### Setear marca y duenio ####
puts("")
puts("")
auto.set_marca("Ford")
auto.set_duenio("Juan Gonzalez")
auto.info()

#### Checkeando respond_to ####
puts("")
puts("")
if (auto.respond_to?('set_anio'))
	puts("MAL")
else
	puts("BIEN")
end

#### Seteando colores ####
puts("")
puts("")
auto.set_colores_disponibles(["Verde", "Amarillo"])
puts(auto.colores_disponibles())

#### Creo una nueva clase ####
puts("")
puts("")
class Rueda
	attr_accessor :rodado

	def initialize(rodado)
		@rodado = rodado
	end

	def rodado_uno?
		@rodado == 1
	end

	def rodado_dos?
		@rodado == 2
	end
end

rueda1 = Rueda.new(1)
rueda2 = Rueda.new(2)

auto.poner_rueda(rueda2) # No
auto.poner_rueda(rueda1) # Si
