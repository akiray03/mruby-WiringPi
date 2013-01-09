
io = WiringPi::GPIO.new
pin = 0
io.mode(pin, WiringPi::OUTPUT)

5.times do
  puts "pin:#{pin} set high"
  io.write(pin, WiringPi::HIGH)
  sleep 1
  puts "pin:#{pin} set low"
  io.write(pin, WiringPi::LOW)
  sleep 1
end
