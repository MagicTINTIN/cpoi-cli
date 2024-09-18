# CPOI-CLI (CPOI command line client)
> Copy Paste Over Internet
---
# What is it ?
It's a command line interface tool to simply easily copy and paste over machines.
It is part of the [cpoi](https://github.com/MagicTINTIN/cpoi) tool.

## How to use it ?
cpoi-cli requires 2 parametters : type and value
```
./cpoi-cli <c|uc|p|d> <value>
```

# Available types

### c - Copy
`./cpoi-cli c "Value to copy"` will save your "Value to copy" in a clipboard for 30 minutes maximum. It will return you a clipboard code formated like `word-word-word` that you'll need to paste the clipboard onto your other machine.

### uc - Unique Copy
`./cpoi-cli uc "Value to copy"`. Same as **c** (Copy), but clipboard will automaticaly be destroyed once it has been pasted.

### p - Paste
`./cpoi-cli p clip-board-code` will display the content of the "clip-board-code"'s clipboard.\
__Note:__ if the clipboard has been created with **uc**, it will be instantly destroyed, and you won't be able to paste it again.

### d - Destroy clipboard
`./cpoi-cli d clip-board-code`. Will destroy the clipboard manually. It won't be possible to paste it anymore.