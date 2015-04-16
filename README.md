lua2exe
=======
You can use this to create an executable from *any* Lua script by concatenating
the `lua2exe` executable and the given Lua script file.

```bash
cat lua2exe helloworld.lua > helloworld
chmod +x helloworld
./helloworld
```

Command Line Arguments
----------------------
Any command line arguments passed to the resulting executable can be read
from within the Lua script by accessing the `os.argv` table.

```bash
./helloworld lola dog
```

```lua
os.argv[1] == "lola"
os.argv[2] == "dog"
```

Disclaimer
----------
This is a proof of concept and shouldn't be considered production ready code
by any means. Consider yourself warned.

License
-------
Mihail Szabolcs, 2015 - Released into the public domain.
