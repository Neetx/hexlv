# HexLV
HexLV is a simple x86-64 kernel that runs in long mode.
Soon I will integrate code in my another toy project: https://github.com/Neetx/osdevTraining that is for i386 architectures.
Thanks to https://intermezzos.github.io/book/first-edition/preface.html that give me x86-64 concepts and initial code.

---

#### Dependences
* make
* gcc
* grub-mkrescue
* nasm
* qemu

Tested on Debian Stretch.

---

#### Run
Clone - make - qemu

```
git clone https://gitlab.com/Neetx/hexlv.git
cd hexlv
make
qemu-system-x86_64 -cdrom build/hexlv.iso
```
---

### CONTACTS:
[Neetx](mailto:neetx@protonmail.com)

