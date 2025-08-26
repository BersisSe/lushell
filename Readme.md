# Lushell - Lua Shell

**Lushell** is a command-line shell that embeds the **Lua programming language** directly into the shell environment. It combines classic shell functionality with the power and flexibility of Lua scripting.

## Features

- Classic shell functionality  
- Lua scripting support  
- Extended Lua functions for system calls (written in C)  

## Extended Lua

Lushell introduces a special file extension, **`.lsh`**, which ensures compatibility with its extended Lua functions. These functions make it easy to interact with the file system and environment directly from Lua scripts.

### Built-in Extended Functions

- ```lua
  cd(path) -> boolean|nil, string|nil

Changes the current working directory to `path`.
Returns `true` on success, or `nil` and an error message on failure.

- ```lua
  ls(path) -> table|nil
  ```

  Returns a table of filenames in the directory, or `nil` and an error message on failure.

* ```lua
  pwd() -> string
  ```

  Returns the current working directory as a string.

* ```lua
  mkdir(path) -> boolean
  ```

  Creates a new directory at the given `path`.
  Returns `true` on success, or `false` on failure.

* ```lua
  getenv(name) -> string|nil
  ```

  Gets the value of an environment variable, or `nil` if not set.

* ```lua
  setenv(name, value) -> boolean
  ```

  Sets an environment variable.
  Returns `true` on success, or `false` on failure.

* ```lua
  rm(path) -> boolean|nil, string|nil
  ```

  Removes a file at the given `path`.
  Returns `true` on success, or `nil` and an error message on failure.

* ```lua
  glob(pattern) -> table|nil, string|nil
  ```

  Expands a wildcard pattern into a table of filenames.
  Returns the matches as a table, or `nil` and an error message if no matches are found.

---

## Installation

Lushell is currently available only by building from source:

```bash
git clone https://github.com/BersisSe/lushell.git
cd lushell
make
make install
```

You can also uninstall with:

```bash
make uninstall
```

> An installer package (like `.deb`) is currently in the works.

