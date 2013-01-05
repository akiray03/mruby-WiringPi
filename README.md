mruby-WiringPi
=========

## install by mrbgems
 - add conf.gem line to `build_config.rb`
```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :git => 'https://github.com/akiray03/mruby-WiringPi.git'
end
```

## example
```ruby
io = WiringPi::GPIO.new
io.write(pin,value)
io.read(pin,value)
```

 - WiringPi::Serial is not tested...

## License
This software is licensed under the same license terms of WiringPi (https://github.com/WiringPi/WiringPi).

