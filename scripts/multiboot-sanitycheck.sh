#!/bin/bash

if grub-file --is-x86-multiboot os/os.elf;
then
      echo "multiboot confirmed";
else
      echo "the file is not mutliboot";
fi

