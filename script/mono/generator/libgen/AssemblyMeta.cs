using System;
using System.Collections.Generic;
using System.Reflection.Emit;
using System.Runtime.CompilerServices;
using System.Reflection;
using System.Linq;

namespace Inanity.Script.Mono
{
	public class AssemblyMeta
	{
		private class ConstructorDesc
		{
			public string[] argumentTypes;
		}

		private class MethodDesc
		{
			public string returnType;
			public string[] argumentTypes;
		}

		private class FunctionDesc
		{
			public string returnType;
			public string[] argumentTypes;
		}

		private class ClassDesc
		{
			public string parentClassName;
			public ConstructorDesc constructor;
			public Dictionary<string, MethodDesc> methods = new Dictionary<string, MethodDesc>();
			public Dictionary<string, FunctionDesc> staticMethods = new Dictionary<string, FunctionDesc>();
			public ClassDesc parentClassDesc;
			public TypeBuilder typeBuilder;
			public MethodBuilder constructorThunkMethodBuilder;
			public ConstructorBuilder constructorBuilder;
		}

		private Dictionary<string, ClassDesc> classes = new Dictionary<string, ClassDesc>();

		public AssemblyMeta()
		{
		}

		public void AddClass(string name)
		{
			if(classes.ContainsKey(name))
				throw new Exception("Class already registered: " + name);
			classes.Add(name, new ClassDesc());
		}

		public void SetClassParent(string className, string parentClassName)
		{
			classes [className].parentClassName = parentClassName;
		}

		public void SetClassConstructor(string className, string[] argumentTypes)
		{
			classes [className].constructor = new ConstructorDesc{ argumentTypes = argumentTypes };
		}

		public void AddClassMethod(string className, string name, string returnType, string[] argumentTypes)
		{
			classes [className].methods.Add(name, new MethodDesc{ returnType = returnType, argumentTypes = argumentTypes });
		}

		public void AddClassStaticMethod(string className, string name, string returnType, string[] argumentTypes)
		{
			classes [className].staticMethods.Add(name, new FunctionDesc {
				returnType = returnType,
				argumentTypes = argumentTypes
			});
		}

