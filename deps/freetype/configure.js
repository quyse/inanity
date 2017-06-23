/* configure for building freetype
 */

exports.configureCompiler = function(objectFile, compiler) {
	// object files: <conf>/module.object
	var a = /^([^\/]+)\/([^\/\.]+)\.([^\/\.]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	compiler.setSourceFile('repo/src/' + a[2] + '/' + a[3] + '.c');
	compiler.cppMode = false;
	compiler.addIncludeDir('repo/include');
	compiler.addIncludeDir('../zlib');
	compiler.addMacro('FT2_BUILD_LIBRARY');
	compiler.addMacro('FT_CONFIG_OPTION_SYSTEM_ZLIB');
	compiler.strict = false;
};

var modules = {
	autofit: "autofit",
	base: "ftbase ftinit ftmm ftsystem ftbitmap ftglyph",
	bdf: "bdf",
	bzip2: "ftbzip2",
	cache: "ftcache",
	cff: "cff",
	cid: "type1cid",
	gxvalid: "gxvalid",
	gzip: "ftgzip",
	lzw: "ftlzw",
	otvalid: "otvalid",
	pcf: "pcf",
	pfr: "pfr",
	psaux: "psaux",
	pshinter: "pshinter",
	psnames: "psnames",
	raster: "raster",
	sfnt: "sfnt",
	smooth: "smooth",
	truetype: "truetype",
	type1: "type1",
	type42: "type42",
	winfonts: "winfnt"
};

exports.configureComposer = function(libraryFile, composer) {
	// library files: <conf>/library
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	for (var module in modules) {
		var objects = modules[module].split(' ');
		for(var i = 0; i < objects.length; ++i)
			composer.addObjectFile(confDir + module + '.' + objects[i]);
	}
};
