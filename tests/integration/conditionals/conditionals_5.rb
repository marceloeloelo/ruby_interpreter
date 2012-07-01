var = 1

if ( var && var )
  puts ("bien")
else
  puts ("mal")
end

if ( var || var )
  puts ("bien")
else
  puts ("mal")
end

if ( var && nil )
  puts ("mal")
else
  puts ("bien")
end

if ( var || nil )
  puts ("bien")
else
  puts ("mal")
end

if ( var && nil || nil && 1 )
  puts ("mal")
else
  puts ("bien")
end

if ( var && nil || nil && 1 )
  puts ("mal")
elsif ( true )
  puts ("bien")
end

if ( var >= 1 || var <= 1 )
  puts ("bien")
else
  puts ("mal")
end

if ( var > 5 && var >= 10 && var == nil || var )
  puts ("bien")
else
  puts ("mal")
end

