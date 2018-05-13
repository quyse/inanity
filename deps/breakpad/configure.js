exports.configureCompiler = function(objectFile, compiler) {
	// объектные файлы: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	var b = /(.*)(\.(c|cc|cpp))$/.exec(a[2]);
	compiler.setSourceFile(b[1].replace(/\./g, '/') + b[2]);
	compiler.addIncludeDir('repo/src');
	compiler.addIncludeDir('include');
	compiler.addMacro('UNICODE');
	compiler.addMacro('NO_STABS_SUPPORT');
	if(b[3] == 'c')
		compiler.cppMode = false;
};

var libraries = {
	'libbreakpad_client': {
		objects: [
			'breakpad.cpp',
			// 'repo.src.common.md5.cc',
		],
		'objects-win32': [
			'repo.src.client.windows.crash_generation.client_info.cc',
			'repo.src.client.windows.crash_generation.crash_generation_client.cc',
			'repo.src.client.windows.crash_generation.crash_generation_server.cc',
			'repo.src.client.windows.crash_generation.minidump_generator.cc',
			'repo.src.client.windows.handler.exception_handler.cc',
			'repo.src.client.windows.sender.crash_report_sender.cc',
			// 'repo.src.common.windows.dia_util.cc',
			'repo.src.common.windows.guid_string.cc',
			'repo.src.common.windows.http_upload.cc',
			// 'repo.src.common.windows.omap.cc',
			// 'repo.src.common.windows.pdb_source_line_writer.cc',
			// 'repo.src.common.windows.string_utils.cc',
		],
		'objects-linux': [
			'repo.src.common.convert_UTF.c',
			'repo.src.client.linux.crash_generation.crash_generation_client.cc',
			// 'repo.src.client.linux.crash_generation.crash_generation_server.cc',
			'repo.src.client.linux.dump_writer_common.thread_info.cc',
			'repo.src.client.linux.dump_writer_common.ucontext_reader.cc',
			'repo.src.client.linux.handler.exception_handler.cc',
			'repo.src.client.linux.handler.minidump_descriptor.cc',
			'repo.src.client.linux.log.log.cc',
			'repo.src.client.linux.microdump_writer.microdump_writer.cc',
			'repo.src.client.linux.minidump_writer.linux_core_dumper.cc',
			'repo.src.client.linux.minidump_writer.linux_dumper.cc',
			'repo.src.client.linux.minidump_writer.linux_ptrace_dumper.cc',
			'repo.src.client.linux.minidump_writer.minidump_writer.cc',
			'repo.src.common.linux.elf_core_dump.cc',
			'repo.src.common.linux.elfutils.cc',
			'repo.src.common.linux.file_id.cc',
			'repo.src.common.linux.guid_creator.cc',
			'repo.src.common.linux.linux_libc_support.cc',
			'repo.src.common.linux.memory_mapped_file.cc',
			'repo.src.common.linux.safe_readlink.cc',
			'repo.src.common.string_conversion.cc',
			'repo.src.client.minidump_file_writer.cc',
		]
	}
};

var executables = {
	dump_syms: {
		'objects-linux': [
			'repo.src.common.dwarf_cfi_to_module.cc',
			'repo.src.common.dwarf_cu_to_module.cc',
			'repo.src.common.dwarf_line_to_module.cc',
			'repo.src.common.language.cc',
			'repo.src.common.module.cc',
			'repo.src.common.path_helper.cc',
			// 'repo.src.common.stabs_reader.cc',
			// 'repo.src.common.stabs_to_module.cc',
			'repo.src.common.dwarf.bytereader.cc',
			'repo.src.common.dwarf.dwarf2diehandler.cc',
			'repo.src.common.dwarf.dwarf2reader.cc',
			'repo.src.common.dwarf.elf_reader.cc',
			'repo.src.common.linux.crc32.cc',
			'repo.src.common.linux.dump_symbols.cc',
			'repo.src.common.linux.elf_symbols_to_module.cc',
			'repo.src.common.linux.elfutils.cc',
			'repo.src.common.linux.file_id.cc',
			'repo.src.common.linux.linux_libc_support.cc',
			'repo.src.common.linux.memory_mapped_file.cc',
			'repo.src.common.linux.safe_readlink.cc',
			'repo.src.tools.linux.dump_syms.dump_syms.cc',
			]
	}
}

var platformed = function(object, field, platform) {
	return (object[field] || []).concat(object[field + '-' + platform] || []);
};

exports.configureComposer = function(libraryFile, composer) {
	// файлы библиотек: <conf>/library
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	var objects = platformed(libraries[a[3]], 'objects', composer.platform);
	for ( var i = 0; i < objects.length; ++i)
		composer.addObjectFile(confDir + objects[i]);
};

exports.configureLinker = function(executableFile, linker) {
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(executableFile);
	linker.configuration = a[2];

	var executable = executables[a[3]];
	var objects = platformed(executable, 'objects', linker.platform);
	for(var i = 0; i < objects.length; ++i) {
		linker.addObjectFile(a[1] + objects[i]);
	}
	var dynamicLibraries = executable.dynamicLibraries || {};
	var dl = dynamicLibraries[linker.platform] || [];
	for(var i = 0; i < dl.length; ++i) {
		var lib = undefined;
		if(typeof dl[i] == 'string')
			lib = dl[i];
		else if((!dl[i].arch || dl[i].arch == linker.arch) && (!dl[i].configuration || dl[i].configuration == linker.configuration))
			lib = dl[i].lib;
		if(lib)
			linker.addDynamicLibrary(lib);
	}
}
