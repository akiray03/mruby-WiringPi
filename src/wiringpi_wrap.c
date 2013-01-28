/*
** wiring_wrap.c - WiringPi Wrapper class
**
** See Copyright Notice in mruby.h
*/

#include "mruby.h"

#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/string.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <gertboard.h>

#ifndef TRUE
#  define	TRUE	(1==1)
#  define	FALSE	(1==2)
#endif

mrb_value
mrb_WiringPi_Setup(mrb_state *mrb, mrb_value self)
{
  if (wiringPiSetup() == -1) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "WiringPiSetup failed");
    return mrb_nil_value();
  }

  return self;
}

mrb_value
mrb_WiringPi_SetupSys(mrb_state *mrb, mrb_value self)
{
  if (wiringPiSetupSys() == -1) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "WiringPiSetupSys failed");
    return mrb_nil_value();
  }

  return self;
}

mrb_value
mrb_WiringPi_SetupGpio(mrb_state *mrb, mrb_value self)
{
  if (wiringPiSetupGpio() == -1) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "WiringPiSetupGpio failed");
    return mrb_nil_value();
  }

  return self;
}

mrb_value
mrb_WiringPi_SetupPiFace(mrb_state *mrb, mrb_value self)
{
  if (wiringPiSetupPiFace() == -1) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "WiringPiSetupPiFacefailed");
    return mrb_nil_value();
  }

  return self;
}

mrb_value
mrb_WiringPi_pinMode(mrb_state *mrb, mrb_value self)
{
  char *m;
  mrb_int pin;
  mrb_value mode;
  mrb_get_args(mrb, "iS", &pin, &mode);

  m = RSTRING_PTR(mode);

  /**/ if (strcasecmp (m, "in")   == 0) pinMode         (pin, INPUT) ;
  else if (strcasecmp (m, "out")  == 0) pinMode         (pin, OUTPUT) ;
  else if (strcasecmp (m, "pwm")  == 0) pinMode         (pin, PWM_OUTPUT) ;
  else if (strcasecmp (m, "up")   == 0) pullUpDnControl (pin, PUD_UP) ;
  else if (strcasecmp (m, "down") == 0) pullUpDnControl (pin, PUD_DOWN) ;
  else if (strcasecmp (m, "tri")  == 0) pullUpDnControl (pin, PUD_OFF) ;
  else
  {
    mrb_raisef(mrb, E_RUNTIME_ERROR, "Invalid mode: %s. Should be in/out/pwm/up/down/tri", m) ;
  }


  return self;
}

mrb_value
mrb_WiringPi_digitalWrite(mrb_state *mrb, mrb_value self)
{
  mrb_int pin, val;
  mrb_get_args(mrb, "ii", &pin, &val);

  if (val == 0)
    digitalWrite(pin, LOW);
  else
    digitalWrite(pin, HIGH);

  return self;
}

mrb_value
mrb_WiringPi_digitalRead(mrb_state *mrb, mrb_value self)
{
  mrb_int pin;
  mrb_value val = mrb_nil_value();
  mrb_get_args(mrb, "i", &pin);

  val = mrb_fixnum_value(digitalRead(pin));

  return val;
}

mrb_value
mrb_WiringPi_pwmWrite(mrb_state *mrb, mrb_value self)
{
  mrb_int pin;
  mrb_float val;
  mrb_get_args(mrb, "if", &pin, &val);

  pwmWrite(pin, val);
  return self;
}

mrb_value
mrb_WiringPi_serialOpen(mrb_state *mrb, mrb_value self)
{
  mrb_value device;
  mrb_int baud;
  char *dev;
  mrb_get_args(mrb, "Si", &device, &baud);
  dev = mrb_string_value_ptr(mrb, device);
  return mrb_fixnum_value(serialOpen(dev, baud));
}

mrb_value
mrb_WiringPi_serialClose(mrb_state *mrb, mrb_value self)
{
  mrb_int fd;
  mrb_get_args(mrb, "i", &fd);

  serialClose(fd);
  return self;
}

mrb_value
mrb_WiringPi_serialPutchar(mrb_state *mrb, mrb_value self)
{
  mrb_int fd;
  mrb_value str;
  char *chr;
  mrb_get_args(mrb, "iS", &fd, &str);

  chr = mrb_string_value_ptr(mrb, str);
  serialPutchar(fd, *chr);

  return self;
}

