/* конфигурационный файл libuv
 * для компиляции в составе с inanity2
 * написан вручную, в основном переводом из uv.gyp
 */

var macros = ['HAVE_CONFIG_H', '_WIN32_WINNT=0x0502', 'EIO_STACKSIZE=262144', '_GNU_SOURCE'];

var includes = ['include', 'include/uv-private', 'src/ares/config_win32', 'src'];

exports.configureCompiler = function(objectFile, compiler) {
	// объектные файлы: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	compiler.setSourceFile(a[2].replace(/\./g, '/') + '.c');
	for ( var i = 0; i < macros.length; ++i)
		compiler.addMacro(macros[i]);
	for ( var i = 0; i < includes.length; ++i)
		compiler.addIncludeDir(includes[i]);
};

var objects = [
// general
'src/uv-common', 'src/ares/ares_cancel', 'src/ares/ares__close_sockets', 'src/ares/ares_data', 'src/ares/ares_destroy', 'src/ares/ares_expand_name', 'src/ares/ares_expand_string', 'src/ares/ares_fds', 'src/ares/ares_free_hostent', 'src/ares/ares_free_string', 'src/ares/ares_gethostbyaddr', 'src/ares/ares_gethostbyname', 'src/ares/ares__get_hostent', 'src/ares/ares_getnameinfo', 'src/ares/ares_getopt', 'src/ares/ares_getsock', 'src/ares/ares_init', 'src/ares/ares_library_init', 'src/ares/ares_llist', 'src/ares/ares_mkquery', 'src/ares/ares_nowarn', 'src/ares/ares_options', 'src/ares/ares_parse_aaaa_reply', 'src/ares/ares_parse_a_reply', 'src/ares/ares_parse_mx_reply', 'src/ares/ares_parse_ns_reply', 'src/ares/ares_parse_ptr_reply', 'src/ares/ares_parse_srv_reply', 'src/ares/ares_parse_txt_reply', 'src/ares/ares_process', 'src/ares/ares_query', 'src/ares/ares__read_line', 'src/ares/ares_search', 'src/ares/ares_send', 'src/ares/ares_strcasecmp', 'src/ares/ares_strdup',
	'src/ares/ares_strerror', 'src/ares/ares_timeout', 'src/ares/ares__timeval', 'src/ares/ares_version', 'src/ares/ares_writev', 'src/ares/bitncmp', 'src/ares/inet_net_pton', 'src/ares/inet_ntop',
	// windows
	'src/ares/windows_port', 'src/ares/ares_getenv', 'src/ares/ares_platform', 'src/win/async', 'src/win/cares', 'src/win/core', 'src/win/error', 'src/win/fs', 'src/win/fs-event', 'src/win/getaddrinfo', 'src/win/handle', 'src/win/loop-watcher', 'src/win/pipe', 'src/win/process', 'src/win/req', 'src/win/stream', 'src/win/tcp', 'src/win/tty', 'src/win/threadpool', 'src/win/threads', 'src/win/timer', 'src/win/udp', 'src/win/util', 'src/win/winapi', 'src/win/winsock', ];

exports.configureComposer = function(libraryFile, composer) {
	// файлы библиотек: <conf>/library
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	for ( var i = 0; i < objects.length; ++i)
		composer.addObjectFile(confDir + objects[i].replace(/\//g, '.'));
};
