/* configure for building assimp */

exports.configureCompiler = function(objectFile, compiler) {
	// object files: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	var b = /^(contrib\.)?(.*)(\.(c|cc))$/.exec(a[2]);
	if(!b)
		b = /^(contrib\.)?(.*)$/.exec(a[2]);

	var source = (b[1] ? 'repo/contrib/' : 'repo/code/') + b[2].replace(/\./g, '/') + (b[3] || '.cpp');
	compiler.setSourceFile(source);
	compiler.cppMode = (b[3] != '.c');
	compiler.addMacro('ASSIMP_BUILD_NO_OWN_ZLIB');
	compiler.addIncludeDir('../zlib');
};

var lib = {
	objects: [
		// PCH
		'AssimpPCH',
		// core
		'Assimp',
		// boost - skipped
		// logging
		'DefaultLogger',
		// common
		'BaseImporter',
		'BaseProcess',
		'PostStepRegistry',
		'ImporterRegistry',
		'DefaultIOStream',
		'DefaultIOSystem',
		'Importer',
		'SGSpatialSort',
		'VertexTriangleAdjacency',
		'SpatialSort',
		'SceneCombiner',
		'ScenePreprocessor',
		'SkeletonMeshBuilder',
		'SplitByBoneCountProcess',
		'StandardShapes',
		'TargetAnimation',
		'RemoveComments',
		'Subdivision',
		// 3DS
		'3DSConverter',
		'3DSLoader',
		// AC
		'ACLoader',
		'ASELoader',
		'ASEParser',
		// B3D
		'B3DImporter',
		// BVH
		'BVHLoader',
		// Collada
		'ColladaLoader',
		'ColladaParser',
		'ColladaExporter',
		// DXF
		'DXFLoader',
		// CSM
		'CSMLoader',
		// HMP
		'HMPLoader',
		// IRR
		'IRRLoader',
		'IRRMeshLoader',
		'IRRShared',
		// LWO
		'LWOAnimation',
		'LWOBLoader',
		'LWOLoader',
		'LWOMaterial',
		// LWS
		'LWSLoader',
		// MD2
		'MD2Loader',
		// MD3
		'MD3Loader',
		// MD5
		'MD5Loader',
		'MD5Parser',
		// MDC
		'MDCLoader',
		// MDL
		'MDLLoader',
		'MDLMaterialLoader',
		// MaterialSystem
		'MaterialSystem',
		// NFF
		'NFFLoader',
		// NDO
		'NDOLoader',
		// OFFFormat
		'OFFLoader',
		// Obj
		'ObjFileImporter',
		'ObjFileMtlImporter',
		'ObjFileParser',
		'ObjExporter',
		// Ogre
		'OgreImporter',
		'OgreMaterial',
		'OgreMesh',
		'OgreSkeleton',
		// Ply
		'PlyLoader',
		'PlyParser',
		'PlyExporter',
		//MS3D
		'MS3DLoader',
		//COB
		'COBLoader',
		//BLENDER
		'BlenderLoader',
		'BlenderDNA',
		'BlenderScene',
		'BlenderModifier',
		'BlenderBMesh',
		'BlenderTessellator',
		//IFC
		'IFCLoader',
		'IFCReaderGen',
		'IFCUtil',
		'IFCGeometry',
		'IFCMaterial',
		'IFCProfile',
		'IFCCurve',
		'IFCBoolean',
		'IFCOpenings',
		'STEPFileReader',
		'STEPFileEncoding',
		// XGL
		'XGLLoader',
		//FBX
		'FBXImporter',
		'FBXParser',
		'FBXTokenizer',
		'FBXConverter',
		'FBXUtil',
		'FBXDocument',
		'FBXProperties',
		'FBXMeshGeometry',
		'FBXMaterial',
		'FBXModel',
		'FBXAnimation',
		'FBXNodeAttribute',
		'FBXDeformer',
		'FBXBinaryTokenizer',
		'FBXDocumentUtil',
		// PostProcessing
		'CalcTangentsProcess',
		'ComputeUVMappingProcess',
		'ConvertToLHProcess',
		'FindDegenerates',
		'FindInstancesProcess',
		'FindInvalidDataProcess',
		'FixNormalsStep',
		'GenFaceNormalsProcess',
		'GenVertexNormalsProcess',
		'PretransformVertices',
		'ImproveCacheLocality',
		'JoinVerticesProcess',
		'LimitBoneWeightsProcess',
		'RemoveRedundantMaterials',
		'RemoveVCProcess',
		'SortByPTypeProcess',
		'SplitLargeMeshes',
		'TerragenLoader',
		'TextureTransform',
		'TriangulateProcess',
		'ValidateDataStructure',
		'OptimizeGraph',
		'OptimizeMeshes',
		'DeboneProcess',
		'ProcessHelper',
		'MakeVerboseFormat',
		// Q3D
		'Q3DLoader',
		// Q3BSP
		'Q3BSPFileParser',
		'Q3BSPFileImporter',
		'Q3BSPZipArchive',
		// Raw
		'RawLoader',
		// SMD
		'SMDLoader',
		// STL
		'STLLoader',
		'STLExporter',
		// Unreal
		'UnrealLoader',
		// XFile
		'XFileImporter',
		'XFileParser',
		// Exporter
		'Exporter',
		'AssimpCExport',
		// Extra
		// IrrXML
		'contrib.irrXML.irrXML',
		// ConvertUTF
		'contrib.ConvertUTF.ConvertUTF.c',
		// Clipper
		'contrib.clipper.clipper',
		// Poly2Tri
		'contrib.poly2tri.poly2tri.common.shapes.cc',
		'contrib.poly2tri.poly2tri.sweep.advancing_front.cc',
		'contrib.poly2tri.poly2tri.sweep.cdt.cc',
		'contrib.poly2tri.poly2tri.sweep.sweep.cc',
		'contrib.poly2tri.poly2tri.sweep.sweep_context.cc',
		// unzip
		'contrib.unzip.ioapi.c',
		'contrib.unzip.unzip.c',
	]
};

exports.configureComposer = function(libraryFile, composer) {
	// library files: <conf>/library
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	var objects = lib.objects;
	for(var i = 0; i < objects.length; ++i)
		composer.addObjectFile(confDir + objects[i]);
};
