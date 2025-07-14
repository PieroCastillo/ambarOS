if grub-file --is-x86-multiboot2 bin/volta.bin; then
  echo [+] Multiboot confirmed
else
  echo [-] The file is not a valid multiboot file
fi