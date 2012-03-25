#include "XafConverter.h"
#include <sstream>

/*
Функция выполняет конвертирование XML-файла анимации формата XAF в
файл анимации формата Inanity. Конвертируются все элементы, найденные в файле.
Если просят, можно создать также файл дерева шестеренок, олицетворяющий структуру костей.
Результат записывается как анимация вида LinearSequence.

Примечание к реализации
Слово анимация употребляется равносильно слову объект.
Предполагается, что все анимации одинаковой длины.
Первый кадр анимации считается "оригинальным" при создании структуры костей.
*/
void ConvertXafFile(const std::wstring& sourceFileName, const std::wstring& targetFileName, const std::wstring& gearTreeFileName)
{
	//открыть исходный XML-файл анимации
	ptr<FileSystem> fileSystem = new FolderFileSystem(L"");
	ptr<XmlElement> document = XmlElement::ReadFromFile(fileSystem->LoadFile(sourceFileName));

	//структура для хранения анимации
	struct Animation
	{
		//имя (берется из тега Node)
		std::string name;
		//имя родительского объекта
		std::string parentName;
		//номер родительского объекта
		int parentNumber;
		//массив времен
		Array<float> times;
		//массив абсолютных ключей
		Array<D3DXMATRIX> absoluteKeys;
		//массив относительных ключей
		Array<D3DXMATRIX> relativeKeys;

		Animation(const std::string& name, const std::string& parentName, const Array<float>& times, const Array<D3DXMATRIX>& keys)
			: name(name), parentName(parentName), times(times), absoluteKeys(keys), relativeKeys(keys.size())
		{
		}
	};
	//массив найденных анимаций
	std::vector<Animation> animations;
	//номера анимаций по именам
	std::map<std::string, unsigned> animationNumbers;

	//найти тег Node
	std::vector<ptr<XmlElement>>& elements = document->GetElements();
	for(unsigned i = 0; i < elements.size(); ++i)
		if(elements[i]->GetName() == "Node")
		{
			//найти тег Samples
			std::vector<ptr<XmlElement>>& nodeElements = elements[i]->GetElements();
			for(unsigned j = 0; j < nodeElements.size(); ++j)
				if(nodeElements[j]->GetName() == "Samples")
				{
					//получить сэмплы
					std::vector<ptr<XmlElement>>& samples = nodeElements[j]->GetElements();

					//создать массивы времен и ключей
					Array<float> times(samples.size());
					Array<D3DXMATRIX> keys(samples.size());

					//заполнить массивы
					for(unsigned k = 0; k < samples.size(); ++k)
					{
						times[k] = float(atoi(samples[k]->GetAttributes()["t"].c_str())) / (160 * 30);
						std::istringstream stream(samples[k]->GetAttributes()["v"]);
						D3DXMATRIX m;
						for(unsigned y = 0; y < 4; ++y)
						{
							for(unsigned x = 0; x < 3; ++x)
								stream >> m.m[y][x];
							m.m[y][3] = 0;
						}
						m.m[3][3] = 1;
						keys[k] = m;
					}

					//получить имя объекта
					std::string name = elements[i]->GetAttributes()["name"];
					//получить имя родительского объекта
					std::string parentName = elements[i]->GetAttributes()["parentNode"];

					//запомнить номер анимации по имени
					animationNumbers[name] = animations.size();
					//создать анимацию
					animations.push_back(Animation(name, parentName, times, keys));
				}
		}

	//теперь получим относительные анимации, которые и запишем в файлы
	for(unsigned i = 0; i < animations.size(); ++i)
	{
		//получить родительскую анимацию
		int parentAnimation;
		//получить трансформацию родительской анимации
		if(animations[i].parentName == "Scene Root")
			parentAnimation = -1;
		else
		{
			std::map<std::string, unsigned>::const_iterator it = animationNumbers.find(animations[i].parentName);
			if(it == animationNumbers.end())
				throw new Exception(L"Invalid parent node name");
			parentAnimation = it->second;
		}
		animations[i].parentNumber = parentAnimation;

		//получить собственно массивы
		//цикл по кадрам
		for(unsigned j = 0; j < animations[i].relativeKeys.size(); ++j)
		{
			//получить родительскую трансформацию
			D3DXMATRIX parentTransformation;
			if(parentAnimation < 0)
				D3DXMatrixIdentity(&parentTransformation);
			else
				parentTransformation = animations[parentAnimation].absoluteKeys[j];

			//обратить ее
			D3DXMATRIX invParentTransformation;
			D3DXMatrixInverse(&invParentTransformation, NULL, &parentTransformation);

			//и с помощью обратной матрицы получить относительную трансформацию
			animations[i].relativeKeys[j] = invParentTransformation * animations[i].absoluteKeys[j];
		}

		//создать окончательную анимацию и записать в файл
		ptr<LinearSequenceAnimation<D3DXMATRIX>> animation = new LinearSequenceAnimation<D3DXMATRIX>(animations[i].times, animations[i].relativeKeys);
		ptr<File> file = MatrixAnimationFormatter::Save(&*animation);
		//TEST: сохранение с номерами, пока не придумал, как делать маппинг
		wchar_t q[1000];
		_swprintf(q, L"%s%u.a", targetFileName.c_str(), i);
		fileSystem->SaveFile(file, q);
//		fileSystem->SaveFile(file, targetFileName + Strings::Ascii2Unicode(animations[i].name) + L".a");
	}

	//если нужно, сделать еще и дерево шестеренок
	if(gearTreeFileName.length())
	{
		//подсчитать, сколько некорневых анимаций
		//а заодно запомнить номера в списках исключенных шестеренок
		unsigned nonRootAnimationsCount = 0;
		std::vector<int> skippedNumbers(animations.size(), -1);
		for(unsigned i = 0; i < animations.size(); ++i)
			if(animations[i].parentNumber >= 0)
				skippedNumbers[i] = nonRootAnimationsCount++;

		/* Ситуация такая. На каждую корневую анимацию надо одну управляющую шестеренку.
		На каждую некорневую анимацию надо по одной комбинирующей, управляющей и константной
		шестеренке, всего три. */

		//создать сохранятель деревьев
		ptr<GearTreeSaver<D3DXMATRIX>> saver = new GearTreeSaver<D3DXMATRIX>(
			nonRootAnimationsCount,		//количество комбинирующих шестеренок
			animations.size(),				//количество управляющих шестеренок
			nonRootAnimationsCount);	//количество константных шестеренок

		/* Еще. Номером результирующей шестеренки является:
		для корневой анимации - saver->GetControlGearNumber(number)
		для некорневой анимации - saver->GetCombineGearNumber(skippedNumbers[number])
		*/

		//зададим комбинирующие и константные шестеренки
		for(unsigned i = 0; i < animations.size(); ++i)
			if(skippedNumbers[i] >= 0)
			{
				//в соответствии с вышесказанным получить номер шестеренки
				int parentResultGear = skippedNumbers[animations[i].parentNumber];
				if(parentResultGear >= 0)
					parentResultGear = saver->GetCombineGearNumber(parentResultGear);
				else
					parentResultGear = saver->GetControlGearNumber(animations[i].parentNumber);

				//указать источники для комбинирующей шестеренки
				//первый источник - это результирующая шестеренка родительской анимации
				//второй источник - это собственная управляющая шестеренка
				saver->SetCombineGearSources(skippedNumbers[i], parentResultGear, saver->GetControlGearNumber(i));
				//указать константную шестеренку, то есть оригинальный семпл
				saver->SetConstGearSample(skippedNumbers[i], animations[i].relativeKeys[0]);
			}

		//все, сохранить дерево шестеренок
		fileSystem->SaveFile(saver->Save(), gearTreeFileName);
	}
}
