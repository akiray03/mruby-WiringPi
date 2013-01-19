mruby-WiringPi
=========

## How to use
 - checkout mruby

```bash
git clone https://github.com/mruby/mruby.git  # original mruby
  # or
git clone https://github.com/iij/mruby.git    # iij extended mruby (File, IO, Socket, ... extend)
```

  - edit `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :git => 'https://github.com/akiray03/mruby-WiringPi.git'  # add this line
end
```

 - build

```bash
ruby ./minirake
```

 - exec

```bash
sudo ./bin/mruby build/mrbgems/mruby-WiringPi/sample/sample.rb
```

## sample

```ruby
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
io.read(pin)
```

 - WiringPi::Serial is not tested...

## License
This software is licensed under the same license terms of WiringPi (https://github.com/WiringPi/WiringPi).

