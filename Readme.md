# Lushell - Lua Shell

**Lushell** is a command-line shell that embeds the **Lua programming language** directly into the shell environment. It combines classic shell functionality with the power and flexibility of Lua scripting.

## Features

- Classic shell functionality
- Script execution using Lua
- Extended Lua functions for system calls (written in C)

## Extended Lua

Lushell introduces a special file extension, **`.lsh`**, which ensures compatibility with its extended Lua functions. These functions make it easy to interact with the file system directly from Lua scripts.

### Built-in Extended Functions

- ```lua
  cd(path)
  ```

  Changes the current working directory to `path`.

- ```lua
  ls(path)
  ```

  Returns a table of files and directories found at `path`.

- ```lua
  pwd()
  ```

  Returns the current working directory.

- ```lua
  mkdir(path)
  ```

  Creates a new directory at the given `path`.

---

## Example `.lsh` Script

```lua
-- example.lsh

print("Current directory: " .. pwd())

mkdir("test_dir")
cd("test_dir")

print("Now inside: " .. pwd())

for _, file in ipairs(ls(".")) do
    print("Found:", file)
end
```
## Installation 
Lushell is currently available only by building from source
```bash
git clone https://github.com/BersisSe/lushell.git
cd lushell
make
make install
```
**You can also use `make uninstall`**

*A installer package(Like .deb) is currently in the works*
