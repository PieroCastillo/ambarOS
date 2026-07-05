set_project("ambarOS")
set_version("0.1.0")
set_xmakever("2.8.0")

local platform_dir = "platform/x86_64"
local kernel_targets = {}

rule("nasm.kernel")
    set_extensions(".asm")
    on_build_file(function (target, sourcefile, opt)
        import("core.project.depend")

        local obj = target:objectfile(sourcefile)

        depend.on_changed(function ()
            os.mkdir(path.directory(obj))
            os.vrunv("nasm", {
                "-f", "elf64",
                sourcefile,
                "-o", obj
            })
        end, {
            files = sourcefile,
            dependfile = obj .. ".d"
        })

        table.insert(target:objectfiles(), obj)
    end)

for _, srcdir in ipairs(os.dirs("src/*")) do
    local name = path.basename(srcdir)
    local linker = path.join(srcdir, "linker.ld")

    if os.isfile(linker) then
        table.insert(kernel_targets, name)

        target(name)
            set_kind("binary")
            set_toolchains("clang")
            set_toolset("ld", "ld")
            set_languages("c++26")

            add_rules("nasm.kernel")

            add_files(path.join(srcdir, "**.cpp"))
            add_files(path.join(srcdir, platform_dir, "**.asm"))

            add_includedirs(path.join(srcdir, "include"))

            add_cxxflags(
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
                {force = true}
            )

            add_ldflags(
                "--nmagic",
                "-m", "elf_x86_64",
                "-T", linker,
                {force = true}
            )

            set_targetdir("bin")
            set_filename(name .. ".bin")
    end
end

target("ambarOS")
    set_kind("phony")
    add_deps(table.unpack(kernel_targets))

    on_build(function ()
        import("core.project.depend")

        depend.on_changed(function ()
            os.mkdir("bin/boot/grub")
            os.mkdir("iso")

            os.cp("grub.cfg", "bin/boot/grub/grub.cfg")
            os.vrunv("sh", {"./check.sh"})
            os.vrunv("grub-mkrescue", {"-o", "iso/ambarOS.iso", "bin"})

            print("[+] Created iso/ambarOS.iso")
        end, {
            files = table.join(
                {"grub.cfg"},
                table.wrap(os.files("bin/*.bin"))
            ),
            dependfile = "build/ambarOS.iso.d"
        })
    end)

    on_run(function ()
        os.vrunv("qemu-system-x86_64", {
            "-cdrom", "iso/ambarOS.iso"
        })
    end)