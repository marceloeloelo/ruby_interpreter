# int vs int

1 <=> 2
puts(1 <=> 2)
2 <=> 1
puts(2 <=> 1)
2 <=> 2
puts(2 <=> 2)

# double vs double

1.1 <=> 2.1
puts(1.1 <=> 2.1)
2.1 <=> 1.1
puts(2.1 <=> 1.1)
2.1 <=> 2.1
puts(2.1 <=> 2.1)

# int vs double

1 <=> 2.1
puts(1 <=> 2.1)
1.1 <=> 2
puts(1.1 <=> 2)
1 <=> 1.0
puts(1 <=> 1.0)

# string vs string

"foo" <=> "bar"
puts("foo" <=> "bar")
"bar" <=> "foo"
puts("bar" <=> "foo")
"foo" <=> "foo"
puts("foo" <=> "foo")
"" <=> "foo"
puts("" <=> "foo")
"" <=> ""
puts("" <=> "")

# string vs int

"foo" <=> 1
puts("foo" <=> 1)
2 <=> "foo"
puts(2 <=> "foo")
2 <=> "2"
puts(2 <=> "2")

# string vs double

2.1 <=> "foo"
puts(2.1 <=> "foo")
"foo" <=> 1.1
puts("foo" <=> 1.1)
"1.1" <=> 1.1
puts("1.1" <=> 1.1)

# bool vs bool

true <=> false
puts(true <=> false)

 
