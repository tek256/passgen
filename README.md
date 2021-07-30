<div id="header">
    <p align="center">
      <b>passgen</b><br>
  	  <span font-size="16px">a cli password generator</span><br>
      <span font-size="12px">Made by <a href="http://tek256.com">Devon</a> with love.</span><br><br>
      <span><a href="#usage">Usage</a> | <a href="https://discordapp.com/invite/63GvpMh">Discord</a> | <a href="https://github.com/sponsors/tek256">Support</a></span><br><br>
    </p>
</div>

### About
passgen is meant to be a simple but useful password generator that is easily deployable onto machines without many dependencies.

### Usage

```
Usage:
./passgen [h/uu/un/us/l/s/o]
  --use_nums, -use_nums, -un Use numbers in the generator (on by default)
  --use_upper, -use_upper, -uu Use uppercase in the generator (on by default)
  --use_symbols, -use_symbols, -us Use symbols in the generator (on by default)
  --length=x, -length=x, -len=x Generate specific length password (16 by default)
  --seed=x, -seed=x, -s=x Use specific seed
  --output=x, -out=x, -o=x Output to specific location
  --default, -default, -d Use defaults 
  --help, -help, -h Output this help info
```

### Building
passgen currently just uses a makefile to compile. You can also just substitute it out for your favorite compiler call using just the `main.c` file in the `src/` directory. 

Otherwise it's as easy just using: 
```
make
``` 

and you'll have a `passgen` executable.


