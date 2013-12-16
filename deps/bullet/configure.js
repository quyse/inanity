/* конфигурационный файл bullet 2.81
 * для компиляции в составе с inanity2
 * написан вручную, по инструкциям из src/makefile.am
 */

exports.configureCompiler = function(objectFile, compiler) {
	// объектные файлы: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	// заменяем точки на слеши, и добавляем расширение
	// если оканчивается на .c - это .c-файл, иначе .cpp
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
	compiler.setSourceFile('repo/src/' + source);
	compiler.addIncludeDir('repo/src');
	compiler.strict = false;
};

// объектные файлы по библиотекам
var libraryObjects = {
	'libbullet-linearmath': [
		'LinearMath.btQuickprof',
		'LinearMath.btGeometryUtil',
		'LinearMath.btAlignedAllocator',
		'LinearMath.btSerializer',
		'LinearMath.btConvexHull',
		'LinearMath.btPolarDecomposition',
		'LinearMath.btVector3',
		'LinearMath.btConvexHullComputer'
		],
	'libbullet-collision': [
		'BulletCollision.NarrowPhaseCollision.btRaycastCallback',
		'BulletCollision.NarrowPhaseCollision.btMinkowskiPenetrationDepthSolver',
		'BulletCollision.NarrowPhaseCollision.btSubSimplexConvexCast',
		'BulletCollision.NarrowPhaseCollision.btGjkEpaPenetrationDepthSolver',
		'BulletCollision.NarrowPhaseCollision.btGjkConvexCast',
		'BulletCollision.NarrowPhaseCollision.btPersistentManifold',
		'BulletCollision.NarrowPhaseCollision.btConvexCast',
		'BulletCollision.NarrowPhaseCollision.btPolyhedralContactClipping',
		'BulletCollision.NarrowPhaseCollision.btContinuousConvexCollision',
		'BulletCollision.NarrowPhaseCollision.btGjkPairDetector',
		'BulletCollision.NarrowPhaseCollision.btGjkEpa2',
		'BulletCollision.NarrowPhaseCollision.btVoronoiSimplexSolver',
		'BulletCollision.CollisionDispatch.btActivatingCollisionAlgorithm',
		'BulletCollision.CollisionDispatch.btCollisionObject',
		'BulletCollision.CollisionDispatch.btEmptyCollisionAlgorithm',
		'BulletCollision.CollisionDispatch.btGhostObject',
		'BulletCollision.CollisionDispatch.btSphereSphereCollisionAlgorithm',
		'BulletCollision.CollisionDispatch.btSphereBoxCollisionAlgorithm',
		'BulletCollision.CollisionDispatch.btCollisionDispatcher',
		'BulletCollision.CollisionDispatch.btDefaultCollisionConfiguration',
		'BulletCollision.CollisionDispatch.btSimulationIslandManager',
		'BulletCollision.CollisionDispatch.btBoxBoxDetector',
		'BulletCollision.CollisionDispatch.btConvexPlaneCollisionAlgorithm',
		'BulletCollision.CollisionDispatch.btConvexConcaveCollisionAlgorithm',
		'BulletCollision.CollisionDispatch.btBoxBoxCollisionAlgorithm',
		'BulletCollision.CollisionDispatch.btBox2dBox2dCollisionAlgorithm',
		'BulletCollision.CollisionDispatch.SphereTriangleDetector',
		'BulletCollision.CollisionDispatch.btInternalEdgeUtility',
		'BulletCollision.CollisionDispatch.btManifoldResult',
		'BulletCollision.CollisionDispatch.btCollisionWorld',
		'BulletCollision.CollisionDispatch.btSphereTriangleCollisionAlgorithm',
		'BulletCollision.CollisionDispatch.btConvexConvexAlgorithm',
		'BulletCollision.CollisionDispatch.btConvex2dConvex2dAlgorithm',
		'BulletCollision.CollisionDispatch.btUnionFind',
		'BulletCollision.CollisionDispatch.btCompoundCollisionAlgorithm',
		'BulletCollision.CollisionDispatch.btHashedSimplePairCache',
		'BulletCollision.CollisionDispatch.btCompoundCompoundCollisionAlgorithm',
		'BulletCollision.CollisionShapes.btTetrahedronShape',
		'BulletCollision.CollisionShapes.btShapeHull',
		'BulletCollision.CollisionShapes.btMinkowskiSumShape',
		'BulletCollision.CollisionShapes.btCompoundShape',
		'BulletCollision.CollisionShapes.btConeShape',
		'BulletCollision.CollisionShapes.btConvexPolyhedron',
		'BulletCollision.CollisionShapes.btMultiSphereShape',
		'BulletCollision.CollisionShapes.btUniformScalingShape',
		'BulletCollision.CollisionShapes.btSphereShape',
		'BulletCollision.CollisionShapes.btTriangleIndexVertexArray',
		'BulletCollision.CollisionShapes.btBvhTriangleMeshShape',
		'BulletCollision.CollisionShapes.btTriangleMeshShape',
		'BulletCollision.CollisionShapes.btTriangleBuffer',
		'BulletCollision.CollisionShapes.btStaticPlaneShape',
		'BulletCollision.CollisionShapes.btPolyhedralConvexShape',
		'BulletCollision.CollisionShapes.btEmptyShape',
		'BulletCollision.CollisionShapes.btCollisionShape',
		'BulletCollision.CollisionShapes.btConvexShape',
		'BulletCollision.CollisionShapes.btConvex2dShape',
		'BulletCollision.CollisionShapes.btConvexInternalShape',
		'BulletCollision.CollisionShapes.btConvexHullShape',
		'BulletCollision.CollisionShapes.btTriangleCallback',
		'BulletCollision.CollisionShapes.btCapsuleShape',
		'BulletCollision.CollisionShapes.btConvexTriangleMeshShape',
		'BulletCollision.CollisionShapes.btConcaveShape',
		'BulletCollision.CollisionShapes.btConvexPointCloudShape',
		'BulletCollision.CollisionShapes.btBoxShape',
		'BulletCollision.CollisionShapes.btBox2dShape',
		'BulletCollision.CollisionShapes.btOptimizedBvh',
		'BulletCollision.CollisionShapes.btHeightfieldTerrainShape',
		'BulletCollision.CollisionShapes.btMultimaterialTriangleMeshShape',
		'BulletCollision.CollisionShapes.btCylinderShape',
		'BulletCollision.CollisionShapes.btScaledBvhTriangleMeshShape',
		'BulletCollision.CollisionShapes.btStridingMeshInterface',
		'BulletCollision.CollisionShapes.btTriangleIndexVertexMaterialArray',
		'BulletCollision.CollisionShapes.btTriangleMesh',
		'BulletCollision.BroadphaseCollision.btAxisSweep3',
		'BulletCollision.BroadphaseCollision.btOverlappingPairCache',
		'BulletCollision.BroadphaseCollision.btDbvtBroadphase',
		'BulletCollision.BroadphaseCollision.btMultiSapBroadphase',
		'BulletCollision.BroadphaseCollision.btDispatcher',
		'BulletCollision.BroadphaseCollision.btQuantizedBvh',
		'BulletCollision.BroadphaseCollision.btCollisionAlgorithm',
		'BulletCollision.BroadphaseCollision.btDbvt',
		'BulletCollision.BroadphaseCollision.btSimpleBroadphase',
		'BulletCollision.Gimpact.btGImpactBvh',
		'BulletCollision.Gimpact.btGImpactQuantizedBvh',
		'BulletCollision.Gimpact.btTriangleShapeEx',
		'BulletCollision.Gimpact.btGImpactCollisionAlgorithm',
		'BulletCollision.Gimpact.btGImpactShape',
		'BulletCollision.Gimpact.gim_box_set',
		'BulletCollision.Gimpact.gim_contact',
		'BulletCollision.Gimpact.gim_memory',
		'BulletCollision.Gimpact.gim_tri_collision'
		],
	'libbullet-dynamics': [
		'BulletDynamics.Dynamics.btRigidBody',
		'BulletDynamics.Dynamics.btSimpleDynamicsWorld',
		'BulletDynamics.Dynamics.Bullet-C-API',
		'BulletDynamics.Dynamics.btDiscreteDynamicsWorld',
		'BulletDynamics.ConstraintSolver.btFixedConstraint',
		'BulletDynamics.ConstraintSolver.btGearConstraint',
		'BulletDynamics.ConstraintSolver.btGeneric6DofConstraint',
		'BulletDynamics.ConstraintSolver.btGeneric6DofSpringConstraint',
		'BulletDynamics.ConstraintSolver.btSolve2LinearConstraint',
		'BulletDynamics.ConstraintSolver.btPoint2PointConstraint',
		'BulletDynamics.ConstraintSolver.btTypedConstraint',
		'BulletDynamics.ConstraintSolver.btContactConstraint',
		'BulletDynamics.ConstraintSolver.btSliderConstraint',
		'BulletDynamics.ConstraintSolver.btConeTwistConstraint',
		'BulletDynamics.ConstraintSolver.btHingeConstraint',
		'BulletDynamics.ConstraintSolver.btHinge2Constraint',
		'BulletDynamics.ConstraintSolver.btUniversalConstraint',
		'BulletDynamics.ConstraintSolver.btSequentialImpulseConstraintSolver',
		'BulletDynamics.Vehicle.btWheelInfo',
		'BulletDynamics.Vehicle.btRaycastVehicle',
		'BulletDynamics.Character.btKinematicCharacterController',
		'BulletDynamics.Featherstone.btMultiBody',
		'BulletDynamics.Featherstone.btMultiBodyConstraintSolver',
		'BulletDynamics.Featherstone.btMultiBodyDynamicsWorld',
		'BulletDynamics.Featherstone.btMultiBodyJointMotor',
		'BulletDynamics.Featherstone.btMultiBodyJointLimitConstraint',
		'BulletDynamics.Featherstone.btMultiBodyConstraint',
		'BulletDynamics.Featherstone.btMultiBodyPoint2Point',
		'BulletDynamics.MLCPSolvers.btDantzigLCP',
		'BulletDynamics.MLCPSolvers.btMLCPSolver'
		],
	'libbullet-softbody': [
		'BulletSoftBody.btDefaultSoftBodySolver',
		'BulletSoftBody.btSoftBodyRigidBodyCollisionConfiguration',
		'BulletSoftBody.btSoftBody',
		'BulletSoftBody.btSoftRigidCollisionAlgorithm',
		'BulletSoftBody.btSoftBodyConcaveCollisionAlgorithm',
		'BulletSoftBody.btSoftRigidDynamicsWorld',
		'BulletSoftBody.btSoftBodyHelpers',
		'BulletSoftBody.btSoftSoftCollisionAlgorithm'
		]
};

exports.configureComposer = function(libraryFile, composer) {
	// файлы библиотек: <conf>/library
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	var objects = libraryObjects[a[3]];
	for ( var i = 0; i < objects.length; ++i)
		composer.addObjectFile(confDir + objects[i]);
};
