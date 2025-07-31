# 🐚 Minishell <img src="https://42.fr/wp-content/uploads/2021/05/42-Final-sigle-seul.svg" alt="42" width="30" height="30">

A Unix shell implementation written in C, recreating bash behavior with modern features.

*42 Beirut Project*

---

## Features

- Interactive command line with history
- Command execution with PATH resolution
- Signal handling (Ctrl+C, Ctrl+D, Ctrl+\\)
- Environment variables with `$VAR` and `$?` expansion
- I/O redirection (`<`, `>`, `>>`)
- Heredoc (`<<`) with delimiter support
- Pipes (`|`) for command chaining
- Quote handling (single and double quotes)
- Wildcard expansion (`*`)
- Logical operators (`&&`, `||`) with parentheses
- Built-in commands: `cd`, `echo`, `env`, `exit`, `export`, `pwd`, `unset`

---

## Installation

```bash
git clone https://github.com/ApparentlyVenus/minishell.git
cd minishell
make
```

## Usage

```bash
./minishell

# Examples
minishell@~$ ls -la | grep ".c"
minishell@~$ echo "Hello $USER" > file.txt
minishell@~$ cat << EOF
Hello World
EOF
minishell@~$ mkdir test && cd test || echo "Failed"
```

---

## Built-in Commands

| Command | Description |
|---------|-------------|
| `cd` | Change directory (`cd`, `cd -`, `cd ~`, `cd path`) |
| `echo` | Display text (`echo [-n] text`) |
| `env` | Display environment variables |
| `exit` | Exit shell (`exit [code]`) |
| `export` | Set environment variables (`export VAR=value`) |
| `pwd` | Print working directory |
| `unset` | Remove environment variables |

## Architecture

```
Input → Tokenization → Parsing → Expansion → Execution → Cleanup
```

**Modules:**
- **Tokenizer**: Lexical analysis and quote handling
- **Parser**: AST construction with operator precedence
- **Expansion**: Variable, wildcard, and quote expansion
- **Execution**: Process management and I/O redirection
- **Built-ins**: Shell command implementations

## Authors

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/yousseff907">
        <img src="https://github.com/yousseff907.png" width="100px;" alt=""/>
        <br />
        <sub><b>Youssef Itani (yitani)</b></sub>
      </a>
      <br />
      <sub>Tokenization, Builtins, Signals</sub>
    </td>
    <td align="center">
      <a href="https://github.com/ApparentlyVenus">
        <img src="https://github.com/ApparentlyVenus.png" width="100px;" alt=""/>
        <br />
        <sub><b>Omar Dana (odana)</b></sub>
      </a>
      <br />
      <sub>Parsing, Execution, Expansion</sub>
    </td>
  </tr>
</table>
