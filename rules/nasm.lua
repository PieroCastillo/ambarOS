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