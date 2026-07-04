set_project("ambarOS")
set_version("0.1.0")
set_xmakever("2.8.0")

local platform_dir = "platform/x86_64"
local kernel_targets = {}

local cxxflags = {
    "-ffreestanding",
    "-nostdlib",
    "-nostartfiles",
    "-nodefaultlibs",
    "-masm=intel",
    "-march=x86-64-v3",
    "-mno-red-zone",
    "-fno-exceptions",
    "-fno-rtti",
    "-fno-builtin",
    "-std=c++2c"
}

for _, srcdir in ipairs(os.dirs("src/*")) do
    local name = path.basename(srcdir)
    local linker = path.join(srcdir, "linker.ld")

    if os.isfile(linker) then
        table.insert(kernel_targets, name)

        target(name)
            set_kind("phony")

            on_build(function ()
                local builddir = path.join("build", name)
                local binfile = path.join("bin", name .. ".bin")
                local includedir = path.join(srcdir, "include")
                local objfiles = {}

                os.mkdir(builddir)
                os.mkdir("bin")

                for _, file in ipairs(os.files(path.join(srcdir, "**.cpp"))) do
                    local obj = path.join(builddir, path.basename(file) .. ".o")
                    os.vrunv("clang++", table.join(cxxflags, {
                        "-c",
                        "-I" .. includedir,
                        file,
                        "-o", obj
                    }))
                    table.insert(objfiles, obj)
                end

                for _, file in ipairs(os.files(path.join(srcdir, platform_dir, "**.asm"))) do
                    local obj = path.join(builddir, path.basename(file) .. ".o")
                    os.vrunv("nasm", {
                        "-f", "elf64",
                        file,
                        "-o", obj
                    })
                    table.insert(objfiles, obj)
                end

                os.vrunv("ld", table.join({
                    "--nmagic",
                    "-m", "elf_x86_64",
                    "-T", linker
                }, objfiles, {
                    "-o", binfile
                }))

                print("[+] Linked: %s", binfile)
            end)
    end
end

target("ambarOS")
    set_kind("phony")
    add_deps(table.unpack(kernel_targets))

    on_build(function ()
        os.mkdir("bin/boot/grub")
        os.mkdir("iso")

        os.cp("grub.cfg", "bin/boot/grub/grub.cfg")
        os.vrunv("sh", {"./check.sh"})
        os.vrunv("grub-mkrescue", {"-o", "iso/ambarOS.iso", "bin"})

        print("[+] Created iso/ambarOS.iso")
    end)

    on_run(function ()
        os.vrunv("qemu-system-x86_64", {
            "-cdrom", "iso/ambarOS.iso"
        })
    end)