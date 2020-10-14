APPLICATION = cwa-tracker
RIOTBASE ?= $(CURDIR)/../RIOT
BOARD ?= particle-xenon

DEVELHELP ?= 0
QUIET ?= 1

# define dependencies
USEPKG += nimble
USEMODULE += nimble_scanner
USEMODULE += bluetil_addr

USEMODULE += random
USEMODULE += periph_rtt

USEMODULE += vfs
USEMODULE += fatfs_vfs
USEMODULE += mtd_sdcard

USEMODULE += fmt
USEMODULE += xtimer
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

# USEPKG += u8g2
$(info tach)

# define pin and other board specific configuration for specific boards
ifeq (particle-xenon,$(BOARD))
  $(info Moin xenon)
  PROGRAMMER = jlink
  USEMODULE += stdio_rtt
  TERMPROG ?= $(RIOTTOOLS)/jlink/jlink.sh
  TERMFLAGS ?= term-rtt

  CFLAGS += -DSDCARD_SPI_PARAM_SPI=SPI_DEV\(0\)
  CFLAGS += -DSDCARD_SPI_PARAM_CS=GPIO_PIN\(0,31\)
  CFLAGS += -DSDCARD_SPI_PARAM_CLK=GPIO_PIN\(1,15\)
  CFLAGS += -DSDCARD_SPI_PARAM_MOSI=GPIO_PIN\(1,13\)
  CFLAGS += -DSDCARD_SPI_PARAM_MISO=GPIO_PIN\(1,14\)

#   CFLAGS +=
endif
ifeq (samr21-xpro,$(BOARD))
  $(info Moin samr21)

  CFLAGS += -DSDCARD_SPI_PARAM_SPI=SPI_DEV\(0\)
  CFLAGS += -DSDCARD_SPI_PARAM_CS=GPIO_PIN\(0,31\)
  CFLAGS += -DSDCARD_SPI_PARAM_CLK=GPIO_PIN\(0,15\)
  CFLAGS += -DSDCARD_SPI_PARAM_MOSI=GPIO_PIN\(0,13\)
  CFLAGS += -DSDCARD_SPI_PARAM_MISO=GPIO_PIN\(0,14\)
endif


include $(RIOTBASE)/Makefile.include
