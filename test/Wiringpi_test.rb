assert("Wiringpi") do
  Object.const_defined?(:Wiringpi)
end

assert("WiringPi::GPIO") do
  Object.const_defined?(:WiringPi) && WiringPi.const_defined?(:GPIO)
end

