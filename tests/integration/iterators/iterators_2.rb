var = 1

encontre = false
while ( var && var && !encontre )
  encontre = true
  puts ("bien")
end

encontre = false
while ( (var || var) && !encontre)
  encontre = true
  puts ("bien")
end

while ( var && nil )
  puts ("mal")
end

encontre = false
while ( (var || nil) && !encontre )
  encontre = true
  puts ("bien")
end

encontre = false
while ( !(var && nil || nil && 1) && !encontre )
  encontre = true
  puts ("bien")
end

encontre = false
while ( (var >= 1 || var <= 1) && !encontre )
  encontre = true
  puts ("bien")
end

encontre = false
while ( (var > 5 && var >= 10 && var == nil || var) &&  !encontre )
  encontre = true
  puts ("bien")
end

encontre = false
cont = 0
while (!encontre)
  if ( (var > 5 && var >= 10 && var == nil || var) && cont == 3 )
	encontre = true
  end
  cont += 1
  puts (cont)
end

