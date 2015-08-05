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
	compiler.addMacro('HAVE_OT');
	compiler.addMacro('HAVE_UCDN');
	compiler.strict = false;
};

var objects = [
	// base
	'hb-blob',
	'hb-buffer-serialize',
	'hb-buffer',
	'hb-common',
	// 'hb-coretext',
	'hb-face',
	'hb-fallback-shape',
	'hb-font',
	'hb-ot-tag',
	'hb-set',
	'hb-shape',
	'hb-shape-plan',
	'hb-shaper',
	//'hb-tt-font',
	'hb-unicode',
	'hb-ucdn',
	// 'hb-warning',

	// opentype
	'hb-ot-layout',
	'hb-ot-map',
	'hb-ot-shape',
	'hb-ot-shape-complex-arabic',
	'hb-ot-shape-complex-default',
	'hb-ot-shape-complex-hangul',
	'hb-ot-shape-complex-hebrew',
	'hb-ot-shape-complex-indic-table',
	'hb-ot-shape-complex-indic',
	'hb-ot-shape-complex-myanmar',
	'hb-ot-shape-complex-sea',
	'hb-ot-shape-complex-thai',
	'hb-ot-shape-complex-tibetan',
	'hb-ot-shape-fallback',
	'hb-ot-shape-normalize',

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
