512.times do |i|
  s = i
  a = []
  b = []
  9.times do 
    a.push (s&1)
    b.push (s&1) == 1
    s = s >> 1
  end
  b01a = b[0] ^ b[1]
  b01b = !(b[0] | b[1])
  b23a = b[2] ^ b[3]
  b23b = !(b[2] | b[3])
  b03a = (b01a & b23b) | (b01b & b23a)
  b03b = b01b & b23b

  b45a = b[4] ^ b[5]
  b45b = !(b[4] | b[5])
  b67a = b[6] ^ b[7]
  b67b = !(b[6] | b[7])
  b47a = (b45a & b67b) | (b45b & b67a)
  b47b = b45b & b67b
  b07a = (b03a & b47b) | (b03b & b47a)
  b07b = b03b & b47b
  b08 = (b07a & !b[8]) | (b07b & b[8])

  next if !b08
  puts "#{a.join("")}"
end
