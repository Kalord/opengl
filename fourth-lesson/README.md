# Четвертый урок
## План
1. Графический конвейер
2. Передача вершин
3. Вершинный шейдер
4. Фрагментный шейдер
5. Шейдерная программа
6. Связывание вершинных атрибутов
7. Vertex Array Object
8. Element Buffer Object

------------

### 1)Графический конвейер
OpenGL работает в 3D пространстве, монитор пользователя в свою очередь может отображать только 2D пространства, одна из задач графического конвейера - преобразовать 3D пространство в 2D пространство цветных пикселей на экране.
Этот процесс можно разделить на шесть этапов, каждый этап выполняется с помощью отдельной программы - **шейдера**.
**Шейдер  - ** это программа, которая работает на GPU.
Некоторые шейдер могут создаваться разработчиком, для дальнейшего исполнения.
Этапы графического конвейера:
1. Вершинный шейдер (может создваться разработчиком)
2. Формирование форм
3. Геометрический шейдер (может создваться разработчиком)
4. Растеризация
5. Фрагментный шейдер (может создваться разработчиком)
6. Тесты и смешивание

На вход графическому конвейеру передаются вершинные данные
Вершинные данные - набор вершин. Каждая вершина представлена атрибутами, эти атрибуты включают в себя координаты, цвет, дополнительные параметры и т.д.

Рассмотрим каждый этап графического конвейера.
**1) Вершинный шейдер** - принимает на вход вершину, его задача преобразование одних координат в другие. Данные процесс преобразования называется **нормализацией**. Обычно он сводится к тому, что преобразовывает все координаты в диапазон от -1.0 до 1.0. Об задачи нормализации будет сказано ниже.
**2) Формирование формы** - это этап, которые берет все вершины из вершинного шейдера и преобразует их в фигуру.
**3) Геометрический шейдер** - принимает на вход набор вершин, формирующих примитив. Работает уже с фигурой в целом, способен динамически ее дополнять.
**4) Растеризация** - примитив преобразовывается в пиксельное представление, формируя фрагмент. Растеризация может производится с помощью разных алгоритмов. Два алгоритма растеризации представлены в этом репозитории (DDA-Line, Алгоритм Брезенхэма) https://github.com/Tyutnev/rasterization
**5) Фрагментный шейдер** - вычисление цвета пикселей.
**6) Тесты и смешивание** - проверяет значения прозрачности и смешивает цвета.

------------

### 2)Передача вершин
Для отрисовки, нужно передать данные о вершинах, эти данные нужно передавать в трехмерном пространстве (x, y, z). OpenGL не преобразовывает все переданные ему 3D координаты в 2D пиксели на экране; OpenGL только обрабатывает 3D координаты в определенном промежутке между -1.0 и 1.0 по всем 3 координатам (x, y и z). Все такие координаты называются координатами, нормализованными под устройство (или просто нормализованными).
```cpp
//Определим вершины треугольника
//Данные о вершинах будем хранить в куче
GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};
```
**Нормализованные координаты - ** после того, как вершины будут обработаны в вершинном шейдере, они должны быть нормализованы, то есть находится в пределе от -1.0 до 1.0.
В отличии от экранных координат, положительное значение оси y указывает наверх, а координаты (0, 0) это центр графа, вместо верхнего левого угла.
![](https://hsto.org/files/b5e/861/5c2/b5e8615c2eac4c488adc727685ce14c8.png)
Ваши NDC координаты будут затем преобразованы в координаты экранного пространства через Viewport с использованием данных, предоставленных через вызов glViewport. Координаты экранного пространства затем трансформируются во фрагменты и подаются на вход фрагментному шейдеру.
После того, как мы определили вершины, мы должны их передать в память GPU, алгоритм передачи:
1.  Определение структуры данных VBO (vertex buffer objects), в которой будут хранится вершины для передачи.
2. Выделение памяти на GPU для VBO.
3. Передача

```cpp
	//vertex buffer objects
	//Будет хранить вершины нашего треугольника
	GLuint VBO;
	//Создание буфера
	glGenBuffers(1, &VBO);
	//Указание о том, что буфер имеет тип VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
```
Скопируем данные из выше описанного массива vertices в VBO
```cpp
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```
Рассмотрим детальную эту функцию:
1.Тип буффера, в который будут записаны данных, в нашем случае это VBO.
2. Размер данных, которые будут скопированы
3. Данные
4. Указание видеокарте на то, как она должна работать с данными. Существует три режима работы с данными: **GL_STATIC_DRAW** - данные не будут изменяться, или будут это делать очень редко. **GL_DYNAMIC_DRAW** - данные будут меняться.  **GL_STREAM_DRAW** - данные будут меняться при каждой отрисовке.

### 3)Вершинный шейдер
**Вершинный шейдер** - один из програмируемых шейдеров. Шейдеры создаются на Си-подобном языке GLSL. Код шейдера можно задать как простую строку, затем создать объект шейдера и скомпилировать его. Затем из отдельных шейдеров линкуется шейдерная программа.

```cpp
#version 330 core

layout (location = 0) in vec3 position;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}
``````cpp
//Установка версии шейдера
//Указание на то, что мы используем Core-profile
#version 330 core

//Указываем входные вершинные атрибуты
//Вершины имеют трехмерные координаты, поэтой причине мы создаем vec3
//Указав layout (location = 0) - задаем положение равное нулю
layout (location = 0) in vec3 position;

void main()
{
    //Для обозначения результата работы вершинного шейдера, мы должны присвоить
    //значение преопределенной переменной gl_Position
    //Это результат работы вершинного шейдера
    //Она имеет тип vec4
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}
```
Теперь мы можем определить глобальую переменную типа стринг на куче, и сохранить туда данный код.
```cpp
const GLchar* vertexShaderSource = "#version 330 core\n"
						"layout (location = 0) in vec3 position;\n"
						"void main()\n"
						"{\n"
						"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
						 "}\0";
```