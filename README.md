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

## Pin Mapping

| wiringPi | GPIO (Pin#) | Name   |   
|:--------:|:----:|:-------|
|      0   |  17  | GPIO 0 |
|      1   |  18  | GPIO 1 | 
|      2   |  27  | GPIO 2 | 
|      3   |  22  | GPIO 3 | 
|      4   |  23  | GPIO 4 | 
|      5   |  24  | GPIO 5 | 
|      6   |  25  | GPIO 6 | 
|      7   |   4  | GPIO 7 | 
|      8   |   2  | SDA    |   
|      9   |   3  | SCL    |   
|     10   |   8  | CE0    |   
|     11   |   7  | CE1    |   
|     12   |  10  | MOSI   |   
|     13   |   9  | MISO   |   
|     14   |  11  | SCLK   |   
|     15   |  14  | TxD    |   
|     16   |  15  | RxD    |   
|     17   |  28  | GPIO 8 | 
|     18   |  29  | GPIO 9 | 
|     19   |  30  | GPIO10 |
|     20   |  31  | GPIO11 |

## TODO

 - cross build support

## License
This software is licensed under the same license terms of WiringPi (https://github.com/WiringPi/WiringPi).

