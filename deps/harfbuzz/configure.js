/* configure for building harfbuzz
 */

exports.configureCompiler = function(objectFile, compiler) {
	// object files: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	compiler.setSourceFile('repo/src/' + a[2] + '.cc');
	compiler.addIncludeDir('../freetype/repo/include');
	compiler.addIncludeDir('repo/src');
	compiler.addIncludeDir('../ucdn/repo');
	compiler.addIncludeDir('generated');
	compiler.addMacro('HB_NO_MT');
	compiler.addMacro('HAVE_OT');
	compiler.addMacro('HAVE_FREETYPE');
	compiler.addMacro('HAVE_UCDN');
	// crazy hacks for xbox
	if(compiler.platform == 'xbox') {
		compiler.addMacro('getenv=(char*)""+__noop');
	}

	compiler.strict = false;
};

var objects = [
	// base
	'hb-aat-layout',
	'hb-aat-map',
	'hb-blob',
	'hb-buffer',
	'hb-buffer-serialize',
	'hb-common',
	'hb-coretext',
	'hb-draw',
	'hb-face',
	'hb-fallback-shape',
	'hb-font',
	'hb-gdi',
	'hb-map',
	'hb-number',
	'hb-set',
	'hb-shape',
	'hb-shape-plan',
	'hb-shaper',
	'hb-static',
	'hb-style',
	'hb-subset',
	'hb-subset-cff-common',
	'hb-subset-cff1',
	'hb-subset-cff2',
	'hb-subset-input',
	'hb-subset-plan',
	'hb-ucd',
	'hb-unicode',

	// opentype
	'hb-ot-cff1-table',
	'hb-ot-cff2-table',
	'hb-ot-color',
	'hb-ot-face',
	'hb-ot-font',
	'hb-ot-layout',
	'hb-ot-map',
	'hb-ot-meta',
	'hb-ot-metrics',
	'hb-ot-name',
	'hb-ot-shape',
	'hb-ot-shape-complex-arabic',
	'hb-ot-shape-complex-default',
	'hb-ot-shape-complex-hangul',
	'hb-ot-shape-complex-hebrew',
	'hb-ot-shape-complex-indic',
	'hb-ot-shape-complex-indic-table',
	'hb-ot-shape-complex-khmer',
	'hb-ot-shape-complex-myanmar',
	'hb-ot-shape-complex-thai',
	'hb-ot-shape-complex-use',
	'hb-ot-shape-complex-use-table',
	'hb-ot-shape-complex-vowel-constraints',
	'hb-ot-shape-fallback',
	'hb-ot-shape-normalize',
	'hb-ot-tag',
	'hb-ot-var',

	// freetype
	'hb-ft',
];

exports.configureComposer = function(libraryFile, composer) {
	// library files: <conf>/library
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	for(var i = 0; i < objects.length; ++i)
		composer.addObjectFile(confDir + objects[i]);
};
