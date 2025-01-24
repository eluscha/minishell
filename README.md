
# Minishell

A basic implementation of a Unix shell created as a programming exercise for the 42berlin core curriculum. 
This project was designed to deepen our understanding of process management, system calls, and terminal interaction in a Unix-like environment.


---

## Authors

- **Eleonora Winkler** - [GitHub Profile](https://github.com/eluscha)
- **auspens** - [GitHub Profile](https://github.com/auspens)

---

## Features

- Execute built-in shell commands like `cd`, `exit`, and `pwd`.
- Support for environment variables, including built-in commands to modify them.
- Execute external commands found using $PATH.
- Support for input/output redirection (`>`, `<`, `>>`, `<<`).
- Support for piping (`|`).
- Support for multiline input (open quotes/unfunushed piping).
- Handles `Ctrl+C`, `Ctrl+\`, and `Ctrl+D` gracefully.
- Interactive and non-interactive modes.
- Command history support.

---

## Limitations

- Does not handle advanced shell features like job control (`fg`, `bg`).
- Limited error handling for edge cases.
- Does not support command chaining with `;`.
- Does not support conditional execution with `&&` or `||`.

---

## Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/eluscha/minishell.git minishell
   cd minishell
   ```

2. Compile the program using provided Makefile:

   ```bash
   make
   ```

3. Run the shell:

   ```bash
   ./minishell
   ```

---

## Usage

- Launch the shell and start typing commands.
- For example:

  ```
  $ ls -l
  $ cd /home/user
  $ echo "Hello, World!" > output.txt
  $ cat output.txt | grep Hello
  ```

- Use `exit` to leave the shell.


---

## Built With

- **C Language** - The core programming language used.
- **POSIX System Calls** - For managing processes, file descriptors, and more.

---


