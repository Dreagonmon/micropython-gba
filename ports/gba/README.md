# Gameboy Advance Port

# Requirement

- [devkitPRO (gba-dev)](https://devkitpro.org/wiki/Getting_Started)
- [mgba](https://mgba.io)

# Build

```bash
make
```

the file will be located at `build/micropython.gba`

# Test Run

```bash
make clean && make -j8 run
```
