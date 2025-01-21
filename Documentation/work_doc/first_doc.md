
Набор наблюдений по работе кода
===============================

успешное? создание cmake
------------------------

```
./util_cmake_converter -s ../GIT/GameCore/CvGameCoreDLL/CvGameCoreDLL.2012.sln
```


Точка входа
-----------

Внимание на

 * CvGameCoreDLL.h
 * CvGameCoreDLL.cpp


Глобальные настройки?
---------------------

CvGlobals.h
CvGlobals.cpp


Ошибки headers
--------------
в CvXMLLoadUtility.h ошибка на объявление CvDLLUtilityIFaceBase -?


Python API & SDK
----------------

В python вызывается:

```python
from CvPythonExtensions import *
```

Это обращение к ядру - в sdk python api ядра формируется в CyEnumsInterface.cpp ,
не исключено что гдето ещё? 
Вполне возможно что в sdk далеко не всё api ядра -
либо сам сдк должен буть ещё слинкован с недоступной (закрытой) частью кода.


CvDLLDataStreamIFaceBase.h
--------------------------
Он пустой - зачем он нужен ???
