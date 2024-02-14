BUILD = ./build
EXE = $(BUILD)/pong-wars

.PHONY: all build config run clean

all: build

build: 
	meson compile -C $(BUILD)

config:
	[ -d "$(BUILD)" ] && meson setup $(BUILD) --reconfigure || meson setup $(BUILD)

run:
	$(EXE)

clean:
	meson compile -C $(BUILD) --clean
