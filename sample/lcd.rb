# coding: utf-8

class LCD
  LCD_CLEAR   = 0x01
  LCD_HOME    = 0x02
  LCD_ENTRY   = 0x04
  LCD_ON_OFF  = 0x08
  LCD_CDSHIFT = 0x10
  LCD_FUNC    = 0x20
  LCD_CGRAM   = 0x40
  LCD_DGRAM   = 0x80

  LCD_ENTRY_SH = 0x01
  LCD_ENTRY_ID = 0x02

  LCD_ON_OFF_B = 0x01
  LCD_ON_OFF_C = 0x02
  LCD_ON_OFF_D = 0x04

  LCD_FUNC_F  = 0x04
  LCD_FUNC_N  = 0x08
  LCD_FUNC_DL = 0x10

  LCD_CDSHIFT_RL = 0x04

  LEVEL_LOW   = WiringPi::LOW
  LEVEL_HIGH  = WiringPi::HIGH
  MODE_INPUT  = WiringPi::INPUT
  MODE_OUTPUT = WiringPi::OUTPUT

  def initialize(args = {})
    @rows = args[:rows].to_i
    @cols = args[:cols].to_i
    @bits = args[:bits].to_i
    @rs   = args[:rs].to_i
    @strb = args[:strb].to_i
    @dpin = args[:dpin]
    @lcds = []

    unless @bits == 4 or @bits == 8
      raise "Invalid bits"
    end
    unless @rows > 0 && @rows <= 20
      raise "Invalid rows"
    end
    unless @cols > 0 && @cols <= 20
      raise "Invalid cols"
    end
    unless @dpin.class == Array
      raise "Invalid dpin (not Array)"
    end
    unless @dpin.size == @bits
      raise "invalid dpin length"
    end

    @gpio = WiringPi::GPIO.new
    lcd_init
  end

  def sleep(i)
    if Object.const_defined? :Sleep
      Sleep.sleep(i)
    else
      sleep(i)
    end
  end

  def msleep(i)
    usleep(i * 1000.0)
  end

  def usleep(i)
    if Object.const_defined? :Sleep
      Sleep.usleep(i)
    else
      sleep(i/1000.0/1000.0)
    end
  end

  def strobe
    @gpio.write @strb, LEVEL_HIGH
    usleep 50
    @gpio.write @strb, LEVEL_LOW
    usleep 50
  end

  def send_data_cmd(data)
    @dpin.each_with_index do |pin, idx|
      val = data & (2 << (idx-1)) > 0 ? LEVEL_HIGH : LEVEL_LOW
      @gpio.write pin, val
    end
    strobe
  end

  def put_command(cmd)
    @gpio.write @rs, LEVEL_LOW
    send_data_cmd cmd
  end

  def lcd_init
    @gpio.write @rs, LEVEL_LOW
    @gpio.mode  @rs, MODE_OUTPUT
    @gpio.write @strb, LEVEL_LOW
    @gpio.mode  @strb, MODE_OUTPUT

    @dpin.each do |pin|
      @gpio.write pin, LEVEL_LOW
      @gpio.mode  pin, MODE_OUTPUT
    end

    msleep(35)

    func = LCD_FUNC | LCD_FUNC_DL
    put_command func
    msleep(35)
    put_command func
    msleep(35)
    put_command func
    msleep(35)

    if @rows > 1
      func |= LCD_FUNC_N
      put_command func
      msleep(35)
    end

    put_command LCD_ON_OFF | LCD_ON_OFF_D
    msleep 2
    put_command LCD_ENTRY | LCD_ENTRY_ID
    msleep 2
    put_command LCD_CDSHIFT | LCD_CDSHIFT_RL
    msleep 2
    put_command LCD_CLEAR
    msleep 10

    self
  end

  def lcd_pos(pos_x, pos_y)
    row_offset = [0x00, 0x40, 0x14, 0x54]
    put_command pos_x + (LCD_DGRAM | row_offset[pos_y])
  end

  def lcd_putchar(char)
    @gpio.write @rs, LEVEL_HIGH
    send_data_cmd char
  end

  def lcd_puts(str)
    str.each_byte do |char|
      lcd_putchar char
    end
  end
end

if $0 == __FILE__
  lcd = LCD.new rows: 18, cols:2, bits: 8, rs: 13, strb: 12, dpin: [0,1,2,3,4,5,6,7]
  p lcd

  lcd.lcd_pos 0, 0
  lcd.lcd_puts "Hello, mruby"
  lcd.lcd_pos 0, 1
  lcd.lcd_puts "Hi, Raspberry Pi"
end
