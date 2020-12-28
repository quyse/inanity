/* configure for building icu
 */

exports.configureCompiler = function(objectFile, compiler) {
	// object files: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	var source = a[2];
	var b = /^(.*)\.c$/.exec(source);
	if(b) {
		// .c-файл
		source = b[1].replace(/\./g, '/') + '.c';
		compiler.cppMode = false;
	}
	else
		// .cpp-файл
		source = source.replace(/\./g, '/') + '.cpp';
	compiler.setSourceFile('repo/source/' + source);
	compiler.addIncludeDir('repo/source/common');
	compiler.addIncludeDir('repo/source/i18n');
	compiler.addMacro('U_STATIC_IMPLEMENTATION');
	compiler.addMacro('U_ENABLE_DYLOAD=0');
	compiler.addMacro('UCONFIG_NO_FILE_IO=1');
	// crazy hacks for windows
	if(compiler.platform == 'win32') {
		compiler.addMacro('__WINTZ'); // to skip including wintz.h
		compiler.addMacro('uprv_detectWindowsTimeZone=(const char*)__noop');
	}
	// crazy hacks for xbox
	if(compiler.platform == 'xbox') {
		compiler.addMacro('U_TZSET=__noop');
		compiler.addMacro('getenv=""+__noop');
		compiler.addMacro('GetACP=65001+__noop');
	}
	// crazy hack for switch
	if(compiler.platform == 'switch') {
		compiler.addMacro('register=');
	}

	var c = /^([^\/]*)\//.exec(source);
	switch(c[1]) {
	case 'common':
		compiler.addMacro('U_COMMON_IMPLEMENTATION');
		break;
	case 'i18n':
		compiler.addMacro('U_I18N_IMPLEMENTATION');
		break;
	}
	compiler.strict = false;
};

var objects = [
	'common.appendable',
	'common.bmpset',
	'common.brkeng',
	'common.brkiter',
	'common.bytesinkutil',
	'common.bytestream',
	'common.bytestrie',
	'common.bytestriebuilder',
	'common.bytestrieiterator',
	'common.caniter',
	'common.characterproperties',
	'common.chariter',
	'common.charstr',
	'common.cmemory',
	'common.cstr',
	'common.cstring',
	'common.cwchar',
	'common.dictbe',
	'common.dictionarydata',
	'common.dtintrv',
	'common.edits',
	'common.errorcode',
	'common.filteredbrk',
	'common.filterednormalizer2',
	'common.icudataver',
	'common.icuplug',
	'common.loadednormalizer2impl',
	'common.localebuilder',
	'common.localematcher',
	'common.localeprioritylist',
	'common.locavailable',
	'common.locbased',
	'common.locdispnames',
	'common.locdistance',
	'common.locdspnm',
	'common.locid',
	'common.loclikely',
	'common.loclikelysubtags',
	'common.locmap',
	'common.locresdata',
	'common.locutil',
	'common.lsr',
	'common.messagepattern',
	'common.normalizer2',
	'common.normalizer2impl',
	'common.normlzr',
	'common.parsepos',
	'common.patternprops',
	'common.pluralmap',
	'common.propname',
	'common.propsvec',
	'common.punycode',
	'common.putil',
	'common.rbbi',
	'common.rbbi_cache',
	'common.rbbidata',
	'common.rbbinode',
	'common.rbbirb',
	'common.rbbiscan',
	'common.rbbisetb',
	'common.rbbistbl',
	'common.rbbitblb',
	'common.resbund',
	'common.resbund_cnv',
	'common.resource',
	'common.restrace',
	'common.ruleiter',
	'common.schriter',
	'common.serv',
	'common.servlk',
	'common.servlkf',
	'common.servls',
	'common.servnotf',
	'common.servrbf',
	'common.servslkf',
	'common.sharedobject',
	'common.simpleformatter',
	'common.static_unicode_sets',
	'common.stringpiece',
	'common.stringtriebuilder',
	'common.uarrsort',
	'common.ubidi',
	'common.ubidi_props',
	'common.ubidiln',
	'common.ubiditransform',
	'common.ubidiwrt',
	'common.ubrk',
	'common.ucase',
	'common.ucasemap',
	'common.ucasemap_titlecase_brkiter',
	'common.ucat',
	'common.uchar',
	'common.ucharstrie',
	'common.ucharstriebuilder',
	'common.ucharstrieiterator',
	'common.uchriter',
	'common.ucln_cmn',
	'common.ucmndata',
	'common.ucnv',
	'common.ucnv2022',
	'common.ucnv_bld',
	'common.ucnv_cb',
	'common.ucnv_cnv',
	'common.ucnv_ct',
	'common.ucnv_err',
	'common.ucnv_ext',
	'common.ucnv_io',
	'common.ucnv_lmb',
	'common.ucnv_set',
	'common.ucnv_u16',
	'common.ucnv_u32',
	'common.ucnv_u7',
	'common.ucnv_u8',
	'common.ucnvbocu',
	'common.ucnvdisp',
	'common.ucnvhz',
	'common.ucnvisci',
	'common.ucnvlat1',
	'common.ucnvmbcs',
	'common.ucnvscsu',
	'common.ucnvsel',
	'common.ucol_swp',
	'common.ucptrie',
	'common.ucurr',
	'common.udata',
	'common.udatamem',
	'common.udataswp',
	'common.uenum',
	'common.uhash',
	'common.uhash_us',
	'common.uidna',
	'common.uinit',
	'common.uinvchar',
	'common.uiter',
	'common.ulist',
	'common.uloc',
	'common.uloc_keytype',
	'common.uloc_tag',
	'common.umapfile',
	'common.umath',
	'common.umutablecptrie',
	'common.umutex',
	'common.unames',
	'common.unifiedcache',
	'common.unifilt',
	'common.unifunct',
	'common.uniset',
	'common.uniset_closure',
	'common.uniset_props',
	'common.unisetspan',
	'common.unistr',
	'common.unistr_case',
	'common.unistr_case_locale',
	'common.unistr_cnv',
	'common.unistr_props',
	'common.unistr_titlecase_brkiter',
	'common.unorm',
	'common.unormcmp',
	'common.uobject',
	'common.uprops',
	'common.ures_cnv',
	'common.uresbund',
	'common.uresdata',
	'common.usc_impl',
	'common.uscript',
	'common.uscript_props',
	'common.uset',
	'common.uset_props',
	'common.usetiter',
	'common.ushape',
	'common.usprep',
	'common.ustack',
	'common.ustr_cnv',
	'common.ustr_titlecase_brkiter',
	'common.ustr_wcs',
	'common.ustrcase',
	'common.ustrcase_locale',
	'common.ustrenum',
	'common.ustrfmt',
	'common.ustring',
	'common.ustrtrns',
	'common.utext',
	'common.utf_impl',
	'common.util',
	'common.util_props',
	'common.utrace',
	'common.utrie',
	'common.utrie2',
	'common.utrie2_builder',
	'common.utrie_swap',
	'common.uts46',
	'common.utypes',
	'common.uvector',
	'common.uvectr32',
	'common.uvectr64',
	// 'common.wintz',

	'stubdata.stubdata',
];

exports.configureComposer = function(libraryFile, composer) {
	// library files: <conf>/library
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	for(var i = 0; i < objects.length; ++i)
		composer.addObjectFile(confDir + objects[i]);
};