mrb_value
mrb_WiringPi_serialPuts(mrb_state *mrb, mrb_value self)
{
  mrb_int fd;
  mrb_value str;
  char *chr;
  mrb_get_args(mrb, "iS", &fd, &str);

  chr = mrb_string_value_ptr(mrb, str);
  serialPuts(fd, chr);

  return self;
}

mrb_value
mrb_WiringPi_serialPrintf(mrb_state *mrb, mrb_value self)
{
  mrb_int argc;
  mrb_value *argv;
  mrb_int fd;
  mrb_value str;
  char *chr;

  mrb_get_args(mrb, "*", &argv, &argc);
  if (argc <= 1) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "too few arguments");
    return mrb_nil_value();
  } else {
    if (mrb_type(argv[0]) != MRB_TT_FIXNUM) {
      mrb_raise(mrb, E_TYPE_ERROR, "invalid argument type of 1st. 1st argument requires fixnum.");
      return mrb_nil_value();
    }
    fd = mrb_fixnum(argv[0]);
    str = mrb_str_format(mrb, argc - 2, argv + 2, argv[1]);
  }

  chr = mrb_string_value_ptr(mrb, str);
  serialPuts(fd, chr);
  return self;
}

mrb_value
mrb_WiringPi_serialDataAvail(mrb_state *mrb, mrb_value self)
{
  mrb_int fd;
  mrb_get_args(mrb, "i", &fd);
  return mrb_fixnum_value(serialDataAvail(fd));
}

mrb_value
mrb_WiringPi_serialGetchar(mrb_state *mrb, mrb_value self)
{
  mrb_int fd;
  mrb_get_args(mrb, "i", &fd);
  return mrb_fixnum_value(serialGetchar(fd));
}

void
mrb_mruby_WiringPi_gem_final(mrb_state *mrb)
{
}

void
mrb_mruby_WiringPi_gem_init(mrb_state *mrb)
{
  struct RClass *wpi;

  wpi = mrb_define_class(mrb, "Wiringpi", mrb->object_class);

  mrb_define_class_method(mrb, wpi, "wiringPiSetup",       mrb_WiringPi_Setup,        ARGS_NONE());
  mrb_define_class_method(mrb, wpi, "wiringPiSetupSys",    mrb_WiringPi_SetupSys,     ARGS_NONE());
  mrb_define_class_method(mrb, wpi, "wiringPiSetupGpio",   mrb_WiringPi_SetupGpio,    ARGS_NONE());
  mrb_define_class_method(mrb, wpi, "wiringPiSetupPiFace", mrb_WiringPi_SetupPiFace,  ARGS_NONE());
  mrb_define_class_method(mrb, wpi, "pinMode",             mrb_WiringPi_pinMode,      ARGS_REQ(2));
  mrb_define_class_method(mrb, wpi, "digitalWrite",        mrb_WiringPi_digitalWrite, ARGS_REQ(2));
  mrb_define_class_method(mrb, wpi, "digitalRead",         mrb_WiringPi_digitalRead,  ARGS_REQ(1));
  mrb_define_class_method(mrb, wpi, "pwmWrite",            mrb_WiringPi_pwmWrite,     ARGS_REQ(2));

  mrb_define_class_method(mrb, wpi, "serialOpen",        mrb_WiringPi_serialOpen,      ARGS_REQ(2));
  mrb_define_class_method(mrb, wpi, "serialClose",       mrb_WiringPi_serialClose,     ARGS_REQ(1));
  mrb_define_class_method(mrb, wpi, "serialPutchar",     mrb_WiringPi_serialPutchar,   ARGS_REQ(2));
  mrb_define_class_method(mrb, wpi, "serialPuts",        mrb_WiringPi_serialPuts,      ARGS_REQ(2));
  mrb_define_class_method(mrb, wpi, "serialPrintf",      mrb_WiringPi_serialPrintf,    ARGS_ANY());
  mrb_define_class_method(mrb, wpi, "serialDataAvail",   mrb_WiringPi_serialDataAvail, ARGS_REQ(1));
  mrb_define_class_method(mrb, wpi, "serialGetchar",     mrb_WiringPi_serialGetchar,   ARGS_REQ(1));

}