		public void Resolve(string assemblyName, string dllName)
		{
			// create assembly and module
			AssemblyBuilder assemblyBuilder = AppDomain.CurrentDomain.DefineDynamicAssembly(
				new AssemblyName(assemblyName),
				AssemblyBuilderAccess.Save
			);
			ModuleBuilder moduleBuilder = assemblyBuilder.DefineDynamicModule(assemblyName, dllName);

			// create Inanity.Object type
			TypeBuilder inanityObjectTypeBuilder = moduleBuilder.DefineType(
				"Inanity.Object",
				TypeAttributes.Public | TypeAttributes.Abstract,
				null, // parent
				new Type[] {
					typeof(IDisposable)
				}
			);
			inanityObjectTypeBuilder.DefineField(
				"ptr",
				typeof(UIntPtr),
				FieldAttributes.Family
			);
			ConstructorBuilder inanityObjectConstructorBuilder = inanityObjectTypeBuilder.DefineDefaultConstructor(MethodAttributes.Family);
			{
				MethodBuilder disposeMethod = inanityObjectTypeBuilder.DefineMethod(
					"Dispose",
					MethodAttributes.Public | MethodAttributes.Final | MethodAttributes.HideBySig,
					typeof(void),
					new Type[] { }
				);
				disposeMethod.SetImplementationFlags(MethodImplAttributes.InternalCall);
			}
			inanityObjectTypeBuilder.CreateType();

			// create type bulders for all classes
			foreach(string fullClassName in classes.Keys)
				classes[fullClassName].typeBuilder = moduleBuilder.DefineType(
					fullClassName,
					TypeAttributes.Public | (classes[fullClassName].constructor == null ? TypeAttributes.Abstract : 0)
				);

			// lambda for resolving types
			Func<string, Type> resolveType = delegate(string typeName)
			{
				switch(typeName)
				{
				case "void":
					return typeof(void);
				case "bool":
					return typeof(bool);
				case "int32":
					return typeof(Int32);
				case "uint32":
					return typeof(UInt32);
				case "int64":
					return typeof(Int64);
				case "uint64":
					return typeof(UInt64);
				case "float":
					return typeof(float);
				case "double":
					return typeof(double);
				case "string":
					return typeof(string);
				default:
					{
						ClassDesc classDesc;
						if(!classes.TryGetValue(typeName, out classDesc))
							throw new Exception("Cannot resolve type " + typeName);
						return classDesc.typeBuilder;
					}
				}
			};

			// set parents, create constructors and methods
			foreach(string className in classes.Keys)
			{
				ClassDesc classDesc = classes[className];
				TypeBuilder typeBuilder = classDesc.typeBuilder;
				// set parent
				{
					TypeBuilder parentTypeBuilder;
					if(classDesc.parentClassName != null)
					{
						if(!classes.TryGetValue(classDesc.parentClassName, out classDesc.parentClassDesc))
							throw new Exception("Parent class " + classDesc.parentClassName + " is not registered");
						parentTypeBuilder = classDesc.parentClassDesc.typeBuilder;
					}
					else
						parentTypeBuilder = inanityObjectTypeBuilder;
					typeBuilder.SetParent(parentTypeBuilder);
				}
				// set constructor
				{
					ConstructorDesc constructorDesc = classDesc.constructor;
					if(constructorDesc != null)
					{
						Type[] argumentTypes = constructorDesc.argumentTypes.Select(resolveType).ToArray();

						// define extern thunk method
						MethodBuilder methodBuilder = typeBuilder.DefineMethod(
							"__c",
							MethodAttributes.Private,
							CallingConventions.HasThis,
							typeof(void),
							argumentTypes
						);
						methodBuilder.SetImplementationFlags(MethodImplAttributes.InternalCall);
						classDesc.constructorThunkMethodBuilder = methodBuilder;

						// define constructor
						ConstructorBuilder constructorBuilder = typeBuilder.DefineConstructor(
							MethodAttributes.Public,
							CallingConventions.HasThis,
							argumentTypes
						);
						classDesc.constructorBuilder = constructorBuilder;
					}
					else
						classDesc.constructorBuilder = typeBuilder.DefineDefaultConstructor(MethodAttributes.Family);
				}
				// add methods
				foreach(string methodName in classDesc.methods.Keys)
				{
					MethodDesc methodDesc = classDesc.methods[methodName];
					MethodBuilder methodBuilder = typeBuilder.DefineMethod(
						methodName,
						MethodAttributes.Public | MethodAttributes.Final,
						CallingConventions.HasThis,
						resolveType(methodDesc.returnType),
						methodDesc.argumentTypes.Select(resolveType).ToArray()
					);
					methodBuilder.SetImplementationFlags(MethodImplAttributes.InternalCall);
				}
				// add static methods
				foreach(string staticMethodName in classDesc.staticMethods.Keys)
				{
					FunctionDesc functionDesc = classDesc.staticMethods[staticMethodName];
					MethodBuilder methodBuilder = typeBuilder.DefineMethod(
						staticMethodName,
						MethodAttributes.Public | MethodAttributes.Final | MethodAttributes.Static,
						CallingConventions.Standard,
						resolveType(functionDesc.returnType),
						functionDesc.argumentTypes.Select(resolveType).ToArray()
					);
					methodBuilder.SetImplementationFlags(MethodImplAttributes.InternalCall);
				}
			}

			// emit code for constructors
			foreach(ClassDesc classDesc in classes.Values)
			{
				ConstructorDesc constructorDesc = classDesc.constructor;
				if(constructorDesc == null)
					continue;

				Type[] argumentTypes = constructorDesc.argumentTypes.Select(resolveType).ToArray();

				ILGenerator ilGenerator = classDesc.constructorBuilder.GetILGenerator();
				ilGenerator.Emit(OpCodes.Ldarg_0);
				ilGenerator.Emit(OpCodes.Call,
					classDesc.parentClassDesc != null
					? classDesc.parentClassDesc.constructorBuilder
					: inanityObjectConstructorBuilder
				);
				ilGenerator.Emit(OpCodes.Ldarg_0);
				for(int i = 0; i < argumentTypes.Length; ++i)
					ilGenerator.Emit(OpCodes.Ldarg, i + 1);
				ilGenerator.Emit(OpCodes.Call, classDesc.constructorThunkMethodBuilder);
				ilGenerator.Emit(OpCodes.Ret);
			}

			// create types
			foreach(ClassDesc classDesc in classes.Values)
				classDesc.typeBuilder.CreateType();

			// save assembly
			assemblyBuilder.Save(dllName);
		}
	}
}

