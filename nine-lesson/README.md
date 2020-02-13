# Девятый урок

План урока
1. Пространство Вида
2. LookAt
3. Передвижение
4. Осмотр
5. Класс Camera

# 1)Пространство вида
Когда мы говорим о пространстве камеры/вида, мы подразумеваем вид всех вершин с точки зрения камеры, положение которой в этом прстранстве является базовой точкой начала координат: матрица вида трансформирует мировые координаты в координаты вида, измеряющиеся относительно расположения и направления камеры. Для однозначого математического описания камеры, нам необходимо ее положение в мировом пространстве, направление в котором она смотрит, вектор указывающий правое направление, и вектор указывающий направление вверх.
![](https://hsto.org/files/11f/4c4/861/11f4c4861254494fa06c3614e86b3a4e.png)

### Позиция камеры
Положение камеры это вектор, содержащий координаты камеры в мировом пространстве:
```c++
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
```

### Направление камеры
Следующим необходимым нам вектором, будет вектор направления камеры. Пока что камера нацелена в базовую точку нашей сцены: (0, 0, 0). Вычитание вектора положения камеры из точки начала координат даст нам вектор направления камеры. Мы знаем, что камера смотрит вдоль отрицательного направления оси-Z, а нам нужен вектор направленный вдоль положительной оси-Z самой камеры. Если при вычитании мы изменим очередность аргументов, то получим вектор, указывающий в положительном направлении оси-Z камеры:
```c++
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
```

### Правая ось
Еще один вектор, без которого нам не обойтись, это вектор указывающий в правую сторону и представляющий положительное направление оси-X камеры. Чтбы вычислить этот вектор, мы сделаем небольшой трюк, и для этого снача зададим вектор указывающий направление вверх. Затем мы векторно перемножим вычисленное в шаге 2 направление камеры и вектор указывающий вверх. Так как результатом векторного произведения является вектор, перпендикулярный исходным векторам, то мы получим вектор, указывающий в положительном направлении оси-X:
```c++
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
```

### Ось вверх
Теперь, когда у нас есть вектора двух осей X и Z, получить вектор, который бы указывал в положительном направлении оси-Y камеры, будет достаточно просто, мы сделаем векторное умножение правого вектора и вектора направления камеры:
```c++
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
```

С помощью векторного произведения и небольших хитростей мы смогли рассчитать все вектора, которые задают пространство Камеры/Вида. Данный алгоритм в линейной алгебре известен как процесс Грама-Шмидта. 

### 2)LookAt
Одно из замечательных свойств матриц заключается в том, что если вы зададите координатное пространство с помощью 3 перпендикулярных осей, то из векторов этих осей и дополнительного вектора смещений можно сформировать матрицу, уножение на которую будет преобразовывать любые вектора в это определенное вами координатное пространство. Это именно то, что делает матрица LookAt, и теперь, когда у нас есть все необходимые определения пространства Камеры/Вида, то есть 3 перпендикулярных оси и вектор расположения камеры, мы можем создать нашу собственну матрицу LookAt.
![](data:image/png;base64,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)

Где R - правый вектор, U - вектор, указывающий вверх, D - вектор направления камеры, а P - позиция камеры. Обратите внимание на то, что вектор положения камеры инвертирован, поскольку в конечном итоге мы будем сдвигать мировые координаты в направлении, противоположном движениию камеры. Использование матрицы LookAt в качестве матрицы вида позволяет эффективно преобразовать все мировые координаты в только что заданное нами пространство. Матрица LookAt делает именно то, о чем говорит ее название: она создает матрицу вида, которая смотрит на заданную цель.

К счастью, библиотека GLM сделаем всю эту работу за нас. Нам нужно только указать положение камеры, координату цели и вектор в мировом пространстве направления вверх. По этим данным GLM самостоятельно создает матрицу LookAt, которую мы сможем использовать в качестве нашей матрицы вида:
```c++
glm::mat4 view;
view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
  		   glm::vec3(0.0f, 0.0f, 0.0f), 
  		   glm::vec3(0.0f, 1.0f, 0.0f));
```

Функция glm::LookAt требует следующие параметры: позиция камеры, координата цели и вектор направленный вверх.

Прежде чем углубиться в вопросы, связанные с пользовательским вводом, давайте немного развлечёмся и покрутим камеру вокруг нашей сцены. Для простоты будем все время держать камеру направленной на точку (0,0,0).

Для вычисления позиции нашей камеры мы применим немного тригонометрии и рассчитаем для каждого кадра координаты X и Z, которые будут представлять собой точку, расположенную на круговой траектории. Пересчитывая в каждом кадре X и Z координаты, мы будем переходить по всем точкам круга и, таким образом, камера станет вращаться вокруг сцены. Давайте зададим размер этой окружности константным значением радиуса и, используя функцию glfwGetTime из библиотеки GLFW, вычислим для каждой итерации игрового цикла новую матрицу вида:
```c++
GLfloat radius = 10.0f;
GLfloat camX = sin(glfwGetTime()) * radius;
GLfloat camZ = cos(glfwGetTime()) * radius;
glm::mat4 view;
view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
```

Пример находится в этой же директории в поддиректории example.