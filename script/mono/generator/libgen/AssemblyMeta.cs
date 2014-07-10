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
			public string parent;
			public ConstructorDesc constructor;
			public Dictionary<string, MethodDesc> methods = new Dictionary<string, MethodDesc>();
			public Dictionary<string, FunctionDesc> staticMethods = new Dictionary<string, FunctionDesc>();
		}

		private Dictionary<string, ClassDesc> classes = new Dictionary<string, ClassDesc>();

		public AssemblyMeta()
		{
		}

		public void AddClass(string name)
		{
			classes.Add(name, new ClassDesc());
		}

		public void SetClassParent(string className, string parentClassName)
		{
			classes [className].parent = parentClassName;
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

			// InternalCall attribute for methods
			CustomAttributeBuilder internalCallAttribute = new CustomAttributeBuilder(
				typeof(MethodImplAttribute).GetConstructor(
					new Type[]{ typeof(MethodImplOptions) }),
				new object[]{ MethodImplOptions.InternalCall });

			// create Inanity.Object type
			TypeBuilder inanityObjectTypeBuilder = moduleBuilder.DefineType(
				"Inanity.Object",
				TypeAttributes.Public | TypeAttributes.Abstract,
				null, // parent
				new Type[] {
					typeof(IDisposable)
				}
			);
			{
				MethodBuilder disposeMethod = inanityObjectTypeBuilder.DefineMethod(
					"Dispose",
					MethodAttributes.Public | MethodAttributes.Final | MethodAttributes.HideBySig,
					typeof(void),
					new Type[] { }
				);
				disposeMethod.SetCustomAttribute(internalCallAttribute);
			}
			inanityObjectTypeBuilder.CreateType();

			// create type bulders for all classes
			Dictionary<string, TypeBuilder> typeBuilders = new Dictionary<string, TypeBuilder>();
			foreach(string fullClassName in classes.Keys)
				typeBuilders.Add(fullClassName, moduleBuilder.DefineType(
					fullClassName,
					TypeAttributes.Public | (classes[fullClassName].constructor == null ? TypeAttributes.Abstract : 0)
				));

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
						TypeBuilder typeBuilder;
						if(!typeBuilders.TryGetValue(typeName, out typeBuilder))
							throw new Exception("Cannot resolve type " + typeName);
						return typeBuilder;
					}
				}
			};

			// set parents, populate classes
			foreach(string className in classes.Keys)
			{
				ClassDesc classDesc = classes[className];
				TypeBuilder typeBuilder = typeBuilders[className];
				// set parent
				{
					string parentClassName = classDesc.parent;
					TypeBuilder parentTypeBuilder;
					if(parentClassName != null)
					{
						if(!typeBuilders.TryGetValue(parentClassName, out parentTypeBuilder))
							throw new Exception("Parent class " + parentClassName + " is not registered");
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
						ConstructorBuilder constructorBuilder = typeBuilder.DefineConstructor(
							MethodAttributes.Public,
							CallingConventions.Standard,
							constructorDesc.argumentTypes.Select(resolveType).ToArray()
						);
						constructorBuilder.SetCustomAttribute(internalCallAttribute);
					}
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
					methodBuilder.SetCustomAttribute(internalCallAttribute);
				}
				// add static methods
				foreach(string staticMethodName in classDesc.staticMethods.Keys)
				{
					FunctionDesc functionDesc = classDesc.staticMethods[staticMethodName];
					MethodBuilder methodBuilder = typeBuilder.DefineMethod(
						staticMethodName,
						MethodAttributes.Public | MethodAttributes.Final,
						CallingConventions.Standard,
						resolveType(functionDesc.returnType),
						functionDesc.argumentTypes.Select(resolveType).ToArray()
					);
					methodBuilder.SetCustomAttribute(internalCallAttribute);
				}
			}

			// create types
			foreach(TypeBuilder typeBuilder in typeBuilders.Values)
				typeBuilder.CreateType();

			// save assembly
			assemblyBuilder.Save(dllName);
		}
	}
}

