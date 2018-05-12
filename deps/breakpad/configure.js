exports.configureCompiler = function(objectFile, compiler) {
	// объектные файлы: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	var b = /(.*)(\.(c|cc|cpp))$/.exec(a[2]);
	compiler.setSourceFile(b[1].replace(/\./g, '/') + b[2]);
	compiler.addIncludeDir('repo/src');
	compiler.addMacro('UNICODE');
};

var libraries = {
	'libbreakpad_client': {
		objects: [
			'breakpad.cpp',
			// 'repo.src.common.convert_UTF.c',
			// 'repo.src.common.md5.cc',
			// 'repo.src.common.string_conversion.cc',
			// 'repo.src.client.minidump_file_writer.cc',
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
			'repo.src.client.linux.crash_generation.crash_generation_client.cc',
			'repo.src.client.linux.crash_generation.crash_generation_server.cc',
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
		]
	}
};

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
